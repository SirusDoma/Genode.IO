#include <Genode/IO/IOException.hpp>
#include "ResourceContainer.hpp"

namespace Gx
{
    template<class R>
    R &ResourceContainer<R>::Store(const std::string &id, std::unique_ptr<R> resource, CacheMode mode)
    {
        if (resource == nullptr)
            throw ResourceStoreException(id, "[" + id + "] Cannot store nullptr resource.");

        auto current = Find(id);
        if (current)
        {
            if (mode == CacheMode::Allocate)
                throw ResourceStoreException(id, "[" + id + "] Resource with same ID is already exists.");
            else if (mode == CacheMode::Reuse)
                return *current;
        }

        m_resources[id] = std::move(resource);
        return *m_resources[id].get();
    }

    template<class R>
    R &ResourceContainer<R>::Store(const std::string &id, std::function<std::unique_ptr<R>()> deserializer, CacheMode mode)
    {
        auto current = Find(id);
        if (current)
        {
            if (mode == CacheMode::Allocate)
                throw ResourceStoreException(id, "[" + id + "] Resource with same ID is already exists.");
            else if (mode == CacheMode::Reuse)
                return *current;
        }

        auto resource = deserializer();
        if (resource == nullptr)
            throw ResourceStoreException(id, "[" + id + "] Cannot store nullptr resource.");

        m_resources[id] = std::move(resource);
        return *m_resources[id].get();
    }

    template<class R>
    bool ResourceContainer<R>::Destroy(const R &resource)
    {
        for (auto it = m_resources.begin(); it != m_resources.end(); it++)
        {
            if (it->second.get() == &resource)
            {
                m_resources.erase(it++);
                return true;
            }
        }

        return false;
    }

    template<class R>
    bool ResourceContainer<R>::Destroy(const std::string &id)
    {
        return m_resources.erase(id) != 0;
    }

    template<class R>
    R *ResourceContainer<R>::Find(const std::string &id) const
    {
        if (auto it = m_resources.find(id); it != m_resources.end())
            return it->second.get();

        return nullptr;
    }

    template<class R>
    bool ResourceContainer<R>::Contains(const std::string &id) const
    {
        return m_resources.find(id) != m_resources.end();
    }

    template<class R>
    std::size_t ResourceContainer<R>::Count() const
    {
        return m_resources.size();
    }

    template<class R>
    void ResourceContainer<R>::Clear()
    {
        m_resources.clear();
    }

    template<class R>
    ResourceContainer<R>::~ResourceContainer()
    {
        Clear();
    }
}