#include <Genode/IO/Loaders/FontLoader.hpp>

namespace Gx
{
    void FontLoader::UseSmooth(bool smooth)
    {
        m_smooth = smooth;
    }

    std::unique_ptr<sf::Font> FontLoader::LoadFromFile(const std::string &fileName, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::Font>();
        if (!resource->loadFromFile(fileName))
            return nullptr;

        resource->setSmooth(m_smooth);
        return resource;
    }

    std::unique_ptr<sf::Font> FontLoader::LoadFromMemory(void *data, std::size_t size, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::Font>();
        if (!resource->loadFromMemory(data, size))
            return nullptr;

        resource->setSmooth(m_smooth);
        return resource;
    }

    std::unique_ptr<sf::Font> FontLoader::LoadFromStream(sf::InputStream &stream, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::Font>();
        if (!resource->loadFromStream(stream))
            return nullptr;

        resource->setSmooth(m_smooth);
        return resource;
    }
}