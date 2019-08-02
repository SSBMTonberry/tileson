//
// Created by robin on 02.08.2019.
//

#ifndef TILESON_PROPERTY_H
#define TILESON_PROPERTY_H

#include <any>
#include <string>
namespace tson
{
    class Property
    {
        public:
            Property();
            explicit Property(std::string name);
            Property(std::string name, std::any value);

            void setValue(const std::any &value);
            void setStrValue(const std::string &value);
            void setName(const std::string &name);

            const std::type_info& getValueType() const;
            std::string getValueTypeInfo();
            const std::any &getValue() const;
            template <typename T>
            T getValue() const;
            const std::string &getName() const;


        protected:
            void initialize();

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
