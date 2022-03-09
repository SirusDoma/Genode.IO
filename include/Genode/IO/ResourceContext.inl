#include <Genode/IO/ResourceManager.hpp>

namespace Gx
{
    template<class R>
    R& ResourceContext::Acquire(const std::string &id) const
    {
        if (!m_resources)
            throw ResourceAccessException(id, "ResourceManager is not set within this context.");

        auto resource = m_resources->Find<R>(id);
        if (!resource)
            throw ResourceAccessException(id);

        return *resource;
    }

    template<class R>
    R& ResourceContext::Acquire(const std::string &id, const std::string &path) const
    {
        if (!m_resources)
            throw ResourceAccessException(id, "ResourceManager is not set within this context.");

        return m_resources->AddFromFile<R>(id, path, CacheMode::Reuse);
    }

    template<class R>
    R& ResourceContext::Acquire(const std::string &id, const void *data, std::size_t dataSize) const
    {
        if (!m_resources)
            throw ResourceAccessException(id, "ResourceManager is not set within this context.");

        return m_resources->AddFromMemory<R>(id, data, dataSize, CacheMode::Reuse);
    }

    template<class R>
    R& ResourceContext::Acquire(const std::string &id, sf::InputStream &stream) const
    {
        if (!m_resources)
            throw ResourceAccessException(id, "ResourceManager is not set within this context.");

        return m_resources->AddFromStream<R>(id, stream, CacheMode::Reuse);
    }
}