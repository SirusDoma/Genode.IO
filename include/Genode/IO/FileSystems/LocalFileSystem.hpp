#ifndef GENODE_LOCAL_FILESYSTEM_HPP
#define GENODE_LOCAL_FILESYSTEM_HPP

#include <filesystem>

#include <Genode/IO/FileSystem.hpp>

namespace Gx
{
    class LocalFileSystem : public IFileSystem
    {
    public:
        LocalFileSystem();
        explicit LocalFileSystem(const std::string &root);
        ~LocalFileSystem() override = default;

        bool IsExists(const std::string &fileName) const override;
        std::string GetFullName(const std::string &fileName) const;

        std::unique_ptr<sf::InputStream> Open(const std::string &fileName) override;
        std::size_t Read(const std::string &fileName, void *data) override;
        std::size_t Read(const std::string &fileName, void *data, std::size_t size) override;
        std::size_t GetFileSize(const std::string &fileName) override;

    private:
        std::filesystem::path m_root;
    };


}

#endif //GENODE_LOCAL_FILESYSTEM_HPP
