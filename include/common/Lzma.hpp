//
// Created by robin on 16.01.2021.
//
//#include "../../extras/pocketlzma.hpp"
#ifdef POCKETLZMA_POCKETLZMA_H

#ifndef TILESON_LZMA_HPP
#define TILESON_LZMA_HPP

namespace tson
{
    class Lzma : public IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>
    {
        public:
            inline const std::string &name() const override
            {
                return NAME;
            }

            inline std::vector<uint8_t> decompress(const std::vector<uint8_t> &input) override
            {
                std::vector<uint8_t> out;

                plz::PocketLzma p;
                plz::StatusCode status = p.decompress(input, out);

                if(status != plz::StatusCode::Ok)
                    return std::vector<uint8_t>();

                return out;
            }

            inline std::vector<uint8_t> decompressFile(const fs::path &path) override
            {
                std::vector<uint8_t> in;
                std::vector<uint8_t> out;

                plz::PocketLzma p;
                plz::FileStatus fileStatus = plz::File::FromFile(path.u8string(), in);
                if(fileStatus.status() != plz::FileStatus::Code::Ok)
                    return std::vector<uint8_t>();

                plz::StatusCode status = p.decompress(in, out);

                if(status != plz::StatusCode::Ok)
                    return std::vector<uint8_t>();

                return out;
            }

            inline std::vector<uint8_t> decompress(const void *data, size_t size) override
            {
                std::vector<uint8_t> out;

                plz::PocketLzma p;
                plz::StatusCode status = p.decompress((uint8_t*) data, size, out);

                if(status != plz::StatusCode::Ok)
                    return std::vector<uint8_t>();

                return out;
            }

        private:
            inline static const std::string NAME {"lzma"};
    };
}

#endif //TILESON_LZMA_HPP

#endif