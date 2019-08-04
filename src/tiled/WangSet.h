//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_WANGSET_H
#define TILESON_WANGSET_H

#include <json.hpp>

namespace tson
{
    class WangSet
    {
        public:
            WangSet() = default;
            explicit WangSet(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] int getTile() const;

        private:
            //TODO: cornercolors 	array 	      Array of Wang colors
            //TODO: edgecolors 	    array 	      Array of Wang colors
            std::string             m_name;       /*! 'name': Name of the Wang set */
            int                     m_tile{};       /*! 'tile': Local ID of tile representing the Wang set */
            //TODO: wangtiles 	    array 	      Array of Wang tiles

            /*
               cornercolors 	array 	      Array of Wang colors
               edgecolors 	    array 	      Array of Wang colors
               name 	        string 	      Name of the Wang set
               tile 	        int 	      Local ID of tile representing the Wang set
               wangtiles 	    array 	      Array of Wang tiles
             */
    };
}

#endif //TILESON_WANGSET_H
