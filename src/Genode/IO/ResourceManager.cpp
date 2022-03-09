#include <Genode/IO/ResourceManager.hpp>
#include <Genode/IO/ResourceContext.hpp>

namespace Gx
{
    ResourceManager::ResourceManager() :
        m_containers()
    {
        m_contextFactory = [] (const std::string &id, ResourceManager &manager) {
            return std::make_unique<ResourceContext>(id, manager);
        };
    }

    ResourceManager::~ResourceManager()
    {
       Clear();
    }

    void ResourceManager::Clear()
    {
        m_containers.clear();
    }
}