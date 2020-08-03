//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_PROJECTFOLDER_HPP
#define TILESON_PROJECTFOLDER_HPP

namespace tson
{
    class ProjectFolder
    {
        public:
            inline ProjectFolder(const fs::path &path);

            inline const fs::path &getPath() const;
            inline bool hasWorldFile() const;
            inline const std::vector<ProjectFolder> &getSubFolders() const;
            inline const std::vector<fs::path> &getFiles() const;

        private:
            inline void loadData();
            fs::path                    m_path;
            bool                        m_hasWorldFile;
            tson::World                 m_world;
            std::vector<ProjectFolder>  m_subFolders;
            std::vector<fs::path>       m_files;

    };

    ProjectFolder::ProjectFolder(const fs::path &path) : m_path {path}
    {
        loadData();
    }

    void ProjectFolder::loadData()
    {
        //Search and see if there is a World file .world file
        fs::path worldPath;
        for (const auto & entry : fs::directory_iterator(m_path))
        {
            if(fs::is_regular_file(entry.path()))
            {
                if(entry.path().extension() == ".world")
                {
                    m_hasWorldFile = true;
                    worldPath = entry.path();
                }
            }
        }

        if(m_hasWorldFile)
            m_world.parse(worldPath);

        for (const auto & entry : fs::directory_iterator(m_path))
        {
            if (fs::is_directory(entry.path()))
                m_subFolders.emplace_back(entry.path()).loadData();
            else if (fs::is_regular_file(entry.path()))
            {
                if(m_hasWorldFile && m_world.contains(entry.path().filename().u8string()))
                    m_files.emplace_back(entry.path());
                else if(!m_hasWorldFile)
                    m_files.emplace_back(entry.path());
            }
        }

    }

    const fs::path &ProjectFolder::getPath() const
    {
        return m_path;
    }

    bool ProjectFolder::hasWorldFile() const
    {
        return m_hasWorldFile;
    }

    const std::vector<ProjectFolder> &ProjectFolder::getSubFolders() const
    {
        return m_subFolders;
    }

    const std::vector<fs::path> &ProjectFolder::getFiles() const
    {
        return m_files;
    }
}

#endif //TILESON_PROJECTFOLDER_HPP

#endif //DISABLE_CPP17_FILESYSTEM