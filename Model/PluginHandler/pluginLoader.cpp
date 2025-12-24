#include "pluginLoader.hpp"

#include <dlfcn.h>
#include <stdexcept>

#include "../Commands/interface/rootCommand.hpp"

namespace {
    struct DlCloser {
        void operator()(void* h) const noexcept {
            if (h) dlclose(h);
        }
    };

    template <class T>
    T load_sym(void* handle, const char* name) {
        dlerror();
        void* p = dlsym(handle, name);
        if (const char* err = dlerror()) throw std::runtime_error(std::string("failed: ") + name + ": " + err);
        return reinterpret_cast<T>(p);
    }
}

PluginLoader::LoadedPlugin PluginLoader::load(const std::string& path) {
    void* raw = dlopen(path.c_str(), RTLD_NOW);
    if (!raw) throw std::runtime_error(std::string("failed: ") + dlerror());

    std::shared_ptr<void> lib(raw, DlCloser{});

    auto getData  = load_sym<PluginGD>(raw, PluginGetData);
    auto create   = load_sym<PluginCC>(raw, PluginCreateCommand);
    auto destroy  = load_sym<PluginDC>(raw, PluginDestroyCommand);

    PluginCommandInfo info{};
    if (!getData(&info)) throw std::runtime_error("getData returned false");
    if (!info.command_name || info.command_name[0] == '\0')
        throw std::runtime_error("Empty command name");

    void* obj = create();
    if (!obj) throw std::runtime_error("nullptr as ptr to command");

    std::shared_ptr<RootCommand> cmd(
        static_cast<RootCommand*>(obj),
        [destroy, lib](RootCommand* p) {
            destroy(static_cast<void*>(p));
        }
    );

    return LoadedPlugin{info, std::move(cmd), std::move(lib)};
}
