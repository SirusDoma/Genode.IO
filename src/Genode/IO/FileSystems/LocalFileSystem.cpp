#include <Genode/IO/FileSystems/LocalFileSystem.hpp>

#include <filesystem>

#include <SFML/System/FileInputStream.hpp>

namespace Gx
{
    LocalFileSystem::LocalFileSystem() :
        m_root()
    {
    }

    LocalFileSystem::LocalFileSystem(const std::string &root) :
        m_root(root)
    {
    }

    bool LocalFileSystem::IsExists(const std::string &fileName) const
    {
        return std::filesystem::exists(GetFullName(fileName));
    }

    std::string LocalFileSystem::GetFullName(const std::string &fileName) const
    {
        auto fullName = std::filesystem::path(fileName);
        if (!m_root.empty())
            fullName = m_root / fullName;

        return fullName.string();
    }

    std::unique_ptr<sf::InputStream> LocalFileSystem::Open(const std::string &fileName)
    {
        auto stream = std::make_unique<sf::FileInputStream>();
        if (!stream->open(GetFullName(fileName)))
            return nullptr;

        return stream;
    }

    size_t LocalFileSystem::Read(const std::string &fileName, void *data)
    {
        return Read(fileName, data, 0);
    }

    std::size_t LocalFileSystem::Read(const std::string &fileName, void *data, std::size_t size)
    {
        auto stream = sf::FileInputStream();
        if (!stream.open(GetFullName(fileName)))
            return -1;

        if (size <= 0)
        {
            size = stream.getSize();
            if (size <= 0)
                return size;
        }

        return stream.read(data, size);
    }

    std::size_t LocalFileSystem::GetFileSize(const std::string &fileName)
    {
        auto stream = sf::FileInputStream();
        if (!stream.open(GetFullName(fileName)))
            return -1;

        return stream.getSize();
    }
}