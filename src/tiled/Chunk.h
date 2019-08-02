//
// Created by robin on 02.08.2019.
//

#ifndef TILESON_CHUNK_H
#define TILESON_CHUNK_H

#include <json.hpp>
#include "../objects/Vector2.hpp"

namespace tson
{
    class Chunk
    {
        public:
            Chunk() = default;
            explicit Chunk(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] const std::vector<int> &getData() const;
            [[nodiscard]] const std::string &getBase64Data() const;
            [[nodiscard]] const Vector2i &getSize() const;
            [[nodiscard]] const Vector2i &getPosition() const;

        private:
            std::vector<int> m_data;        /*! 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only. */
            std::string      m_base64Data;  /*! 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data. */
            tson::Vector2i   m_size;        /*!  x='width' (in tiles) and y='height' (in tiles): */
            tson::Vector2i   m_position;    /*! 'x' and 'y' position in tiles */
    };
}

#endif //TILESON_CHUNK_H
