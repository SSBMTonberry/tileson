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


        private:
            inline void loadData();
            fs::path                    m_path;
            bool                        m_hasWorldFile;
            std::vector<ProjectFolder>  m_subFolders;
            std::vector<ProjectFolder>  m_files;

    };

    ProjectFolder::ProjectFolder(const fs::path &path) : m_path {path}
    {
        loadData();
    }

    void ProjectFolder::loadData()
    {
        //RBP/TODO: Load data if folder exists
    }
}

#endif //TILESON_PROJECTFOLDER_HPP

#endif //DISABLE_CPP17_FILESYSTEM