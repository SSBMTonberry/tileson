//
// Created by Robin on 23.08.2019.
//

#ifndef TILESON_MEMORYSTREAM_H
#define TILESON_MEMORYSTREAM_H

#include "MemoryBuffer.h"

namespace tson
{
    class MemoryStream : public std::istream {
        public:
            MemoryStream(const uint8_t *p, size_t l) :
                    std::istream(&m_buffer),
                    m_buffer(p, l) {
                rdbuf(&m_buffer);
            }

        private:
            MemoryBuffer m_buffer;
    };
}

#endif //TILESON_MEMORYSTREAM_H
