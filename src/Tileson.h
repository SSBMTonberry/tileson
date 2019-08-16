//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_TILESON_H
#define TILESON_TILESON_H

#include "tiled/Map.h"
#include <fstream>

namespace tson
{
    class Tileson
    {
        public:
            Tileson() = default;

            tson::Map parse(const fs::path &path);
            tson::Map parse(const void * data, size_t size);

        private:
            tson::Map parseJson(const nlohmann::json &json);
    };
}

#endif //TILESON_TILESON_H
