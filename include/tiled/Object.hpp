//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_OBJECT_HPP
#define TILESON_OBJECT_HPP

//#include "../external/json.hpp"
#include "../objects/Vector2.hpp"
#include "../objects/PropertyCollection.hpp"
#include "Text.hpp"

#include "../common/Enums.hpp"

namespace tson
{
    class Object
    {
        public:
            //enum class Type : uint8_t
            //{
            //        Undefined = 0,
            //        Object = 1,
            //        Ellipse = 2,
            //        Rectangle = 3,
            //        Point = 4,
            //        Polygon = 5,
            //        Polyline = 6,
            //        Text = 7,
            //        Template = 8
            //};


            inline Object() = default;
            inline explicit Object(IJson &json);
            inline bool parse(IJson &json);

            [[nodiscard]] inline ObjectType getObjectType() const;
            [[nodiscard]] inline bool isEllipse() const;
            [[nodiscard]] inline uint32_t getGid() const;
            [[nodiscard]] inline const Vector2i &getSize() const;
            [[nodiscard]] inline int getId() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline bool isPoint() const;
            [[nodiscard]] inline float getRotation() const;
            [[nodiscard]] inline const std::string &getTemplate() const;
            [[nodiscard]] inline const std::string &getType() const;
            [[nodiscard]] inline bool isVisible() const;
            [[nodiscard]] inline const Vector2i &getPosition() const;

            [[nodiscard]] inline const std::vector<tson::Vector2i> &getPolygons() const;
            [[nodiscard]] inline const std::vector<tson::Vector2i> &getPolylines() const;
            [[nodiscard]] inline PropertyCollection &getProperties();
            [[nodiscard]] inline const Text &getText() const;

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            //v1.2.0-stuff
            [[nodiscard]] inline TileFlipFlags getFlipFlags() const;
            inline bool hasFlipFlags(TileFlipFlags flags);


        private:
            inline void setObjectTypeByJson(IJson &json);

            ObjectType                        m_objectType = ObjectType::Undefined;    /*! Says with object type this is */
            bool                              m_ellipse {};                            /*! 'ellipse': Used to mark an object as an ellipse */
            uint32_t                          m_gid {};                                /*! 'gid': GID, only if object comes from a Tilemap */
            tson::Vector2i                    m_size;                                  /*! x = 'width' (Width in pixels), y = 'height' (Height in pixels). Ignored if using a gid.)*/
            int                               m_id{};                                  /*! 'id': Incremental id - unique across all objects */
            std::string                       m_name;                                  /*! 'name':  String assigned to name field in editor*/
            bool                              m_point {};                              /*! 'point': Used to mark an object as a point */
            std::vector<tson::Vector2i>       m_polygon; 	                           /*! 'polygon': A list of x,y coordinates in pixels */
            std::vector<tson::Vector2i>       m_polyline; 	                           /*! 'polyline': A list of x,y coordinates in pixels */
            tson::PropertyCollection          m_properties; 	                       /*! 'properties': A list of properties (name, value, type). */
            float                             m_rotation {};                           /*! 'rotation': Angle in degrees clockwise */
            std::string                       m_template;                              /*! 'template': Reference to a template file, in case object is a template instance */
            tson::Text                        m_text; 	                               /*! first: 'text' second: 'wrap' */
            std::string                       m_type;                                  /*! 'type': String assigned to type field in editor */
            bool                              m_visible {};                            /*! 'visible': Whether object is shown in editor. */
            tson::Vector2i                    m_position;                              /*! 'x' and 'y': coordinate in pixels */

            //v1.2.0-stuff
            tson::TileFlipFlags               m_flipFlags = TileFlipFlags::None;       /*! Resolved using bit 32, 31 and 30 from gid */
    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T tson::Object::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

/*!
 * Parses a json Tiled object
 * @param json
 */
tson::Object::Object(IJson &json)
{
    parse(json);
}

/*!
 * Parses a json Tiled object and autoamtically determines the object type based on the data presented.
 * Call getObjectType() to see what object type it is.
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Object::parse(IJson &json)
{
    bool allFound = true;

    if(json.count("ellipse") > 0) m_ellipse = json["ellipse"].get<bool>(); //Optional
    if(json.count("gid") > 0)
    {
        uint32_t gid = json["gid"].get<uint32_t>(); //Optional
        if (gid & FLIPPED_HORIZONTALLY_FLAG) m_flipFlags |= TileFlipFlags::Horizontally;
        if (gid & FLIPPED_VERTICALLY_FLAG) m_flipFlags |= TileFlipFlags::Vertically;
        if (gid & FLIPPED_DIAGONALLY_FLAG) m_flipFlags |= TileFlipFlags::Diagonally;

        // Clear flags
        gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

        m_gid = gid;
    }
    if(json.count("id") > 0) m_id = json["id"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("point") > 0) m_point = json["point"].get<bool>(); //Optional
    if(json.count("rotation") > 0) m_rotation = json["rotation"].get<float>(); else allFound = false;
    if(json.count("template") > 0) m_template = json["template"].get<std::string>(); //Optional
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); else allFound = false;
    if(json.count("visible") > 0) m_visible = json["visible"].get<bool>(); else allFound = false;

    if(json.count("width") > 0 && json.count("height") > 0)
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
    if(json.count("x") > 0 && json.count("y") > 0)
        m_position = {json["x"].get<int>(), json["y"].get<int>()}; else allFound = false;

    if(json.count("text") > 0)
    {
        bool hasColor = json["text"].count("color") > 0;
        tson::Color c = (hasColor) ? tson::Colori(json["text"]["color"].get<std::string>()) : tson::Colori();
        m_text = {json["text"]["text"].get<std::string>(), json["text"]["wrap"].get<bool>(), c}; //Optional
    }

    setObjectTypeByJson(json);

    if(m_objectType == ObjectType::Template)
        allFound = true; //Just accept anything with this type

    //More advanced data
    if(json.count("polygon") > 0 && json["polygon"].isArray())
    {
        auto &polygon = json.array("polygon");
        std::for_each(polygon.begin(), polygon.end(),[&](std::unique_ptr<IJson> &item)
        {
            IJson &j = *item;
            m_polygon.emplace_back(j["x"].get<int>(), j["y"].get<int>());
        });

    }

    if(json.count("polyline") > 0 && json["polyline"].isArray())
    {
        auto &polyline = json.array("polyline");
        std::for_each(polyline.begin(), polyline.end(),[&](std::unique_ptr<IJson> &item)
        {
            IJson &j = *item;
            m_polyline.emplace_back(j["x"].get<int>(), j["y"].get<int>());
        });
    }

    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item)
        {
            m_properties.add(*item);
        });
    }

    return allFound;
}


/*!
 * Sets an object type based on json data.
 * @param json
 */
void tson::Object::setObjectTypeByJson(IJson &json)
{
    m_objectType = ObjectType::Undefined;
    if(m_ellipse)
        m_objectType = ObjectType::Ellipse;
    else if(m_point)
        m_objectType = ObjectType::Point;
    else if(json.count("polygon") > 0)
        m_objectType = ObjectType::Polygon;
    else if(json.count("polyline") > 0)
        m_objectType = ObjectType::Polyline;
    else if(json.count("text") > 0)
        m_objectType = ObjectType::Text;
    else if(json.count("gid") > 0)
        m_objectType = ObjectType::Object;
    else if(json.count("template") > 0)
        m_objectType = ObjectType::Template;
    else
        m_objectType = ObjectType::Rectangle;
}

/*!
 * Gets what type of object this is.
 * @return
 */

tson::ObjectType tson::Object::getObjectType() const
{
    return m_objectType;
}

/*!
 * 'ellipse': Used to mark an object as an ellipse
 * @return
 */
bool tson::Object::isEllipse() const
{
    return m_ellipse;
}

/*!
 * 'gid': GID, only if object comes from a Tilemap
 * @return
 */
uint32_t tson::Object::getGid() const
{
    return m_gid;
}

/*!
 * x = 'width' (Width in pixels), y = 'height' (Height in pixels). Ignored if using a gid.)
 * @return
 */
const tson::Vector2i &tson::Object::getSize() const
{
    return m_size;
}

/*!
 * 'id': Incremental id - unique across all objects
 * @return
 */
int tson::Object::getId() const
{
    return m_id;
}

/*!
 * 'name': String assigned to name field in editor
 * @return
 */
const std::string &tson::Object::getName() const
{
    return m_name;
}

/*!
 * 'point': Used to mark an object as a point
 * @return true if the object is of type point
 */
bool tson::Object::isPoint() const
{
    return m_point;
}

/*!
 * 'rotation': Angle in degrees clockwise
 * @return
 */
float tson::Object::getRotation() const
{
    return m_rotation;
}

/*!
 * 'template': Reference to a template file, in case object is a template instance
 * @return
 */
const std::string &tson::Object::getTemplate() const
{
    return m_template;
}

/*!
 * 'type': String assigned to type field in editor
 * @return
 */
const std::string &tson::Object::getType() const
{
    return m_type;
}

/*!
 * 'visible': Whether object is shown in editor.
 * @return
 */
bool tson::Object::isVisible() const
{
    return m_visible;
}

/*!
 * 'x' and 'y': coordinate in pixels
 * @return
 */
const tson::Vector2i &tson::Object::getPosition() const
{
    return m_position;
}

/*!
 * 'polygon': A list of x,y coordinates in pixels.
 * If this is a Polygon type, this function will return the points used to create it
 * @return
 */
const std::vector<tson::Vector2i> &tson::Object::getPolygons() const
{
    return m_polygon;
}

/*!
 * 'polyline': A list of x,y coordinates in pixels
 * If this is a Polyline type, this function will return the points used to create it
 * @return
 */
const std::vector<tson::Vector2i> &tson::Object::getPolylines() const
{
    return m_polyline;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Object::getProperties()
{
    return m_properties;
}

/*!
 * 'type': String assigned to type field in editor
 * @return
 */
const tson::Text &tson::Object::getText() const
{
    return m_text;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Object::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);
    return nullptr;
}

/*!
 * Get all flip flags
 * @return
 */
tson::TileFlipFlags tson::Object::getFlipFlags() const
{
    return m_flipFlags;
}

/*!
 *
 * @param flags Which flags to check for. Several flags can be checked at once using the bitwise or operator.
 * Example:
 * hasFlipFlags(TileFlipFlags::Vertically | TileFlipFlags::Horizontally)
 *
 * @return true if the flag(s) specified are set
 */
bool tson::Object::hasFlipFlags(TileFlipFlags flags)
{
    return ((m_flipFlags & flags) == flags) ? true : false;
}

#endif //TILESON_OBJECT_HPP
