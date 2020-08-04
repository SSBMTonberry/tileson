//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_PROJECT_HPP
#define TILESON_PROJECT_HPP

#include <fstream>
#include <sstream>
#include <memory>
#include "World.hpp"
#include "../objects/ProjectFolder.hpp"
#include "../objects/ProjectData.hpp"

namespace tson
{
    class Project
    {
        public:
            inline Project() = default;
            inline explicit Project(const fs::path &path);
            inline bool parse(const fs::path &path);

            [[nodiscard]] inline const ProjectData &getData() const;
            [[nodiscard]] inline const fs::path &getPath() const;
            [[nodiscard]] inline const std::vector<ProjectFolder> &getFolders() const;

        private:
            inline void parseJson(const nlohmann::json &json);
            fs::path m_path;
            std::vector<ProjectFolder> m_folders;
            ProjectData m_data;
    };

    Project::Project(const fs::path &path)
    {
        parse(path);
    }

    bool Project::parse(const fs::path &path)
    {
        m_path = path;
        std::ifstream i(m_path.u8string());
        nlohmann::json json;
        try
        {
            i >> json;
        }
        catch(const nlohmann::json::parse_error &error)
        {
            std::string message = "Parse error: ";
            message += std::string(error.what());
            message += std::string("\n");
            return false;
        }
        parseJson(json);
        return true;
    }

    const ProjectData &Project::getData() const
    {
        return m_data;
    }

    void Project::parseJson(const nlohmann::json &json)
    {
        m_data.basePath = m_path.parent_path(); //The directory of the project file

        if(json.count("automappingRulesFile") > 0) m_data.automappingRulesFile = json["automappingRulesFile"].get<std::string>();
        if(json.count("commands") > 0)
        {
            m_data.commands.clear();
            std::for_each(json["commands"].begin(), json["commands"].end(), [&](const nlohmann::json &item)
            {
                m_data.commands.emplace_back(item.get<std::string>());
            });
        }
        if(json.count("extensionsPath") > 0) m_data.extensionsPath = json["extensionsPath"].get<std::string>();
        if(json.count("folders") > 0)
        {
            m_data.folders.clear();
            m_data.folderPaths.clear();
            std::for_each(json["folders"].begin(), json["folders"].end(), [&](const nlohmann::json &item)
            {
                std::string folder = item.get<std::string>();
                m_data.folders.emplace_back(folder);
                m_data.folderPaths.emplace_back(m_data.basePath / folder);
                m_folders.emplace_back(m_data.basePath / folder);
            });
        }
        if(json.count("objectTypesFile") > 0) m_data.objectTypesFile = json["objectTypesFile"].get<std::string>();

    }

    const fs::path &Project::getPath() const
    {
        return m_path;
    }

    const std::vector<ProjectFolder> &Project::getFolders() const
    {
        return m_folders;
    }


}

#endif //TILESON_PROJECT_HPP

#endif //DISABLE_CPP17_FILESYSTEM
