//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_TERRAIN_H
#define TILESON_TERRAIN_H

#include <json.hpp>
#include "../objects/PropertyCollection.h"

namespace tson
{
    class Terrain
    {
        public:
            Terrain() = default;
            Terrain(std::string name, int tile);
            explicit Terrain(const nlohmann::json &json);

            bool parse(const nlohmann::json &json);

            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] int getTile() const;
            [[nodiscard]] PropertyCollection &getProperties();

            template <typename T>
            T get(const std::string &name);
            tson::Property * getProp(const std::string &name);

        private:
            std::string                 m_name;        /*! 'name': Name of terrain */
            int                         m_tile {};     /*! 'tile': Local ID of tile representing terrain */
            tson::PropertyCollection    m_properties;  /*! 'properties': A list of properties (name, value, type). */
    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T tson::Terrain::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

#endif //TILESON_TERRAIN_H
