//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_TILESON_H
#define TILESON_TILESON_H

#include "tiled/Map.h"
#include <fstream>

#if MSVC
#include <filesystem>
    namespace fs = std::filesystem;
#elif MINGW
#if __MINGW64_VERSION_MAJOR > 6
        #include <filesystem>
        namespace fs = std::filesystem;
    #else
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #endif
#elif APPLE
#include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #if __GNUC__ < 8 //GCC major version less than 8
#include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #else
        #include <filesystem>
namespace fs = std::filesystem;
    #endif
#endif

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
