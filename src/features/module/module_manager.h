
#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "bb_module.h"


enum class bb_module_category {
    RENDER,
    COMBAT,
    LEGIT,
    PLAYER,
    CLIENT
};


class module_manager {
    std::vector<std::unique_ptr<bb_module>> modules{};
    std::unordered_map<std::string, bb_module *> module_map{};

    module_manager() = default;

public:
    static inline bool initialized = false;

    static module_manager& get() {
        static module_manager instance;
        return instance;
    }

    void init();

    [[nodiscard]] std::vector<bb_module *> get_modules() const;

    [[nodiscard]] bb_module *get_module(const std::string& name) const;

    void run_all_modules();

    void enable_defaults() const;

    void WndProc_handler(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param);
};


#endif //MODULE_MANAGER_H
