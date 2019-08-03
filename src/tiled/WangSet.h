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

        private:

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
