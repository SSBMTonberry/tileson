//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_WORLD_HPP
#define TILESON_WORLD_HPP

#include "../objects/WorldMapData.hpp"
namespace tson
{
    class World
    {
        public:
            inline explicit World(const fs::path &path);

        private:
            fs::path m_path;
            std::vector<WorldMapData> m_maps;
            bool m_onlyShowAdjacentMaps;
            std::string m_type;
    };

    World::World(const fs::path &path) : m_path {path}
    {
        //RBP/TODO: Load data
    }
}

#endif //TILESON_WORLD_HPP

#endif //DISABLE_CPP17_FILESYSTEM