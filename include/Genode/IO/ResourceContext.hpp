#ifndef GENODE_RESOURCE_CONTEXT_HPP
#define GENODE_RESOURCE_CONTEXT_HPP

#include <SFML/System/FileInputStream.hpp>

namespace Gx
{
    class ResourceManager;
    class ResourceContext
    {
    public:
        /// Initializes a new instance of ResourceContext.
        /// \param id ID of resource that about to load.
        explicit ResourceContext(std::string id);

        /// Initializes a new instance of ResourceContext with Resource ID and ResourceManager.
        /// \param id ID of resource that about to load.
        /// \param resources ResourceManager to use when resolving resource dependency.
        ResourceContext(std::string id, ResourceManager &resources);

        /// Gets default context when using loader independently.
        static const ResourceContext Default;

        /// Gets ID of resource that about to load.
        const std::string &GetID() const;

        /// Acquire resource dependency that match with given Resource Type and Resource ID.
        /// If resource with given \p id is not stored inside ResourceManager, it will throw ResourceAccessException.
        /// \tparam R Type of Resource dependency to acquire.
        /// \param id ID of Resource to look up.
        /// \return pointer of Resource that managed by ResourceManager if exists; otherwise, throw ResourceAccessException.
        template<class R>
        R& Acquire(const std::string &id) const;

        /// Acquire resource dependency that match with given Resource Type and Resource ID.
        /// If resource with given \p id is not stored inside ResourceManager, it will attempt to load the resource dependency from a file of given path.
        /// \tparam R Type of Resource dependency to acquire.
        /// \param id ID of Resource to look up.
        /// \param path Path of Resource file to load when Resource with given \p id is not stored inside ResourceManager.
        /// \return pointer of Resource managed by ResourceManager.
        template<class R>
        R& Acquire(const std::string &id, const std::string &path) const;

        /// Acquire resource dependency that match with given Resource Type and Resource ID.
        /// If resource with given \p id is not stored inside ResourceManager, it will attempt to load the resource dependency from given memory.
        /// \tparam R Type of Resource dependency to acquire.
        /// \param id ID of Resource to look up.
        /// \param data Resource data to load when Resource with given \p id is not stored inside ResourceManager.
        /// \param dataSize Size of given Resource data.
        /// \return pointer of Resource managed by ResourceManager.
        template<class R>
        R& Acquire(const std::string &id, const void* data, std::size_t dataSize) const;

        /// Acquire resource dependency that match with given Resource Type and Resource ID.
        /// If resource with given \p id is not stored inside ResourceManager, it will attempt to load the resource dependency from given stream.
        /// \tparam R Type of Resource dependency to acquire.
        /// \param id ID of Resource to look up.
        /// \param stream Data stream that hold Resource data to load when Resource with given \p id is not stored inside ResourceManager.
        /// \return pointer of Resource managed by ResourceManager.
        template<class R>
        R& Acquire(const std::string &id, sf::InputStream &stream) const;

    private:
        const std::string m_id;
        mutable ResourceManager *m_resources;

        ResourceContext() noexcept;
    };
}

#include <Genode/IO/ResourceContext.inl>
#endif //GENODE_RESOURCE_CONTEXT_HPP
