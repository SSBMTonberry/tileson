//
// Created by robin on 29.07.2020.
//

#ifndef TILESON_IDECOMPRESSOR_H
#define TILESON_IDECOMPRESSOR_H

#include <string_view>

namespace tson
{
    class IDecompressor
    {
        public:
            /*!
             * If the name matches with 'compression' or 'encoding' the decompress() function will
             * be called automatically for the actual Layer. Encoding-related matching is handled first!
             *
             * Known values:
             *
             * compression: zlib, gzip, zstd (since Tiled 1.3) or empty (default) (tilelayer only).
             * encoding: csv (default) or base64 (tilelayer only).
             *
             * @return
             */
            virtual const std::string &name() const = 0;

            virtual std::string decompress(std::string_view s) = 0;
    };
}

#endif //TILESON_IDECOMPRESSOR_H
