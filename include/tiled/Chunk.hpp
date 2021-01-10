//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_CHUNK_HPP
#define TILESON_CHUNK_HPP

//#include "../external/json.hpp"
#include "../objects/Vector2.hpp"

namespace tson
{
    class Chunk
    {
        public:
            inline Chunk() = default;
            inline explicit Chunk(IJson &json);
            inline bool parse(IJson &json);

            [[nodiscard]] inline const std::vector<int> &getData() const;
            [[nodiscard]] inline const std::string &getBase64Data() const;
            [[nodiscard]] inline const Vector2i &getSize() const;
            [[nodiscard]] inline const Vector2i &getPosition() const;

        private:
            std::vector<int> m_data;        /*! 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only. */
            std::string      m_base64Data;  /*! 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data. */
            tson::Vector2i   m_size;        /*!  x='width' (in tiles) and y='height' (in tiles): */
            tson::Vector2i   m_position;    /*! 'x' and 'y' position in tiles */
    };
}

#endif //TILESON_CHUNK_HPP

/*!
 * Parses 'chunk' data from Tiled json and stores the values in this class
 * @param json json-data
 */
tson::Chunk::Chunk(IJson &json)
{
    parse(json);
}

/*!
 * Parses 'chunk' data from Tiled json and stores the values in this class
 * @param json json-data
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Chunk::parse(IJson &json)
{
    bool allFound = true;

    if(json.count("width") > 0 && json.count("height") > 0)
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
    if(json.count("x") > 0 && json.count("y") > 0)
        m_position = {json["x"].get<int>(), json["y"].get<int>()}; else allFound = false;

    //Handle DATA (Optional)
    if(json.count("data") > 0)
    {
        if(json["data"].isArray())
        {
            auto &data = json.array("data");
            std::for_each(data.begin(), data.end(), [&](std::unique_ptr<IJson> &item) { m_data.push_back(item->get<int>()); });
        }
        else
            m_base64Data = json["data"].get<std::string>();
    }

    return allFound;
}

/*!
 * 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return list of tile ids
 */
const std::vector<int> &tson::Chunk::getData() const
{
    return m_data;
}

/*!
 * 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data.
 * @return base64 string
 */
const std::string &tson::Chunk::getBase64Data() const
{
    return m_base64Data;
}

/*!
 * x='width' (in tiles) and y='height' (in tiles).
 * @return Size (x and y), containing the values from the fields 'width' and 'height' in Tiled
 */
const tson::Vector2i &tson::Chunk::getSize() const
{
    return m_size;
}

/*!
 * 'x' and 'y' position in tiles
 * @return Position in int
 */
const tson::Vector2i &tson::Chunk::getPosition() const
{
    return m_position;
}