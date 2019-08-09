//
// Created by robin on 02.08.2019.
//

#include "Chunk.h"

/*!
 * Parses 'chunk' data from Tiled json and stores the values in this class
 * @param json json-data
 */
tson::Chunk::Chunk(const nlohmann::json &json)
{
    parse(json);
}

/*!
 * Parses 'chunk' data from Tiled json and stores the values in this class
 * @param json json-data
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Chunk::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("width") > 0 && json.count("height") > 0)
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
    if(json.count("x") > 0 && json.count("y") > 0)
        m_position = {json["x"].get<int>(), json["y"].get<int>()}; else allFound = false;

    //Handle DATA (Optional)
    if(json.count("data") > 0)
    {
        if(json["data"].is_array())
        {
            std::for_each(json["data"].begin(), json["data"].end(), [&](const nlohmann::json &item) { m_data.push_back(item.get<int>()); });
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
