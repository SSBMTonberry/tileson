//
// Created by robin on 01.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM

#ifndef TILESON_PROJECT_HPP
#define TILESON_PROJECT_HPP

#include "../objects/ProjectData.hpp"
namespace tson
{
    class Project
    {
        public:
            inline Project() = default;
            inline bool parse(const fs::path &path);

        private:

    };

    bool Project::parse(const fs::path &path)
    {
        return false;
    }
}

#endif //TILESON_PROJECT_HPP

#endif //DISABLE_CPP17_FILESYSTEM
