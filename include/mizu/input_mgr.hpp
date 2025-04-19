#ifndef MIZU_INPUT_MGR_HPP
#define MIZU_INPUT_MGR_HPP

#include "mizu/callback_mgr.hpp"
#include "mizu/payloads.hpp"

namespace mizu {
class InputMgr {
public:
    explicit InputMgr(CallbackMgr &callbacks);

    ~InputMgr();

    InputMgr(const InputMgr &) = delete;
    InputMgr &operator=(const InputMgr &) = delete;

    InputMgr(InputMgr &&other) noexcept = delete;
    InputMgr &operator=(InputMgr &&other) = delete;

private:
    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void register_callbacks_();
    void unregister_callbacks_();

    void update_(double dt);

    void key_down_(const PEventKeyDown &p);
    void key_up_(const PEventKeyUp &p);
};
} // namespace mizu

#endif // MIZU_INPUT_MGR_HPP
