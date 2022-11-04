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
            //inline Text(std::string _text, bool _wrap, tson::Colori _color) : text {std::move(_text)}, wrap {_wrap}, color {_color} {};
            inline explicit Text(IJson &json)
            {
                bool hasColor = json.count("color") > 0;
                tson::Color c = (hasColor) ? tson::Colori(json["color"].get<std::string>()) : tson::Colori();
                color = c;
                text = (json.count("text") > 0) ? json["text"].get<std::string>() : "";
                wrap = (json.count("wrap") > 0) ? json["wrap"].get<bool>() : false;

                //Previously missing properties
                bold = (json.count("bold") > 0) ? json["bold"].get<bool>() : false;
                fontFamily = (json.count("fontfamily") > 0) ? json["fontfamily"].get<std::string>() : "sans-serif";
                horizontalAlignment = (json.count("halign") > 0) ? resolveTextAlignmentByString(json["halign"].get<std::string>()) : TextAlignment::Left;
                italic = (json.count("italic") > 0) ? json["italic"].get<bool>() : false;
                kerning = (json.count("kerning") > 0) ? json["kerning"].get<bool>() : true;
                pixelSize = (json.count("pixelsize") > 0) ? json["pixelsize"].get<int32_t>() : 16;
                strikeout = (json.count("strikeout") > 0) ? json["strikeout"].get<bool>() : false;
                underline = (json.count("underline") > 0) ? json["underline"].get<bool>() : false;
                verticalAlignment = (json.count("valign") > 0) ? resolveTextAlignmentByString(json["valign"].get<std::string>()) : TextAlignment::Top;
            };

            //Just make it simple
            std::string text {};
            tson::Colori color {};
            bool wrap{};

            //Previously missing properties
            bool bold {false};
            std::string fontFamily {"sans-serif"};
            TextAlignment horizontalAlignment {TextAlignment::Left};
            bool italic {false};
            bool kerning {true};
            int pixelSize {16};
            bool strikeout {false};
            bool underline {false};
            TextAlignment verticalAlignment {TextAlignment::Top};

        private:
            [[nodiscard]] TextAlignment resolveTextAlignmentByString(const std::string &str) const
            {
                if(str == "left") return TextAlignment::Left;
                if(str == "center") return TextAlignment::Center;
                if(str == "right") return TextAlignment::Right;
                if(str == "justify") return TextAlignment::Justify;
                if(str == "top") return TextAlignment::Top;
                if(str == "bottom") return TextAlignment::Bottom;

                return TextAlignment::Unresolved;
            }
    };
}

#endif //TILESON_TEXT_HPP
