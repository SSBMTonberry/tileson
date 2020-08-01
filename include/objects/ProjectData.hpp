//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_PROJECTDATA_HPP
#define TILESON_PROJECTDATA_HPP

namespace tson
{
    class ProjectData
    {
        public:
            std::string automappingRulesFile;
            std::vector<std::string> commands;
            std::string extensionsPath;
            std::vector<std::string> folders;

            //Tileson specific
            fs::path &basePath;
            std::vector<fs::path> folderPaths;
    };
}

#endif //TILESON_PROJECTDATA_HPP

#endif //DISABLE_CPP17_FILESYSTEM