namespace Gx
{
    template<class FS>
    void FileSystem::Mount()
    {
        static_assert(std::is_base_of<IFileSystem, FS>::value, "Template parameter must inherit Gx::IFileSystem");
        m_systems.insert(std::make_unique<FS>());
    }

    template<class FS>
    void FileSystem::Mount(std::unique_ptr<FS> fs)
    {
        static_assert(std::is_base_of<IFileSystem, FS>::value, "Template parameter must inherit Gx::IFileSystem");
        EnsureDefaultFileSystemRegistered();

        m_systems.insert(std::move(fs));
    }

    template<class FS>
    bool FileSystem::Dismount()
    {
        static_assert(std::is_base_of<IFileSystem, FS>::value, "Template parameter must inherit Gx::IFileSystem");
        EnsureDefaultFileSystemRegistered();

        std::size_t count = m_systems.size();
        for (auto it = m_systems.begin(); it != m_systems.end(); it++)
        {
            if (typeid(*it->get()) == typeid(FS))
                m_systems.erase(it--);
        }

        return m_systems.size() != count;
    }

    template<class FS>
    bool FileSystem::Dismount(FS *fs)
    {
        static_assert(std::is_base_of<IFileSystem, FS>::value, "Template parameter must inherit Gx::IFileSystem");
        EnsureDefaultFileSystemRegistered();

        for (auto it = m_systems.begin(); it != m_systems.end(); it++)
        {
            if (it->get() == fs)
            {
                m_systems.erase(it--);
                return true;
            }
        }

        return false;
    }
}