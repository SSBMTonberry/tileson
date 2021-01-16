//
// Created by robin on 29.07.2020.
//

#ifndef TILESON_IDECOMPRESSOR_HPP
#define TILESON_IDECOMPRESSOR_HPP

#include <string_view>

namespace tson
{
    template <class TIn, class TOut>
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
            [[nodiscard]] virtual const std::string &name() const = 0;

            /*!
             * Used primarily for Tiled related decompression.
             * @param input Input data
             * @return Decompressed data
             */
            virtual TOut decompress(const TIn &input) = 0;

            /*!
             * Used for whole file decompression. Not related to Tiled
             * @param path
             * @return
             */
            virtual TOut decompressFile(const fs::path &path) = 0;

            /*!
             * Used for whole file decompression. Not related to Tiled
             * @param path
             * @return
             */
            virtual TOut decompress(const void *data, size_t size) = 0;
    };
}

#endif //TILESON_IDECOMPRESSOR_HPP
