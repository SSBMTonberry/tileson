//
// Created by robin on 06.06.22.
//

#ifndef TILESON_TILEDENUM_HPP
#define TILESON_TILEDENUM_HPP

namespace tson
{
    class EnumDefinition
    {
        public:
            inline explicit EnumDefinition(IJson &json);

        private:
            uint32_t m_id {};
            std::string m_name {};
            std::map<uint32_t, std::string> m_values {};
            bool m_valuesAsFlags {false};
            EnumStorageType m_storageType { EnumStorageType::Unspecified };
    };

    EnumDefinition::EnumDefinition(IJson &json)
    {
        m_id = json.get<uint32_t>("id");
        m_name = json.get<std::string>("name");
        std::string type = json.get<std::string>("storageType");
        m_storageType = (type == "int") ? EnumStorageType::Int : (type == "string") ? EnumStorageType::String : EnumStorageType::Unspecified;
        m_valuesAsFlags = json.get<bool>("valuesAsFlags");

        if(json.count("values") > 0 && json["values"].isArray())
        {
            uint32_t valueCounter = 0;
            uint8_t flagBit = 0;
            auto &array = json.array("values");
            std::for_each(array.begin(), array.end(), [&](std::unique_ptr<IJson> &item)
            {
                std::string v = item->get<std::string>();
                m_values[valueCounter] = v;
                if(m_valuesAsFlags)
                {
                    valueCounter = 1 << flagBit;
                    ++flagBit;
                }
                else
                {
                    ++valueCounter;
                }
            });
        }
    }

    class EnumValue
    {
        public:
            inline EnumValue(uint32_t value, EnumDefinition *definition);
            inline EnumValue(const std::string &value, EnumDefinition *definition);

        private:
    };

    EnumValue::EnumValue(uint32_t value, EnumDefinition *definition)
    {

    }

    EnumValue::EnumValue(const std::string &value, EnumDefinition *definition)
    {

    }
}

#endif //TILESON_TILEDENUM_HPP
