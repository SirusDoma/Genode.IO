#ifndef GENODE_RESOURCE_MANAGER_HPP
#define GENODE_RESOURCE_MANAGER_HPP

#include <typeindex>
#include <functional>
#include <memory>

#include <SFML/System/InputStream.hpp>

#include <Genode/IO/ResourceContainer.hpp>
#include <Genode/IO/FileSystem.hpp>

namespace Gx
{
    class ResourceContext;

    /// Represents a manager that load, manage and instantiate various type of resources.
    class ResourceManager final : public NonCopyable
    {
    public:
        /// Initializes a new instance of ResourceManager.
        ResourceManager();

        /// Releases the resources inside the ResourceManager..
        ~ResourceManager();

        /// Acknowledge responsibility to manage the given type of resource for this instance of ResourceManager manually.
        /// This function is automatically called whenever there's an attempt to Add or Instantiate resource type that yet to be acknowledge by ResourceManager.
        /// \tparam R Type of Resource to acknowledge by this instance of ResourceManager.
        template<class R>
        void Register();

        /// Release responsibility from managing the given type of resource for this instance of ResourceManager.
        /// All resources that match with given type inside this ResourceManager will immediately destroyed.
        /// \tparam R Type of Resource to release by this instance of ResourceManager.
        /// \return a value indicating whether given type of resource is indeed managed by ResourceManager and successfully destroyed.
        template<class R>
        bool Release();

        /// Instantiate resource from ResourceManager if exists; otherwise, return nullptr.
        ///
        /// \remark
        /// The returned resource is produced by copying original resource within the ResourceManager and the ownership is completely transferred to the caller.
        /// Therefore, Any change that made to returned resource will not reflected to the resource within ResourceManager.
        /// ResourceManager will not responsible to manage its lifetime nor contain it.
        /// \tparam R Type of Resource to instantiate.
        /// \param id Value to identify the resource.
        /// \param fileName Path of the resource file to load.
        /// \return
        template<class R>
        std::unique_ptr<R> Instantiate(const std::string &id);

        /// Instantiate resource from ResourceManager if exists; otherwise, from a file.
        /// If resource is not exists within ResourceManager, this will add the resource with given \p id first via AddFromFile.
        ///
        /// \remark
        /// The returned resource is produced by copying original resource within the ResourceManager and the ownership is completely transferred to the caller.
        /// Therefore, Any change that made to returned resource will not reflected to the resource within ResourceManager.
        /// ResourceManager will not responsible to manage its lifetime nor contain it.
        /// \tparam R Type of Resource to instantiate.
        /// \param id Value to identify the resource.
        /// \param fileName Path of the resource file to load.
        /// \return
        template<class R>
        std::unique_ptr<R> Instantiate(const std::string &id, const std::string &fileName);

        /// Instantiate resource from ResourceManager if exists; otherwise, from a pointer of resource data.
        /// If resource is not exists within ResourceManager, this will add the resource with given \p id first via AddFromMemory.
        ///
        /// \remark
        /// The returned resource is produced by copying original resource within the ResourceManager and the ownership is completely transferred to the caller.
        /// Therefore, Any change that made to returned resource will not reflected to the resource within ResourceManager.
        /// ResourceManager will not responsible to manage its lifetime nor contain it.
        /// \tparam R Type of Resource to instantiate.
        /// \param data Pointer of the resource data to load.
        /// \param size Size of resource data, in bytes.
        /// \return
        template<class R>
        std::unique_ptr<R> Instantiate(const std::string &id, void *data, std::size_t size);

        /// Instantiate resource from ResourceManager if exists; otherwise, from a stream.
        /// If resource is not exists within ResourceManager, this will add the resource with given \p id first via AddFromStream.
        ///
        /// \remark
        /// The returned resource is produced by copying original resource within the ResourceManager and the ownership is completely transferred to the caller.
        /// Therefore, Any change that made to returned resource will not reflected to the resource within ResourceManager.
        /// ResourceManager will not responsible to manage its lifetime nor contain it.
        /// \tparam R Type of Resource to instantiate.
        /// \param stream Input stream that contains the data of resource to load.
        /// \return
        template<class R>
        std::unique_ptr<R> Instantiate(const std::string &id, sf::InputStream &stream);

        /// Instantiate resource from ResourceManager if exists; otherwise, from a deserialization function.
        /// If resource is not exists within ResourceManager, this will add the resource with given \p id first via AddFromDeserializer.
        ///
        /// \remark
        /// The returned resource is produced by copying original resource within the ResourceManager and the ownership is completely transferred to the caller.
        /// Therefore, Any change that made to returned resource will not reflected to the resource within ResourceManager.
        /// ResourceManager will not responsible to manage its lifetime nor contain it.
        /// \tparam R Type of Resource to instantiate.
        /// \param stream Input stream that contains the data of resource to load.
        /// \return
        template<class R>
        std::unique_ptr<R> Instantiate(const std::string &id, std::function<std::unique_ptr<R>()> deserializer);

        /// Add resource to this instance of ResourceManager from a file.
        /// \tparam R Type of Resource to load.
        /// \param id Value to identify the resource.
        /// \param fileName Path of the resource file to load.
        template<class R>
        R &AddFromFile(const std::string &id, const std::string &fileName, CacheMode mode = CacheMode::Reuse);

        /// Add resource to this instance of ResourceManager from a pointer of resource data.
        /// \tparam R Type of Resource to load.
        /// \param id Value to identify the resource.
        /// \param data Pointer of the resource data to load.
        /// \param size Size of resource data, in bytes.
        /// \return
        template<class R>
        R &AddFromMemory(const std::string &id, void *data, std::size_t size, CacheMode mode = CacheMode::Update);

        /// Add resource to this instance of ResourceManager from a stream.
        /// \tparam R Type of Resource to load.
        /// \param id Value to identify the resource.
        /// \param stream Input stream that contains the data of resource to load.
        /// \return
        template<class R>
        R &AddFromStream(const std::string &id, sf::InputStream &stream, CacheMode mode = CacheMode::Update);

        /// Add resource to this instance of ResourceManager from a deserializer function.
        /// \tparam R Type of Resource to load.
        /// \param id Value to identify the resource.
        /// \param stream Input stream that contains the data of resource to load.
        /// \return
        template<class R>
        R &AddFromDeserializer(const std::string &id, std::function<std::unique_ptr<R>()> deserializer, CacheMode mode = CacheMode::Update);

        /// Find resource that match with given type and id.
        /// \tparam R Type of Resource to find.
        /// \param id ID of Resource to retrieve from this instance of ResourceContainer.
        /// \return pointer of Resource if there's resource that match with the given id, otherwise, nullptr.
        template<class R>
        R *Find(const std::string &id) const;

        /// Destroy resource from this instance of ResourceManager.
        /// \param resource Resource to destroy from this instance of ResourceManager.
        /// \return true if resource is found and removed from this instance of ResourceManager, otherwise, false.
        template<class R>
        bool Destroy(const std::string &id);

        /// Destroy resource from this instance of ResourceManager.
        /// \param resource Resource to destroy from this instance of ResourceManager.
        /// \return true if resource is found and removed from this instance of ResourceManager, otherwise, false.
        template<class R>
        bool Destroy(const R& resource);

        /// Destroy all resources inside this instance of ResourceManager.
        void Clear();

    private:
        struct IManagedContainer
        {
            virtual ~IManagedContainer() = default;
        };

        template<class R>
        struct ManagedContainer : public IManagedContainer
        {
            explicit ManagedContainer(std::unique_ptr<ResourceContainer<R>> container) : Container(std::move(container)) {};
            ~ManagedContainer() override { Container = nullptr; };

            std::unique_ptr<ResourceContainer<R>> Container;
        };
        using ContainerMap   = std::unordered_map<std::type_index, std::unique_ptr<IManagedContainer>>;
        using ContextFactory = std::function<std::unique_ptr<ResourceContext>(const std::string&, ResourceManager&)>;

        ContainerMap   m_containers;
        ContextFactory m_contextFactory;
    };
}

#include <Genode/IO/ResourceManager.inl>
#endif //GENODE_RESOURCE_MANAGER_HPP
