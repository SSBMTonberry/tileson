//
// Created by robin on 01.08.22.
//

#ifndef TILESON_PROJECTPROPERTYTYPES_HPP
#define TILESON_PROJECTPROPERTYTYPES_HPP

namespace tson
{
    class ProjectPropertyTypes
    {
        public:
            inline ProjectPropertyTypes() = default;
            inline bool parse(IJson &json, tson::Project *project);

            inline const std::vector<tson::EnumDefinition> &getEnums() const;
            inline const std::vector<tson::TiledClass> &getClasses() const;
            [[nodiscard]] inline tson::EnumDefinition* getEnumDefinition(std::string_view name);
            [[nodiscard]] inline tson::TiledClass* getClass(std::string_view name);
            inline bool isUnhandledContentFound() const;

        private:
            std::vector<tson::EnumDefinition> m_enums;
            std::vector<tson::TiledClass> m_classes;
            bool m_unhandledContentFound {false};

    };

    bool ProjectPropertyTypes::parse(IJson &json, tson::Project *project)
    {
        m_enums.clear();
        m_classes.clear();
        m_unhandledContentFound = false;

        if(json.count("propertyTypes") > 0 && json["propertyTypes"].isArray())
        {
            auto &array = json.array("propertyTypes");
            std::vector<tson::IJson*> classes; //Classes must be handled after enums
            std::vector<tson::IJson*> other; //Unhandled stuff - just to keep track if something is missing...
            std::for_each(array.begin(), array.end(), [&](std::unique_ptr<IJson> &item)
            {
                IJson &j = *item;
                if(j.count("type") > 0)
                {
                    std::string t = j["type"].get<std::string>();
                    if(t == "enum")
                    {
                        m_enums.emplace_back(j); //Can be resolved directly
                    }
                    else if(t == "class")
                    {
                        classes.push_back(item.get()); //Must be resolved later
                    }
                    else
                        other.push_back(item.get()); //Only used to set flag for whether unhandled content was found.
                }
            });

            std::for_each(classes.begin(), classes.end(), [&](IJson *item)
            {
                m_classes.emplace_back(*item, project);
            });

            if(!other.empty())
                m_unhandledContentFound = true;

        }
        return false;
    }

    const std::vector<tson::EnumDefinition> &ProjectPropertyTypes::getEnums() const
    {
        return m_enums;
    }

    const std::vector<tson::TiledClass> &ProjectPropertyTypes::getClasses() const
    {
        return m_classes;
    }

    bool ProjectPropertyTypes::isUnhandledContentFound() const
    {
        return m_unhandledContentFound;
    }

    tson::EnumDefinition *ProjectPropertyTypes::getEnumDefinition(std::string_view name)
    {
        auto it = std::find_if(m_enums.begin(), m_enums.end(), [&](const EnumDefinition &def)
        {
            return def.getName() == name;
        });

        if(it != m_enums.end())
            return &it.operator*();

        return nullptr;
    }

    tson::TiledClass *ProjectPropertyTypes::getClass(std::string_view name)
    {
        auto it = std::find_if(m_classes.begin(), m_classes.end(), [&](const TiledClass &def)
        {
            return def.getName() == name;
        });

        if(it != m_classes.end())
            return &it.operator*();

        return nullptr;
    }
}

#endif //TILESON_PROJECTPROPERTYTYPES_HPP
