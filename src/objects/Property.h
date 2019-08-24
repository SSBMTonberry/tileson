//
// Created by robin on 02.08.2019.
//

#ifndef TILESON_PROPERTY_H
#define TILESON_PROPERTY_H

#include "../../TilesonConfig.h"

#if USE_CPP17_FILESYSTEM
    #if MSVC
        #include <filesystem>
        namespace fs = std::filesystem;
    #elif MINGW
    #if __MINGW64_VERSION_MAJOR > 6
            #include <filesystem>
            namespace fs = std::filesystem;
        #else
            #include <experimental/filesystem>
            namespace fs = std::experimental::filesystem;
        #endif
    #elif APPLE
        #if __clang_major__ < 8
            #include <experimental/filesystem>
            namespace fs = std::experimental::filesystem;
        #else
            #include <filesystem>
            namespace fs = std::filesystem;
        #endif
    #else
        #if __GNUC__ < 8 //GCC major version less than 8
    #include <experimental/filesystem>
            namespace fs = std::experimental::filesystem;
        #else
            #include <filesystem>
    namespace fs = std::filesystem;
        #endif
    #endif
#endif

#include <any>
#include <string>
#include <json.hpp>
#include "Color.hpp"

namespace tson
{
    class Property
    {
        public:

            enum class Type : uint8_t
            {
                Undefined = 0,
                Color = 1, /*! color */
                File = 2, /*! file */
                Int = 3, /*! int */
                Boolean = 4, /*! bool */
                Float = 5, /*! float */
                String = 6 /*! string */
            };

            Property();
            Property(const nlohmann::json &json);
            Property(std::string name, std::any value, Type type);

            void setValue(const std::any &value);
            void setStrValue(const std::string &value);
            void setName(const std::string &name);

            [[nodiscard]] const std::type_info& getValueType() const;
            std::string getValueTypeInfo();
            [[nodiscard]]const std::any &getValue() const;
            template <typename T>
            T getValue() const;
            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] Type getType() const;

        protected:
            void setTypeByString(const std::string &str);
            void setValueByType(const nlohmann::json &json);

            Type m_type = Type::Undefined;
            std::string m_name;
            std::any m_value; //Using std::any to assign any type
    };

    template<typename T>
    T Property::getValue() const
    {
        bool isCorrectType = (m_value.type() == typeid(T));

        if(isCorrectType)
        {
            T value = std::any_cast<T>(m_value);
            return value;
        }
        else
        {
            static T defaultValue;
            return defaultValue;
        }
    }
}

#endif //TILESON_PROPERTY_H
