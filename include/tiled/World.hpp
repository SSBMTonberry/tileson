//
// Created by robin on 01.08.2020.
//

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
            #ifdef JSON11_IS_DEFINED
            inline explicit World(std::unique_ptr<tson::IJson> jsonParser = std::make_unique<tson::Json11>()) : m_json {std::move(jsonParser)}
            {
            }

            inline explicit World(const fs::path &path, std::unique_ptr<tson::IJson> jsonParser = std::make_unique<tson::Json11>());
            #else
            inline explicit World(std::unique_ptr<tson::IJson> jsonParser) : m_json {std::move(jsonParser)}
            {
            }

            inline explicit World(const fs::path &path, std::unique_ptr<tson::IJson> jsonParser);
            #endif
            inline bool parse(const fs::path &path);
            inline std::size_t loadMaps(tson::Tileson *parser); //tileson_forward.hpp
            inline bool contains(std::string_view filename);
            inline const WorldMapData *get(std::string_view filename) const;

            [[nodiscard]] inline const fs::path &getPath() const;
            [[nodiscard]] inline const fs::path &getFolder() const;
            [[nodiscard]] inline const std::vector<WorldMapData> &getMapData() const;
            [[nodiscard]] inline bool onlyShowAdjacentMaps() const;
            [[nodiscard]] inline const std::string &getType() const;
            [[nodiscard]] inline const std::vector<std::unique_ptr<tson::Map>> &getMaps() const;

        private:
            inline void parseJson(IJson &json);

            std::unique_ptr<IJson> m_json = nullptr;
            fs::path m_path;
            fs::path m_folder;
            std::vector<WorldMapData> m_mapData;
            std::vector<std::unique_ptr<tson::Map>> m_maps;
            bool m_onlyShowAdjacentMaps;
            std::string m_type;
    };

    World::World(const fs::path &path, std::unique_ptr<tson::IJson> jsonParser) : m_json {std::move(jsonParser)}
    {
        parse(path);
    }

    bool World::parse(const fs::path &path)
    {
        m_path = path;
        m_folder = m_path.parent_path();

        if(!m_json->parse(path))
            return false;

        parseJson(*m_json);
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

    void World::parseJson(IJson &json)
    {
        if(json.count("onlyShowAdjacentMaps") > 0) m_onlyShowAdjacentMaps = json["onlyShowAdjacentMaps"].get<bool>();
        if(json.count("type") > 0) m_type = json["type"].get<std::string>();

        if(json["maps"].isArray())
        {
            auto &maps = json.array("maps");
            std::for_each(maps.begin(), maps.end(), [&](std::unique_ptr<IJson> &item) { m_mapData.emplace_back(m_folder, *item); });
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

    /*!
     * Get a map by its filename
     * @param filename Filename (including extension) - (example: file.json)
     * @return pointer to WorldMapData or nullptr if not exists
     */
    const WorldMapData * World::get(std::string_view filename) const
    {
        auto iter = std::find_if(m_mapData.begin(), m_mapData.end(), [&](const auto &item) { return item.fileName == filename; });
        return (iter == m_mapData.end()) ? nullptr : iter.operator->();
    }

    /*!
     * Get all maps that have been loaded by loadMaps().
     * NOTE: This is untested, and was a last second addition to Tileson 1.2.0, as I had forgot about the loadMaps() functionality (also untested)
     * If you find anything malfunctioning - please report.
     * @return All maps loaded by loadMaps()
     */
    const std::vector<std::unique_ptr<tson::Map>> &World::getMaps() const
    {
        return m_maps;
    }

}

#endif //TILESON_WORLD_HPP
