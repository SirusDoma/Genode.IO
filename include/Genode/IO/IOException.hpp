#ifndef GENODE_IO_EXCEPTION_HPP
#define GENODE_IO_EXCEPTION_HPP

#include <Genode/System/Exception.hpp>
#include <utility>

namespace Gx
{
    /// The exception that is thrown when an I/O error occurs.
    class IOException : public Exception
    {
    public:
        /// Initializes a new instance of IOException.
        /// \param message The IO exception message.
        explicit IOException(const std::string& message) : Exception(message) {};
    };

    /// The exception that is thrown when resource cannot be loaded.
    class ResourceLoadException : public IOException
    {
    public:
        /// Initializes a new instaoce of ResourceLoadException.
        /// \param message The IO exception message.
        explicit ResourceLoadException(const std::string &message) : IOException(message) {};
    };

    /// The exception that is thrown when failed to store resource into a container.
    class ResourceStoreException : public IOException
    {
    private:
        std::string m_resourceID;

    public:
        /// Initializes a new instance of ResourceStoreException.
        /// \param id The associated Resource ID that used in attempt to store the resource into a container.
        explicit ResourceStoreException(std::string id) : IOException("[" + id + "] Failed to store specified resource."), m_resourceID(std::move(id)) {};

        /// Initializes a new instance of ResourceStoreException.
        /// \param id The associated Resource ID that used in attempt to store the resource into a container.
        /// \param message The IO exception message.
        ResourceStoreException(std::string id, const std::string& message) : IOException(message), m_resourceID(std::move(id)) {};

        /// Gets the value of Resource ID that used in attempt to store the resource into a container.
        const std::string &GetResourceID() { return m_resourceID; }
    };

    /// The exception that is thrown when resource cannot be accessed.
    class ResourceAccessException : public IOException
    {
    private:
        std::string m_resourceID;

    public:
        /// Initializes a new instance of ResourceAccessException.
        /// \param id The associated Resource ID that used in attempt to access the resource from a container.
        explicit ResourceAccessException(std::string id) : IOException("[" + id + "] Resource with specified id is not found."), m_resourceID(std::move(id)) {};

        /// Initializes a new instance of ResourceAccessException.
        /// \param id The associated Resource ID that used in attempt to access the resource from a container.
        /// \param message The IO exception message.
        ResourceAccessException(std::string id, const std::string& message) : IOException(message), m_resourceID(std::move(id)) {};

        /// Gets the value of Resource ID that used in attempt to store the resource into a container.
        const std::string &GetResourceID() { return m_resourceID; }
    };
}

#endif //GENODE_IO_EXCEPTION_HPP
