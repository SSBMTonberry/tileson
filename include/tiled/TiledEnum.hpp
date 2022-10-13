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
            inline std::vector<std::string> getValues(uint32_t num);
            inline bool exists(const std::string &str);
            inline bool exists(uint32_t num);

            [[nodiscard]] inline uint32_t getId() const;
            [[nodiscard]] inline uint32_t getMaxValue() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline EnumStorageType getStorageType() const;
            [[nodiscard]] inline bool hasValuesAsFlags() const;

        private:
            inline bool hasFlag(uint32_t value, uint32_t flag) const;
            uint32_t m_id {};
            uint32_t m_maxValue {};
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
            m_values[0] = "None";
            uint32_t valueCounter = (m_valuesAsFlags) ? 1 : 0;
            uint8_t flagBit = 1;
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

            m_maxValue = valueCounter;
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

    uint32_t EnumDefinition::getMaxValue() const
    {
        return m_maxValue;
    }

    std::vector<std::string> EnumDefinition::getValues(uint32_t num)
    {
        std::vector<std::string> values;
        if(m_valuesAsFlags)
        {
            uint32_t flag = 0;
            uint32_t i = 0;
            while(flag < m_maxValue)
            {
                flag = 1 << i;
                ++i;
                if(m_values.count(flag) > 0 && hasFlag(num, flag))
                {
                    values.emplace_back(m_values[flag]);
                }
            }
        }
        else
        {
            std::string v = getValue(num);
            if(!v.empty())
                values.emplace_back();
        }

        return values;
    }

    bool EnumDefinition::hasFlag(uint32_t value, uint32_t flag) const
    {
        return ((value & flag) == flag);
    }

    class EnumValue
    {
        public:
            inline EnumValue() = default;
            inline EnumValue(uint32_t value, EnumDefinition *definition);
            inline EnumValue(const std::string &value, EnumDefinition *definition);

            [[nodiscard]] inline uint32_t getValue() const;
            inline std::string getValueName() const;
            [[nodiscard]] inline std::vector<std::string> getValueNames() const;
            [[nodiscard]] inline EnumDefinition *getDefinition() const;

            inline bool hasFlagValue(uint32_t flag) const;
            template <typename T>
            inline bool hasFlag(T flags) const;
            inline bool hasAnyFlagValue(uint32_t flags) const;
            template <typename T>
            inline bool hasAnyFlag(T flags) const;
            [[nodiscard]] inline bool containsValueName(const std::string &value) const;

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
     * Checks if uint32 value contains a single flag. If 'valuesAsFlags' is not a part of the EnumDefinition, a simple equality comparison will be done
     * instead.
     * @param flags The uint32 value of the flag you want to check
     * @return 'true' if EnumValue has the requested bit activated. 'false' otherwise.
     */
    bool EnumValue::hasFlagValue(uint32_t flags) const
    {
        if(m_definition->hasValuesAsFlags())
            return ((m_value & flags) == flags) ? true : false;

        return m_value == flags;
    }

    /*!
     * Checks if uint32 value contains one of several possible flags. If 'valuesAsFlags' is not a part of the EnumDefinition, a simple equality comparison will be done
     * instead.
     * @param flags The uint32 values of the flags you want to check
     * @return 'true' if EnumValue has the requested bits activated. 'false' otherwise.
     */
    bool EnumValue::hasAnyFlagValue(uint32_t flags) const
    {
        if(m_definition->hasValuesAsFlags())
            return ((m_value & flags) != 0);

        return m_value == flags;
    }

    uint32_t EnumValue::getValue() const
    {
        return m_value;
    }

    /*!
     * Gets the single name of a value.
     * This function is intended for enums that can only have one value (non-flag).
     * If you want to get several value names (flags), use getValueNames() instead.
     * @return A single name for a value
     */
    std::string EnumValue::getValueName() const
    {
        return (m_definition == nullptr) ? "" : m_definition->getValue(m_value);
    }

    /*!
     * Checks if T value contains a single flag. If 'valuesAsFlags' is not a part of the EnumDefinition, a simple equality comparison will be done
     * instead.
     * @tparam T A uint32_t compatible type
     * @param flags One or more flags you want to verify is included.
     * @return true is all flags presented are set. false otherwise.
     */
    template<typename T>
    bool EnumValue::hasFlag(T flags) const
    {
        return hasFlagValue(static_cast<uint32_t>(flags));
    }

    /*!
     * Checks if T value contains one of several possible flags. If 'valuesAsFlags' is not a part of the EnumDefinition, a simple equality comparison will be done
     * instead.
     * @tparam T A uint32_t compatible type
     * @param flags One or more flags you want to verify is included.
     * @return true is all flags presented are set. false otherwise.
     */
    template<typename T>
    bool EnumValue::hasAnyFlag(T flags) const
    {
        return hasAnyFlagValue(static_cast<uint32_t>(flags));
    }

    EnumDefinition *EnumValue::getDefinition() const
    {
        return m_definition;
    }

    std::vector<std::string> EnumValue::getValueNames() const
    {
        return (m_definition == nullptr) ? std::vector<std::string>() : m_definition->getValues(m_value);
    }

    /*!
     *
     * @param value
     * @return
     */
    bool EnumValue::containsValueName(const std::string &value) const
    {
        if(m_definition != nullptr)
        {
            if(m_definition->hasValuesAsFlags())
            {
                std::vector<std::string> values = m_definition->getValues(m_value);
                auto it = std::find(values.begin(), values.end(), value);
                return it != values.end();
            }
            return m_definition->getValue(value) == m_value;
        }
        return false;
    }
}

#endif //TILESON_TILEDENUM_HPP
