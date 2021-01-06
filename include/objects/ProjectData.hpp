//
// Created by robin on 01.08.2020.
//

#ifndef TILESON_PROJECTDATA_HPP
#define TILESON_PROJECTDATA_HPP

namespace tson
{
    class ProjectData
    {
        public:
            ProjectData() = default;
            std::string automappingRulesFile;
            std::vector<std::string> commands;
            std::string extensionsPath;
            std::vector<std::string> folders;
            std::string objectTypesFile;

            //Tileson specific
            fs::path basePath;
            std::vector<tson::ProjectFolder> folderPaths;
    };
}

#endif //TILESON_PROJECTDATA_HPP