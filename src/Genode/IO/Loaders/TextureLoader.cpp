#include <Genode/IO/Loaders/TextureLoader.hpp>
#include <string>

#include <Genode/IO/ResourceContext.hpp>
#include <Genode/IO/FileSystem.hpp>

namespace Gx
{
    void TextureLoader::UseSmooth(bool smooth)
    {
        m_smooth = smooth;
    }

    std::unique_ptr<sf::Texture> TextureLoader::LoadFromFile(const std::string &fileName, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::Texture>();
        if (!resource->loadFromFile(Gx::FileSystem::GetFullName(fileName)))
            return nullptr;

        resource->setSmooth(m_smooth);
        return resource;
    }

    std::unique_ptr<sf::Texture> TextureLoader::LoadFromMemory(void *data, std::size_t size, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::Texture>();
        if (!resource->loadFromMemory(data, size))
            return nullptr;

        resource->setSmooth(m_smooth);
        return resource;
    }

    std::unique_ptr<sf::Texture> TextureLoader::LoadFromStream(sf::InputStream &stream, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::Texture>();
        if (!resource->loadFromStream(stream))
            return nullptr;

        resource->setSmooth(m_smooth);
        return resource;
    }
}

