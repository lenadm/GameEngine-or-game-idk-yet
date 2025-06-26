#include <unordered_map>
#include <string>

class AssetPathResolver {
public:
    void populateRegistry();
    std::string resolvePath(std::string filename);
private:
    std::unordered_map<std::string, std::string> m_registry;
};
