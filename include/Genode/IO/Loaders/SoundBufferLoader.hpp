#ifndef GENODE_SOUND_BUFFER_LOADER_HPP
#define GENODE_SOUND_BUFFER_LOADER_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include <Genode/IO/IResourceLoader.hpp>

namespace Gx
{
    class SoundBufferLoader : public IResourceLoader<sf::SoundBuffer>
    {
    public:
        SoundBufferLoader() = default;

        std::unique_ptr<sf::SoundBuffer> LoadFromFile(const std::string &fileName, const ResourceContext &ctx) override;
        std::unique_ptr<sf::SoundBuffer> LoadFromMemory(void *data, std::size_t size, const ResourceContext &ctx) override;
        std::unique_ptr<sf::SoundBuffer> LoadFromStream(sf::InputStream &stream, const ResourceContext &ctx) override;
    };
}

#endif //GENODE_SOUND_BUFFER_LOADER_HPP
