//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_OBJECTTEMPLATE_H
#define TILESON_OBJECTTEMPLATE_H

#include <json.hpp>

namespace tson
{
    class ObjectTemplate
    {
        public:
            ObjectTemplate() = default;
            explicit ObjectTemplate(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

        private:
            /*
             * type 	    string 	        template
               tileset 	    Tileset         External tileset used by the template (optional)
               object 	    Object 	        The object instantiated by this template
             *
             * */
    };
}

#endif //TILESON_OBJECTTEMPLATE_H
