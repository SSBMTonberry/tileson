//
// Created by robin on 01.08.2020.
//

#ifndef TILESON_PROJECT_HPP
#define TILESON_PROJECT_HPP

#include <fstream>
#include <sstream>
#include <memory>
#include "World.hpp"
#include "../objects/ProjectPropertyTypes.hpp"
#include "../objects/ProjectFolder.hpp"
#include "../objects/ProjectData.hpp"


namespace tson
{
    class Project
    {
        public:
            #ifdef JSON11_IS_DEFINED
            inline explicit Project(std::unique_ptr<tson::IJson> jsonParser = std::make_unique<tson::Json11>()) : m_json {std::move(jsonParser)}
            {

            }
            inline explicit Project(const fs::path &path, std::unique_ptr<tson::IJson> jsonParser = std::make_unique<tson::Json11>());
            #else
            inline explicit Project(std::unique_ptr<tson::IJson> jsonParser) : m_json {std::move(jsonParser)}
            {

            }
            inline explicit Project(const fs::path &path, std::unique_ptr<tson::IJson> jsonParser);
            #endif
            inline bool parse(const fs::path &path);
            inline void parse();

            [[nodiscard]] inline const ProjectData &getData() const;
            [[nodiscard]] inline const fs::path &getPath() const;
            [[nodiscard]] inline const std::vector<ProjectFolder> &getFolders() const;
            [[nodiscard]] inline tson::EnumDefinition* getEnumDefinition(std::string_view name);
            [[nodiscard]] inline tson::TiledClass* getClass(std::string_view name);


        private:
            inline void parseJson(IJson &json);
            fs::path m_path;
            std::vector<ProjectFolder> m_folders;
            ProjectData m_data;
            std::unique_ptr<IJson> m_json = nullptr;
    };

    Project::Project(const fs::path &path, std::unique_ptr<tson::IJson> jsonParser) : m_json {std::move(jsonParser)}
    {
        parse(path);
    }

    bool Project::parse(const fs::path &path)
    {
        m_path = path;
        std::ifstream i(m_path.generic_string());

        try
        {
            if(!m_json->parse(path))
                return false;
        }
        catch(const std::exception &error)
        {
            std::string message = "Parse error: ";
            message += std::string(error.what());
            message += std::string("\n");
            return false;
        }
        parseJson(*m_json);
        return true;
    }

    const ProjectData &Project::getData() const
    {
        return m_data;
    }

    void Project::parseJson(IJson &json)
    {
        m_data.basePath = (m_path.empty()) ? fs::path() : m_path.parent_path(); //The directory of the project file

        //Make sure these property types are read before any map is, so they can be resolved.
        if(json.count("propertyTypes") > 0)
        {
            m_data.projectPropertyTypes.parse(json, this);
        }

        if(json.count("automappingRulesFile") > 0) m_data.automappingRulesFile = json["automappingRulesFile"].get<std::string>();
        if(json.count("commands") > 0)
        {
            m_data.commands.clear();
            auto &commands = json.array("commands");
            std::for_each(commands.begin(), commands.end(), [&](std::unique_ptr<IJson> &item)
            {
                m_data.commands.emplace_back(item->get<std::string>());
            });
        }
        if(json.count("extensionsPath") > 0) m_data.extensionsPath = json["extensionsPath"].get<std::string>();
        if(json.count("folders") > 0)
        {
            m_data.folders.clear();
            m_data.folderPaths.clear();
            auto &folders = json.array("folders");
            std::for_each(folders.begin(), folders.end(), [&](std::unique_ptr<IJson> &item)
            {
                std::string folder = item->get<std::string>();
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

    tson::EnumDefinition *Project::getEnumDefinition(std::string_view name)
    {
        return m_data.projectPropertyTypes.getEnumDefinition(name);
    }

    tson::TiledClass *Project::getClass(std::string_view name)
    {
        return m_data.projectPropertyTypes.getClass(name);
    }

    /*!
     * Parses preloaded json data. Only used during tests involving project jsons not actually read from files
     * @return
     */
    void Project::parse()
    {
        parseJson(*m_json);
    }

}

#endif //TILESON_PROJECT_HPP
