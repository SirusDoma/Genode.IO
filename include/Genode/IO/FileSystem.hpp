#ifndef GENODE_FILESYSTEM_HPP
#define GENODE_FILESYSTEM_HPP

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>

#include <SFML/System/InputStream.hpp>

namespace Gx
{
    class LocalFileSystem;

    /// Represents an interface that provides FileSystem functionalities.
    class IFileSystem
    {
    public:
        /// Releases the resources that used IFileSystem.
        virtual ~IFileSystem() = default;

        /// Check whether the given \p fileName is exists within the current implementation of IFileSystem.
        /// \param fileName The fileName to check.
        /// \return true if the file is exists; otherwise, false.
        virtual bool IsExists(const std::string &fileName) const = 0;

        /// Open the file as a SFML InputStream.
        /// \param fileName The fileName to open as a stream.
        /// \return Pointer to SFML InputStream if success; otherwise, return nullptr.
        virtual std::unique_ptr<sf::InputStream> Open(const std::string &fileName) = 0;

        /// Read the file and copy its content into a buffer.
        /// \param fileName The fileName to read.
        /// \param data The buffer that will hold the file content.
        /// \return The number of bytes that copied from file content into the buffer if success; otherwise, -1.
        virtual std::size_t Read(const std::string &fileName, void *data) = 0;

        /// Read the file and copy its content into a buffer.
        /// \param fileName The fileName to read.
        /// \param data The buffer that will hold the file content.
        /// \param size The number of bytes that wish to be copied, 0 to read whole content.
        /// \return The number of bytes that copied from file content into the buffer if success; otherwise, -1.
        virtual std::size_t Read(const std::string &fileName, void *data, std::size_t size) = 0;

        /// Gets the file size of given \p fileName.
        /// \param fileName The fileName to check.
        /// \return File size of file that match with given fileName if success; otherwise, -1.
        virtual std::size_t GetFileSize(const std::string &fileName) = 0;
    };

    /// Represents virtual FileSystem.
    class FileSystem
    {
    public:
        /// Gets mounted FileSystem.
        /// \return A vector containing pointers to mounted FileSystems.
        static std::vector<IFileSystem*> GetFileSystems();

        /// Check whether the given \p fileName is exists within one of mounted FileSystem.
        /// \param fileName The fileName to check.
        /// \return true if the file is exists; otherwise, false.
        static bool IsExists(const std::string &fileName);

        /// Open the file as a SFML InputStream.
        /// \param fileName The fileName to open as a stream.
        /// \return Pointer to SFML InputStream if success; otherwise, return nullptr.
        static std::unique_ptr<sf::InputStream> Open(const std::string &fileName);

        /// Read the file and copy its content into a buffer.
        /// \param fileName The fileName to read.
        /// \param data The buffer that will hold the file content.
        /// \param size The number of bytes that wish to be copied, 0 to read whole content.
        /// \return The number of bytes that copied from file content into the buffer if success; otherwise, -1.
        static std::size_t Read(const std::string &fileName, void *data, std::size_t size = 0);

        /// Gets the file size of given \p fileName.
        /// \param fileName The fileName to check.
        /// \return File size of file that match with given fileName if success; otherwise, -1.
        static std::size_t GetFileSize(const std::string &fileName);

        /// Resolve the full name of given \p fileName by using one of mounted LocalFileSystem.
        /// \param fileName The fileName to get as full path.
        /// \return Full path of given fileName that exists within one of mounted LocalFileSystem.
        static std::string GetFullName(const std::string &fileName);

        /// Initialize and mount given type of FileSystem.
        /// \tparam FS FileSystem type to mount.
        template<class FS>
        static void Mount();

        /// Mount given instance of FileSystem.
        /// \tparam FS FileSystem type to mount.
        /// \param fs An instance of FileSystem to mount.
        template<class FS>
        static void Mount(std::unique_ptr<FS> fs);

        /// Dismount particular type of FileSystem
        /// \tparam FS FileSystem type to dismount.
        /// \return true if there's at least one FileSystem that dismounted; otherwise, false.
        template<class FS>
        static bool Dismount();

        /// Dismount particular type of FileSystem
        /// \tparam FS FileSystem type to dismount.
        /// \return true if there's at least one FileSystem that dismounted; otherwise, false.
        template<class FS>
        static bool Dismount(FS *fs);

    private:
        using FileSystemContainer = std::unordered_set<std::unique_ptr<IFileSystem>>;

        static void EnsureDefaultFileSystemRegistered();

        inline static FileSystemContainer m_systems;
    };
}

#include <Genode/IO/FileSystem.inl>
#endif //GENODE_FILESYSTEM_HPP
