//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_WORLDMAPDATA_HPP
#define TILESON_WORLDMAPDATA_HPP

namespace tson
{
    class WorldMapData
    {
        public:
            std::string fileName;
            tson::Vector2i size;
            tson::Vector2i position;
    };
}

#endif //TILESON_WORLDMAPDATA_HPP

#endif //DISABLE_CPP17_FILESYSTEM