#include <unordered_map>
#include <string>
#include <filesystem>

class AssetPathResolver {
public:
    void populateRegistry();
    std::filesystem::path resolvePath(std::string filename);
private:
    std::unordered_map<std::string, std::filesystem::path> m_registry;
};
