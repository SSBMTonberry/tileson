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
            inline uint32_t getValue(const std::string &str);
            inline std::string getValue(uint32_t num);
            inline bool exists(const std::string &str);
            inline bool exists(uint32_t num);

            [[nodiscard]] inline uint32_t getId() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline EnumStorageType getStorageType() const;

            [[nodiscard]] inline bool hasValuesAsFlags() const;

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

    uint32_t EnumDefinition::getValue(const std::string &str)
    {
        auto result = std::find_if(m_values.begin(), m_values.end(), [&](const std::pair<uint32_t, std::string> &pair)
        {
            return pair.second == str;
        });

        if(result != m_values.end())
            return result->first;

        return 0;
    }

    std::string EnumDefinition::getValue(uint32_t num)
    {
        return (m_values.count(num) > 0) ? m_values[num] : "";
    }

    bool EnumDefinition::hasValuesAsFlags() const
    {
        return m_valuesAsFlags;
    }

    bool EnumDefinition::exists(const std::string &str)
    {
        auto result = std::find_if(m_values.begin(), m_values.end(), [&](const std::pair<uint32_t, std::string> &pair)
        {
            return pair.second == str;
        });

        if(result != m_values.end())
            return true;

        return false;
    }

    bool EnumDefinition::exists(uint32_t num) { return (m_values.count(num) > 0); }

    uint32_t EnumDefinition::getId() const
    {
        return m_id;
    }

    const std::string &EnumDefinition::getName() const
    {
        return m_name;
    }

    EnumStorageType EnumDefinition::getStorageType() const
    {
        return m_storageType;
    }

    class EnumValue
    {
        public:
            inline EnumValue(uint32_t value, EnumDefinition *definition);
            inline EnumValue(const std::string &value, EnumDefinition *definition);

            inline uint32_t getValue();
            inline std::string getName();

            inline bool hasFlag(uint32_t flag);
            inline bool hasAnyFlag(uint32_t flags);

        private:
            uint32_t m_value {0};
            EnumDefinition *m_definition = nullptr;
    };

    EnumValue::EnumValue(uint32_t value, EnumDefinition *definition) : m_value {value}, m_definition {definition}
    {

    }

    EnumValue::EnumValue(const std::string &value, EnumDefinition *definition) : m_definition {definition}
    {
        if(!value.empty() && definition != nullptr)
        {
            std::vector<std::string> values = Tools::SplitString(value, ',');
            for(auto &item : values)
            {
                uint32_t v = definition->getValue(item);
                m_value |= v;
            }
        }
    }

    /*!
     * Checks if enum value contains a single flag. If 'valuesAsFlags' is not a part of the EnumDefinition, a simple equality comparison will be done
     * instead.
     * @param flag The uint32 value of the flag you want to check
     * @return 'true' if EnumValue has the requested bit activated. 'false' otherwise.
     */
    bool EnumValue::hasFlag(uint32_t flag)
    {
        if(m_definition->hasValuesAsFlags())
            return ((m_value & flag) == flag) ? true : false;

        return m_value == flag;
    }

    /*!
     * Checks if enum value contains one of several possible flags. If 'valuesAsFlags' is not a part of the EnumDefinition, a simple equality comparison will be done
     * instead.
     * @param flag The uint32 values of the flags you want to check
     * @return 'true' if EnumValue has the requested bits activated. 'false' otherwise.
     */
    bool EnumValue::hasAnyFlag(uint32_t flags)
    {
        if(m_definition->hasValuesAsFlags())
            return ((m_value & flags) != 0);

        return m_value == flags;
    }

    uint32_t EnumValue::getValue()
    {
        return m_value;
    }

    std::string EnumValue::getName()
    {
        return m_definition->getValue(m_value);
    }
}

#endif //TILESON_TILEDENUM_HPP
