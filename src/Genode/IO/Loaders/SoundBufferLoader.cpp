#include <Genode/IO/Loaders/SoundBufferLoader.hpp>

namespace Gx
{
    std::unique_ptr<sf::SoundBuffer> SoundBufferLoader::LoadFromFile(const std::string &fileName, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::SoundBuffer>();
        if (!resource->loadFromFile(fileName))
            return nullptr;

        return resource;
    }

    std::unique_ptr<sf::SoundBuffer> SoundBufferLoader::LoadFromMemory(void *data, std::size_t size, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::SoundBuffer>();
        if (!resource->loadFromMemory(data, size))
            return nullptr;

        return resource;
    }

    std::unique_ptr<sf::SoundBuffer> SoundBufferLoader::LoadFromStream(sf::InputStream &stream, const ResourceContext &ctx)
    {
        auto resource = std::make_unique<sf::SoundBuffer>();
        if (!resource->loadFromStream(stream))
            return nullptr;

        return resource;
    }
}