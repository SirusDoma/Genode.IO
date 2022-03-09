#include <Genode/IO/FileSystem.hpp>
#include <Genode/IO/FileSystems/LocalFileSystem.hpp>

namespace Gx
{
    void FileSystem::EnsureDefaultFileSystemRegistered()
    {
        static bool registered = false;
        if (!registered)
        {
            Mount<Gx::LocalFileSystem>();
            registered = true;
        }
    }

    std::vector<IFileSystem*> FileSystem::GetFileSystems()
    {
        EnsureDefaultFileSystemRegistered();

        auto systems = std::vector<IFileSystem*>(m_systems.size());
        for (auto &fs : m_systems)
            systems.push_back(fs.get());

        return systems;
    }

    bool FileSystem::IsExists(const std::string &fileName)
    {
        EnsureDefaultFileSystemRegistered();

        return std::any_of(m_systems.begin(), m_systems.end(), [&] (auto &fs) { return fs->IsExists(fileName); });
    }

    std::unique_ptr<sf::InputStream> FileSystem::Open(const std::string &fileName)
    {
        for (auto &fs : m_systems)
        {
            if (fs->IsExists(fileName))
                return fs->Open(fileName);
        }

        return nullptr;
    }

    std::size_t FileSystem::Read(const std::string &fileName, void *data, std::size_t size)
    {
        for (auto &fs : m_systems)
        {
            if (fs->IsExists(fileName))
                return fs->Read(fileName, data, size);
        }

        return -1;
    }

    std::size_t FileSystem::GetFileSize(const std::string &fileName)
    {
        for (auto &fs : m_systems)
        {
            if (fs->IsExists(fileName))
                return fs->GetFileSize(fileName);
        }

        return -1;
    }

    std::string FileSystem::GetFullName(const std::string &fileName)
    {
        for (auto &fs : m_systems)
        {
            auto local = dynamic_cast<LocalFileSystem*>(fs.get());
            if (!local)
                continue;

            if (local->IsExists(fileName))
                return local->GetFullName(fileName);
        }

        return {};
    }
}
