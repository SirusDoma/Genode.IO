#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Genode/IO/Loaders/TextureLoader.hpp>
#include <Genode/IO/Loaders/FontLoader.hpp>
#include <Genode/IO/Loaders/SoundBufferLoader.hpp>

namespace
{
    void EnsureDefaultLoadersRegistered()
    {
        static bool registered = false;
        if (!registered)
        {
            Gx::ResourceLoaderFactory::Register<sf::Texture,     Gx::TextureLoader>();
            Gx::ResourceLoaderFactory::Register<sf::Font,        Gx::FontLoader>();
            Gx::ResourceLoaderFactory::Register<sf::SoundBuffer, Gx::SoundBufferLoader>();

            registered = true;
        }
    }
}

namespace Gx
{
    template<class R, class L>
    void ResourceLoaderFactory::Register()
    {
        static_assert(std::is_base_of<IResourceLoader<R>, L>::value, "Type of Loader must inherit Gx::ResourceLoader<R>");
        Remove<R>();

        auto factory = std::make_unique<LoaderFactory<R>>();
        factory->Create = [] { return std::make_unique<L>(); };

        m_loaders[typeid(R)] = std::move(factory);
    }

    template<class R>
    void ResourceLoaderFactory::Register(std::function<std::unique_ptr<IResourceLoader<R>>()> loader)
    {
        Remove<R>();

        auto factory = std::make_unique<LoaderFactory<R>>();
        factory->Create = loader;

        m_loaders[typeid(R)] = std::move(factory);
    }

    template<class R>
    bool ResourceLoaderFactory::Remove()
    {
        return m_loaders.erase(typeid(R)) != 0;
    }

    template<class R>
    std::unique_ptr<IResourceLoader<R>> ResourceLoaderFactory::CreateResourceLoaderFor()
    {
        EnsureDefaultLoadersRegistered();

        auto it = m_loaders.find(typeid(R));
        if (it == m_loaders.end())
            return nullptr;

        auto factory = static_cast<LoaderFactory<R>*>(it->second.get());
        return factory->Create();
    }
}