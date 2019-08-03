//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_TERRAIN_H
#define TILESON_TERRAIN_H

#include <json.hpp>

namespace tson
{
    class Terrain
    {
        public:
            Terrain() = default;
            Terrain(std::string name, int tile);
            explicit Terrain(const nlohmann::json &json);

            bool parse(const nlohmann::json &json);

            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] int getTile() const;

        private:
            std::string   m_name;      /*! 'name': Name of terrain */
            int           m_tile {};   /*! 'tile': Local ID of tile representing terrain */
    };
}

#endif //TILESON_TERRAIN_H
