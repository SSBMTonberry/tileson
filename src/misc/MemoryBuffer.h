//
// Created by Robin on 23.08.2019.
//

#ifndef TILESON_MEMORYBUFFER_H
#define TILESON_MEMORYBUFFER_H

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

#endif //TILESON_MEMORYBUFFER_H
