//
// Created by robin on 02.08.2019.
//

#ifndef TILESON_OBJECT_H
#define TILESON_OBJECT_H

#include <json.hpp>
#include "../objects/Vector2.hpp"
#include "../objects/PropertyCollection.h"
#include "Text.hpp"

namespace tson
{
    class Object
    {
        public:
            enum class Type : uint8_t
            {
                Undefined = 0,
                Object = 1,
                Ellipse = 2,
                Rectangle = 3,
                Point = 4,
                Polygon = 5,
                Polyline = 6,
                Text = 7,
                Template = 8
            };


            Object() = default;
            explicit Object(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] Type getObjectType() const;
            [[nodiscard]] bool isEllipse() const;
            [[nodiscard]] int getGid() const;
            [[nodiscard]] const Vector2i &getSize() const;
            [[nodiscard]] int getId() const;
            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] bool isPoint() const;
            [[nodiscard]] float getRotation() const;
            [[nodiscard]] const std::string &getTemplate() const;
            [[nodiscard]] const std::string &getType() const;
            [[nodiscard]] bool isVisible() const;
            [[nodiscard]] const Vector2i &getPosition() const;

            [[nodiscard]] const std::vector<tson::Vector2i> &getPolygons() const;
            [[nodiscard]] const std::vector<tson::Vector2i> &getPolylines() const;
            [[nodiscard]] PropertyCollection &getProperties();
            [[nodiscard]] const Text &getText() const;

            template <typename T>
            T get(const std::string &name);
            tson::Property * getProp(const std::string &name);

        private:
            void setObjectTypeByJson(const nlohmann::json &json);

            Type                              m_objectType = Type::Undefined;    /*! Says with object type this is */
            bool                              m_ellipse {};                      /*! 'ellipse': Used to mark an object as an ellipse */
            int                               m_gid {};                          /*! 'gid': GID, only if object comes from a Tilemap */
            tson::Vector2i                    m_size;                            /*! x = 'width' (Width in pixels), y = 'height' (Height in pixels). Ignored if using a gid.)*/
            int                               m_id{};                            /*! 'id': Incremental id - unique across all objects */
            std::string                       m_name;                            /*! 'name':  String assigned to name field in editor*/
            bool                              m_point {};                        /*! 'point': Used to mark an object as a point */
            std::vector<tson::Vector2i>       m_polygon; 	                     /*! 'polygon': A list of x,y coordinates in pixels */
            std::vector<tson::Vector2i>       m_polyline; 	                     /*! 'polyline': A list of x,y coordinates in pixels */
            tson::PropertyCollection          m_properties; 	                 /*! 'properties': A list of properties (name, value, type). */
            float                             m_rotation {};                     /*! 'rotation': Angle in degrees clockwise */
            std::string                       m_template;                        /*! 'template': Reference to a template file, in case object is a template instance */
            tson::Text                        m_text; 	                         /*! first: 'text' second: 'wrap' */
            std::string                       m_type;                            /*! 'type': String assigned to type field in editor */
            bool                              m_visible {};                      /*! 'visible': Whether object is shown in editor. */
            tson::Vector2i                    m_position;                        /*! 'x' and 'y': coordinate in pixels */
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

#endif //TILESON_OBJECT_H
