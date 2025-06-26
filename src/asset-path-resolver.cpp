#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <whereami.h>

#include "asset-path-resolver.h"

namespace fs = std::filesystem;

fs::path g_assetsRoot;

void setAssetsRoot();

void AssetPathResolver::populateRegistry() {
    setAssetsRoot();
    for (const auto& entry : fs::recursive_directory_iterator(g_assetsRoot)) {
        if (entry.is_regular_file()) {
            fs::path entryPath = entry.path();
            fs::path relativePath = fs::relative(entryPath, g_assetsRoot);
            m_registry.insert({entryPath.filename().string(), relativePath.string()});
        }
    }
}

std::string AssetPathResolver::resolvePath(std::string name) {
    fs::path relativePath = fs::path(m_registry.at(name));
    return (g_assetsRoot / relativePath).string();
}

void setAssetsRoot() {
    try {
        size_t length = wai_getExecutablePath(NULL, 0, NULL);
        char *buf = new char[length];
        wai_getExecutablePath(buf, length, NULL);
        fs::path execPath(buf);
        g_assetsRoot = execPath.parent_path() / fs::path("assets");
        if (!fs::exists(g_assetsRoot)) {
            std::cout << "ERROR: the executables parent directory does not contain assets\n";
            std::exit(1);
        }
    } catch (std::exception& err) {
        std::cout << err.what();
        std::exit(1);
    }
}
