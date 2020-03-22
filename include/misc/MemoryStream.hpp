//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_MEMORYSTREAM_HPP
#define TILESON_MEMORYSTREAM_HPP

#include "MemoryBuffer.hpp"

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

#endif //TILESON_MEMORYSTREAM_HPP
