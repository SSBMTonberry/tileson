//
// Created by robin on 06.06.22.
//

#ifndef TILESON_TILEDCLASS_HPP
#define TILESON_TILEDCLASS_HPP

namespace tson
{
    class TiledClass
    {
        public:

        private:
            uint32_t m_id {};
            std::string m_name {};
            std::string m_type {};
            PropertyCollection m_members {};

    };
}

#endif //TILESON_TILEDCLASS_HPP
