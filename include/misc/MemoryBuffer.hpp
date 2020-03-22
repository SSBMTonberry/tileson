//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_MEMORYBUFFER_HPP
#define TILESON_MEMORYBUFFER_HPP

#include <iostream>

namespace tson
{
    class MemoryBuffer : public std::basic_streambuf<char> {
        public:
            MemoryBuffer(const uint8_t *p, size_t l) {
                setg((char*)p, (char*)p, (char*)p + l);
            }
    };
}

#endif //TILESON_MEMORYBUFFER_HPP
