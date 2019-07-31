//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_TILESONPARSER_H
#define TILESON_TILESONPARSER_H

#include "json.hpp"

namespace tson
{
    enum class ParserStatus : uint8_t
    {
        Undefined = 0,
        Success = 1
    };

    class TilesonParser
    {
        public:
            TilesonParser() = default;

            ParserStatus getStatus() const;

        private:
            nlohmann::json m_json;

            ParserStatus m_status = ParserStatus::Undefined;
    };
}

#endif //TILESON_TILESONPARSER_H
