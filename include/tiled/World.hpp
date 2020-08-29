//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_WORLD_HPP
#define TILESON_WORLD_HPP

#include "../objects/WorldMapData.hpp"
#include <memory>
namespace tson
{
    class Tileson;
    class World
    {
        public:
            inline World() = default;
            inline explicit World(const fs::path &path);
            inline bool parse(const fs::path &path);
            inline int loadMaps(tson::Tileson *parser); //tileson_forward.hpp
            inline bool contains(std::string_view filename);
            inline const WorldMapData *get(std::string_view filename) const;

            [[nodiscard]] inline const fs::path &getPath() const;
            [[nodiscard]] inline const fs::path &getFolder() const;
            [[nodiscard]] inline const std::vector<WorldMapData> &getMapData() const;
            [[nodiscard]] inline bool onlyShowAdjacentMaps() const;
            [[nodiscard]] inline const std::string &getType() const;

        private:
            inline void parseJson(const nlohmann::json &json);

            fs::path m_path;
            fs::path m_folder;
            std::vector<WorldMapData> m_mapData;
            std::vector<std::unique_ptr<tson::Map>> m_maps;
            bool m_onlyShowAdjacentMaps;
            std::string m_type;
    };

    World::World(const fs::path &path)
    {
        parse(path);
    }

    bool World::parse(const fs::path &path)
    {
        m_path = path;
        m_folder = m_path.parent_path();
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

    const fs::path &World::getPath() const
    {
        return m_path;
    }

    const std::vector<WorldMapData> &World::getMapData() const
    {
        return m_mapData;
    }

    bool World::onlyShowAdjacentMaps() const
    {
        return m_onlyShowAdjacentMaps;
    }

    const std::string &World::getType() const
    {
        return m_type;
    }

    void World::parseJson(const nlohmann::json &json)
    {
        if(json.count("onlyShowAdjacentMaps") > 0) m_onlyShowAdjacentMaps = json["onlyShowAdjacentMaps"].get<bool>();
        if(json.count("type") > 0) m_type = json["type"].get<std::string>();

        if(json["maps"].is_array())
        {
            std::for_each(json["maps"].begin(), json["maps"].end(), [&](const nlohmann::json &item) { m_mapData.emplace_back(m_folder, item); });
        }
    }

    const fs::path &World::getFolder() const
    {
        return m_folder;
    }

    /*!
     * Check if there is WorldMapData in the world that contains the current filename.
     * Filename = <file>.<extension>
     * @param filename
     * @return
     */
    bool World::contains(std::string_view filename)
    {
        //Note: might be moved to std::ranges from C++20.
        return std::any_of(m_mapData.begin(), m_mapData.end(), [&](const auto &item) { return item.fileName == filename; });
    }

    const WorldMapData * World::get(std::string_view filename) const
    {
        auto iter = std::find_if(m_mapData.begin(), m_mapData.end(), [&](const auto &item) { return item.fileName == filename; });
        return (iter == m_mapData.end()) ? nullptr : iter.operator->();
    }

}

#endif //TILESON_WORLD_HPP

#endif //DISABLE_CPP17_FILESYSTEM