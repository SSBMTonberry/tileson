//
// Created by robin on 03.12.22.
//

#ifndef TILESON_TESTTOOLS_HPP
#define TILESON_TESTTOOLS_HPP

#ifdef TILESON_UNIT_TEST_USE_SINGLE_HEADER
    #include "../tileson.hpp"
#else
    #include "../include/tileson.h"
#endif

/*!
 * Will return <base path of executable> / <input parameter>
 * Some OS-es may require different methods to get the proper base path.
 * @param p
 * @return
 */
inline static fs::path GetPathWithBase(const fs::path &p)
{
    fs::path path = fs::current_path() / p;

    //A stupid hack to make MSVC work when using GitHub Actions. Not a problem when done on a local system...
    if(!fs::exists(path))
        return fs::current_path() / fs::path("tests") / p;

    return path;
}

#endif //TILESON_TESTTOOLS_HPP
