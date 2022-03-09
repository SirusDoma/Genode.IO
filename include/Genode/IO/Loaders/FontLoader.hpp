#ifndef GENODE_FONT_LOADER_HPP
#define GENODE_FONT_LOADER_HPP

#include <SFML/Graphics/Font.hpp>
#include <Genode/IO/IResourceLoader.hpp>

namespace Gx
{
    class FontLoader : public IResourceLoader<sf::Font>
    {
    private:
        bool m_smooth = true;

    public:
        FontLoader() = default;
        void UseSmooth(bool smooth);

        std::unique_ptr<sf::Font> LoadFromFile(const std::string &fileName, const ResourceContext &ctx) override;
        std::unique_ptr<sf::Font> LoadFromMemory(void *data, std::size_t size, const ResourceContext &ctx) override;
        std::unique_ptr<sf::Font> LoadFromStream(sf::InputStream &stream, const ResourceContext &ctx) override;
    };
}

#endif //GENODE_FONT_LOADER_HPP
