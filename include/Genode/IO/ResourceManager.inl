#include <Genode/IO/ResourceLoaderFactory.hpp>
#include <Genode/IO/ResourceContext.hpp>
#include "ResourceManager.hpp"


namespace Gx
{
    template<class R>
    void ResourceManager::Register()
    {
        if (m_containers.find(typeid(R)) != m_containers.end())
            return;

        m_containers[typeid(R)] = std::make_unique<ManagedContainer<R>>(std::make_unique<ResourceContainer<R>>());
    }

    template<class R>
    bool ResourceManager::Release()
    {
        return m_containers.erase(typeid(R)) != 0;
    }

    template<class R>
    std::unique_ptr<R> ResourceManager::Instantiate(const std::string &id)
    {
        Register<R>();

        auto resource = Find<R>(id);
        if (!resource)
            return nullptr;

        return std::make_unique<R>(*resource);
    }

    template<class R>
    std::unique_ptr<R> ResourceManager::Instantiate(const std::string &id, const std::string &fileName)
    {
        Register<R>();

        auto resource = &AddFromFile<R>(id, fileName, CacheMode::Reuse);
        return std::make_unique<R>(*resource);
    }

    template<class R>
    std::unique_ptr<R> ResourceManager::Instantiate(const std::string &id, void *data, std::size_t size)
    {
        Register<R>();

        auto resource = &AddFromMemory<R>(id, data, size, CacheMode::Reuse);
        return std::make_unique<R>(*resource);
    }

    template<class R>
    std::unique_ptr<R> ResourceManager::Instantiate(const std::string &id, sf::InputStream &stream)
    {
        Register<R>();

        auto resource = &AddFromStream<R>(id, stream, CacheMode::Reuse);
        return std::make_unique<R>(*resource);
    }

    template<class R>
    std::unique_ptr<R>
    ResourceManager::Instantiate(const std::string &id, std::function<std::unique_ptr<R>()> deserializer)
    {
        Register<R>();

        auto resource = &AddFromStream<R>(id, deserializer, CacheMode::Reuse);
        return std::make_unique<R>(*resource);
    }

    template<class R>
    R &ResourceManager::AddFromFile(const std::string &id, const std::string &fileName, CacheMode mode)
    {
        Register<R>();

        auto loader = ResourceLoaderFactory::CreateResourceLoaderFor<R>();
        if (!loader)
            throw ResourceLoadException("There's no [ResourceLoader] for [" + std::string(typeid(R).name()) + "] type.");

        auto managed = static_cast<ManagedContainer<R>*>(m_containers[typeid(R)].get());
        auto deserializer = [&, this] () {
            auto ctx = std::move(m_contextFactory(id, *this));
            return loader->LoadFromFile(fileName, *ctx);
        };

        return managed->Container->Store(id, deserializer, mode);
    }

    template<class R>
    R &ResourceManager::AddFromMemory(const std::string &id, void *data, std::size_t size, CacheMode mode)
    {
        Register<R>();

        auto loader = ResourceLoaderFactory::CreateResourceLoaderFor<R>();
        if (!loader)
            throw ResourceLoadException("There's no [ResourceLoader] for [" + std::string(typeid(R).name()) + "] type.");

        auto managed = static_cast<ManagedContainer<R>*>(m_containers[typeid(R)].get());
        auto deserializer = [&, this] () {
            auto ctx = std::move(m_contextFactory(id, *this));
            return loader->LoadFromMemory(data, size, *ctx);
        };

        return managed->Container->Store(id, deserializer, mode);
    }

    template<class R>
    R &ResourceManager::AddFromStream(const std::string &id, sf::InputStream &stream, CacheMode mode)
    {
        Register<R>();

        auto loader = ResourceLoaderFactory::CreateResourceLoaderFor<R>();
        if (!loader)
            throw ResourceLoadException("There's no [ResourceLoader] for [" + std::string(typeid(R).name()) + "] type.");

        auto managed = static_cast<ManagedContainer<R>*>(m_containers[typeid(R)].get());
        auto deserializer = [&, this] () {
            auto ctx = std::move(m_contextFactory(id, *this));
            return loader->LoadFromStream(stream, *ctx);
        };

        return &managed->Container->Store(id, deserializer, mode);
    }

    template<class R>
    R &ResourceManager::AddFromDeserializer(const std::string &id, typename std::function<std::unique_ptr<R>()> deserializer, CacheMode mode)
    {
        Register<R>();

        auto managed = static_cast<ManagedContainer<R>*>(m_containers[typeid(R)].get());
        auto &result = managed->Container->Store(id, deserializer, mode);

        return result;
    }

    template<class R>
    R *ResourceManager::Find(const std::string &id) const
    {
        auto it = m_containers.find(typeid(R));
        if (it == m_containers.end())
            return nullptr;

        auto managed = dynamic_cast<ManagedContainer<R>*>(it->second.get());
        if (!managed)
            return nullptr;

        return managed->Container->Find(id);
    }

    template<class R>
    bool ResourceManager::Destroy(const R &resource)
    {
        auto it = m_containers.find(typeid(R));
        if (it == m_containers.end())
            return false;

        auto managed = static_cast<ManagedContainer<R>*>(it->second.get());
        return managed->Container->Destroy(resource);
    }

    template<class R>
    bool ResourceManager::Destroy(const std::string &id)
    {
        Register<R>();

        auto managed = static_cast<ManagedContainer<R>*>(m_containers[typeid(R)].get());
        return managed->Container->Destroy(id);
    }
}