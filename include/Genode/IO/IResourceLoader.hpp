#ifndef GENODE_RESOURCE_LOADER_HPP
#define GENODE_RESOURCE_LOADER_HPP

#include <memory>

namespace Gx
{
    class ResourceContext;

    /// Represents an interface that describe deserialization process for a particular resource type.
    /// \tparam R Represents type of resource that the loader is capable to load.
    template<class R>
    class IResourceLoader
    {
    public:
        /// Initializes a new instance of IResourceLoader.
        IResourceLoader() = default;

        /// Releases the resources used by IResourceLoader.
        virtual ~IResourceLoader() = default;

        /// Load resource from a file.
        /// \param fileName Path of the resource file to load.
        /// \param ctx Context of the resource loading execution.
        /// \return Pointer to the loaded resource.
        virtual std::unique_ptr<R> LoadFromFile(const std::string &fileName, const ResourceContext &ctx) = 0;

        /// Load resource from a pointer of resource data.
        /// \param data Pointer of the resource data to load.
        /// \param size Size of resource data, in bytes.
        /// \param ctx Context of the resource loading execution.
        /// \return Pointer to the loaded resource.
        virtual std::unique_ptr<R> LoadFromMemory(void *data, std::size_t size, const ResourceContext &ctx) = 0;

        /// Load resource from a stream.
        /// \param stream Input stream that contains the data of resource to load.
        /// \param ctx Context of the resource loading execution.
        /// \return Pointer to the loaded resource.
        virtual std::unique_ptr<R> LoadFromStream(sf::InputStream &stream, const ResourceContext &ctx) = 0;
    };
}

#endif //GENODE_RESOURCE_LOADER_HPP
