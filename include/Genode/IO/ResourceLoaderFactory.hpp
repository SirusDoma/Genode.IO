#ifndef GENODE_RESOURCE_LOADER_FACTORY_HPP
#define GENODE_RESOURCE_LOADER_FACTORY_HPP

#include <typeindex>
#include <unordered_map>

#include <Genode/IO/IResourceLoader.hpp>

namespace Gx
{
    /// Provides factory functionalities that manage and instantiate a collection of IResourceLoader.
    class ResourceLoaderFactory
    {
    public:
        /// Register a new IResourceLoader.
        /// \tparam R Type of Resource that associated to the IResourceLoader.
        /// \tparam L Loader type that used for loading associated resource type.
        template<class R, class L>
        static void Register();

        /// Register a new IResourceLoader.
        /// \tparam R Type of Resource that associated to the IResourceLoader.
        /// \param loader A factory function that describe how the IResourceLoader for the associated resource type should be constructed.
        template<class R>
        static void Register(std::function<std::unique_ptr<IResourceLoader<R>>()> loader);

        /// Remove IResourceLoader from ResourceLoaderFactory.
        template<class R>
        static bool Remove();

        /// Instantiate the right IResourceLoader implementation for the given type of resource.
        /// \tparam R Type of Resource that wish to be loaded by IResourceLoader.
        /// \return One of IResourceLoader implementation that capable to load given type of resource.
        template<class R>
        static std::unique_ptr<IResourceLoader<R>> CreateResourceLoaderFor();

    private:
        struct ILoaderFactory {};

        template<class R>
        struct LoaderFactory : public ILoaderFactory
        {
            std::function<std::unique_ptr<IResourceLoader<R>>()> Create;
        };
        using LoaderMap = std::unordered_map<std::type_index, std::unique_ptr<ILoaderFactory>>;

        inline static LoaderMap m_loaders;
    };
}

#include <Genode/IO/ResourceLoaderFactory.inl>
#endif //GENODE_RESOURCE_LOADER_FACTORY_HPP
