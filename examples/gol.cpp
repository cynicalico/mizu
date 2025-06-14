#include "mizu/mizu.hpp"

using RuleString = std::vector<size_t>;
using CellState = std::vector<std::uint32_t>;

const std::size_t ROWS = 100;
const std::size_t COLS = 100;

const RuleString BORN = RuleString{3};
const RuleString STAY = RuleString{2, 3};

const auto SIM_DELAY = std::chrono::milliseconds(50);

const float CELL_SIZE = 10.0f;
const float WINDOW_PADDING = CELL_SIZE;

const auto BG_COLOR = mizu::rgb(0x14182e);
const auto GRID_COLOR = mizu::rgb(0x2b2b45);
const auto TEXT_COLOR = mizu::rgb(0xf5ffe8);
const std::vector CELL_COLORS{
        mizu::rgb(0x4f1d4c),
        mizu::rgb(0x781d4f),
        mizu::rgb(0xad2f45),
        mizu::rgb(0xe64539),
        mizu::rgb(0xff8933),
        // mizu::rgb(0xffc2a1),
        mizu::rgb(0xf5ffe8)};

const auto ALIVE = CELL_COLORS.size() - 1;

class GameOfLife final : public mizu::Application {
public:
    mizu::G2d *g2d;
    mizu::InputMgr *input;
    mizu::Window *window;

    std::unique_ptr<mizu::Texture> font_tex;
    std::unique_ptr<mizu::CodePage437> font;

    CellState state;
    CellState next_state;
    bool simulating;
    mizu::Ticker<> simulation_ticker;

    GameOfLife(mizu::Engine *engine);

    bool mouse_in_bounds() const;
    std::size_t idx_from_mouse_pos() const;

    std::size_t count_neighbors(std::size_t state_r, std::size_t state_c) const;
    void simulate();

    void update(double dt) override;

    void draw() override;
};

GameOfLife::GameOfLife(mizu::Engine *engine)
    : Application(engine), g2d(engine->g2d.get()), input(engine->input.get()), window(engine->window.get()) {
    window->set_icon_dir("examples/icon/gol");

    font_tex = g2d->load_texture(
            "examples/font/1px_7x9_no_bg.png", gloo::MinFilter::NearestMipmapNearest, gloo::MagFilter::Nearest);
    font = std::make_unique<mizu::CodePage437>(g2d, *font_tex, glm::uvec2{7, 9}, 2);

    state = CellState(ROWS * COLS);
    next_state = CellState(ROWS * COLS);
    simulating = false;
    simulation_ticker = mizu::Ticker(SIM_DELAY);

    g2d->set_vsync(false);
}

bool GameOfLife::mouse_in_bounds() const {
    return input->mouse_x() > WINDOW_PADDING && input->mouse_x() < window->size().x - WINDOW_PADDING - 1 &&
           input->mouse_y() > WINDOW_PADDING && input->mouse_y() < window->size().y - WINDOW_PADDING - 1;
}

std::size_t GameOfLife::idx_from_mouse_pos() const {
    const auto r = static_cast<std::size_t>((input->mouse_y() - WINDOW_PADDING) / (CELL_SIZE + 1));
    const auto c = static_cast<std::size_t>((input->mouse_x() - WINDOW_PADDING) / (CELL_SIZE + 1));
    return r * COLS + c;
}

std::size_t GameOfLife::count_neighbors(std::size_t state_r, std::size_t state_c) const {
    std::size_t count = 0;

    const auto start_r = (state_r == 0) ? 0 : state_r - 1;
    const auto end_r = (state_r == ROWS - 1) ? ROWS - 1 : state_r + 1;
    const auto start_c = (state_c == 0) ? 0 : state_c - 1;
    const auto end_c = (state_c == COLS - 1) ? COLS - 1 : state_c + 1;
    for (std::size_t r = start_r; r <= end_r; ++r) {
        for (std::size_t c = start_c; c <= end_c; ++c)
            if (!(r == state_r && c == state_c) && state[r * COLS + c] == ALIVE)
                ++count;
    }

    return count;
}

void GameOfLife::simulate() {
    for (std::size_t r = 0; r < ROWS; ++r) {
        for (std::size_t c = 0; c < COLS; ++c) {
            const auto n = count_neighbors(r, c);
            if (state[r * COLS + c] == ALIVE)
                if (std::ranges::contains(STAY, n))
                    next_state[r * COLS + c] = ALIVE;
                else
                    next_state[r * COLS + c] = ALIVE - 1;
            else if (std::ranges::contains(BORN, n))
                next_state[r * COLS + c] = ALIVE;
            else if (state[r * COLS + c] > 0)
                next_state[r * COLS + c] = state[r * COLS + c] - 1;
            else
                next_state[r * COLS + c] = 0;
        }
    }
    std::swap(state, next_state);
}

void GameOfLife::update(double dt) {
    if (simulating) {
        auto ticks = simulation_ticker.tick();
        for (auto i = 0; i < ticks; ++i)
            simulate();
    }

    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input->down(mizu::MouseButton::Left) && mouse_in_bounds())
        state[idx_from_mouse_pos()] = ALIVE;

    if (input->down(mizu::MouseButton::Right) && mouse_in_bounds())
        state[idx_from_mouse_pos()] = 0;

    if (input->pressed(mizu::Key::Space)) {
        simulating = !simulating;
        if (simulating)
            simulation_ticker.reset();
    }
}

void GameOfLife::draw() {
    g2d->clear(BG_COLOR);

    for (std::size_t i = 0; i < COLS + 1; ++i) {
        const auto x = WINDOW_PADDING + i + i * CELL_SIZE;
        g2d->line({x, WINDOW_PADDING}, {x, window->size().y - WINDOW_PADDING}, GRID_COLOR);
    }

    for (std::size_t i = 0; i < ROWS + 1; ++i) {
        const auto y = WINDOW_PADDING + i + i * CELL_SIZE;
        g2d->line({WINDOW_PADDING, y}, {window->size().x - WINDOW_PADDING, y}, GRID_COLOR);
    }

    for (std::size_t r = 0; r < ROWS; ++r) {
        for (std::size_t c = 0; c < COLS; ++c) {
            if (state[r * COLS + c] > 0) {
                const auto x = WINDOW_PADDING + 1 + c + c * CELL_SIZE;
                const auto y = WINDOW_PADDING + 1 + r + r * CELL_SIZE;
                g2d->fill_rect({x, y}, {CELL_SIZE, CELL_SIZE}, CELL_COLORS[state[r * COLS + c]]);
            }
        }
    }
}

int main(int, char *[]) {
    const auto WINDOW_W = COLS * CELL_SIZE + (COLS + 1) + WINDOW_PADDING * 2;
    const auto WINDOW_H = ROWS * CELL_SIZE + (ROWS + 1) + WINDOW_PADDING * 2;

    mizu::Engine("Game of Life", {WINDOW_W, WINDOW_H}, [](auto &) {}).mainloop<GameOfLife>();
}
