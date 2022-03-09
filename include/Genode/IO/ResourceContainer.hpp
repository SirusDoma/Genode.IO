#ifndef GENODE_RESOURCE_CONTAINER_HPP
#define GENODE_RESOURCE_CONTAINER_HPP

#include <memory>
#include <string>
#include <functional>

#include <Genode/System/NonCopyable.hpp>

namespace Gx
{
    enum CacheMode
    {
        Allocate,
        Update,
        Reuse
    };

    /// Provides central point to store, access and destroy a particular type of resources.
    /// \tparam R Type of resources that stored inside ResourceContainer.
    template<class R>
    class ResourceContainer final : private NonCopyable
    {
    public:
        /// Initializes a new instance of ResourceContainer
        ResourceContainer() = default;

        /// Releases the resources inside the ResourceContainer.
        ~ResourceContainer();

        /// Store given resource that could be identified with given \p id to the ResourceContainer
        /// \param id Value to identify the given resource.
        /// \param resource Resource to store inside container.
        /// \param mode Specifies store mode to use when storing the resource into this instance of ResourceContainer.
        /// \return Reference to Resource that successfully stored into this instance of ResourceContainer.
        R &Store(const std::string &id, std::unique_ptr<R> resource, CacheMode mode = CacheMode::Update);

        /// Store resource to the ResourceContainer by using given resource deserialization function in which the resulting resource could be identified with given \p id.
        /// \param id Value to identify the resource that produced by deserializer.
        /// \param deserializer Resource deserialization function which describe how resource get loaded.
        /// \param mode Specifies store mode to use when storing the resource into this instance of ResourceContainer.
        /// \return Reference to Resource that successfully stored into this instance of ResourceContainer.
        R &Store(const std::string &id, std::function<std::unique_ptr<R>()> deserializer, CacheMode mode = CacheMode::Reuse);

        /// Destroy resource from this instance of ResourceContainer.
        /// \param resource Resource to destroy from this instance of ResourceContainer.
        /// \return true if resource is found and removed from this instance of ResourceContainer; otherwise, false.
        bool Destroy(const R& resource);

        /// Destroy resource from this instance of ResourceContainer.
        /// \param id ID of Resource to destroy from this instance of ResourceContainer.
        /// \return true if resource is found and removed from this instance of ResourceContainer; otherwise, false.
        bool Destroy(const std::string &id);

        /// Find resource that match with given \p id.
        /// \param id ID of Resource to retrieve from this instance of ResourceContainer.
        /// \return pointer of Resource if there's resource that match with the given id; otherwise, nullptr.
        R *Find(const std::string &id) const;

        /// Gets a value indicate whether there's a resource with id that match with given \p id inside this instance of ResourceContainer.
        /// \param id ID of Resource to check.
        /// \return true if Resource is found; otherwise, false.
        bool Contains(const std::string &id) const;

        /// Gets the number of resources inside this instance of ResourceContainer.
        /// \return The number of resources inside this instance of ResourceContainer
        std::size_t Count() const;

        /// Destroy all resources inside this instance of ResourceContainer.
        void Clear();

    private:
        using ResourceMap = std::unordered_map<std::string, std::unique_ptr<R>>;

        ResourceMap m_resources;
    };
}

#include <Genode/IO/ResourceContainer.inl>
#endif //GENODE_RESOURCE_CONTAINER_HPP
