#ifndef GENODE_TEXTURE_LOADER_HPP
#define GENODE_TEXTURE_LOADER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <Genode/IO/IResourceLoader.hpp>

namespace Gx
{
    class TextureLoader : public IResourceLoader<sf::Texture>
    {
    private:
        bool m_smooth = true;

    public:
        TextureLoader() = default;
        void UseSmooth(bool smooth);

        std::unique_ptr<sf::Texture> LoadFromFile(const std::string &fileName, const ResourceContext &ctx) override;
        std::unique_ptr<sf::Texture> LoadFromMemory(void *data, std::size_t size, const ResourceContext &ctx) override;
        std::unique_ptr<sf::Texture> LoadFromStream(sf::InputStream &stream, const ResourceContext &ctx) override;
    };
}

#endif //GENODE_TEXTURE_LOADER_HPP
