//
// Created by robin on 05.08.2019.
//

#ifndef TILESON_TEXT_HPP
#define TILESON_TEXT_HPP

#include <string>

namespace tson
{
    class Text
    {
        public:
            inline Text() = default;
            /*!
             *
             * @param _text Text
             * @param _wrap If the text is marked as wrapped
             */
            inline Text(std::string _text, bool _wrap) : text {std::move(_text)}, wrap {_wrap} {};
            //Just make it simple
            std::string text;
            bool wrap{};
    };
}

#endif //TILESON_TEXT_HPP
