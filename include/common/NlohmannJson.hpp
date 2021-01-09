//
// Created by robin on 08.01.2021.
//

#ifndef TILESON_NLOHMANNJSON_HPP
#define TILESON_NLOHMANNJSON_HPP

class NlohmannJson : public tson::IJson
{
    public:
        NlohmannJson() = default;

        explicit NlohmannJson(nlohmann::json *json)
        {
            m_json = json;
        }

        explicit NlohmannJson(const fs::path &path)
        {
            parse(path);
        }

        explicit NlohmannJson(const void *data, size_t size)
        {
            parse(data, size);
        }

        std::unique_ptr<IJson> at(std::string_view key) override
        {
            return std::make_unique<NlohmannJson>(&m_json->operator[](key.data()).front());
        }

        std::unique_ptr<IJson> at(size_t pos) override
        {
            return std::make_unique<NlohmannJson>(&m_json->at(pos));
        }

        std::vector<std::unique_ptr<IJson>> array(std::string_view key) override
        {
            std::vector<std::unique_ptr<IJson>> vec;
            if(m_json->count(key.data()) > 0 && m_json->operator[](key.data()).is_array())
            {
                std::for_each(m_json->operator[](key.data()).begin(), m_json->operator[](key.data()).end(), [&](nlohmann::json &item)
                {
                    nlohmann::json *ptr = &item;
                    vec.emplace_back(std::make_unique<NlohmannJson>(ptr));
                });
            }
            return vec;
        }

        size_t size() const override
        {
            return m_json->size();
        }

        //IJson &operator[](std::string_view key) override
        //{
        //    NlohmannJson j {m_json[key.data()].front()};
        //    return j; //*this;
        //}

        bool parse(const fs::path &path) override
        {
            if(fs::exists(path) && fs::is_regular_file(path))
            {
                m_data = std::make_unique<nlohmann::json>();
                std::ifstream i(path.u8string());
                try
                {
                    i >> *m_data;
                    m_json = m_data.get();
                }
                catch(const nlohmann::json::parse_error &error)
                {
                    std::string message = "Parse error: ";
                    message += std::string(error.what());
                    message += std::string("\n");
                    std::cerr << message;
                    return false;
                }
                return true;
            }
            return false;
        }

        bool parse(const void *data, size_t size) override
        {
            m_data = std::make_unique<nlohmann::json>();
            tson::MemoryStream mem {(uint8_t *)data, size};
            try
            {
                mem >> *m_data;
                m_json = m_data.get();
            }
            catch (const nlohmann::json::parse_error& error)
            {
                std::string message = "Parse error: ";
                message += std::string(error.what());
                message += std::string("\n");
                std::cerr << message;
                return false;
            }
            return true;
        }

        [[nodiscard]] size_t count(std::string_view key) const override
        {
            return m_json->operator[](key.data()).count(key);
        }

        [[nodiscard]] bool any(std::string_view key) const override
        {
            return m_json->operator[](key.data()).count(key) > 1;
        }

        [[nodiscard]] bool isArray() const override
        {
            return false;
        }

        [[nodiscard]] bool isObject() const override
        {
            return true;
        }

        [[nodiscard]] bool isNull() const override
        {
            return false;
        }

    protected:
        [[nodiscard]] int32_t getInt32(std::string_view key) const override
        {
            return m_json->operator[](key.data()).get<int32_t>();
        }

        [[nodiscard]] uint32_t getUInt32(std::string_view key) const override
        {
            return m_json->operator[](key.data()).get<uint32_t>();
        }

        [[nodiscard]] int64_t getInt64(std::string_view key) const override
        {
            return m_json->operator[](key.data()).get<int64_t>();
        }

        [[nodiscard]] uint64_t getUInt64(std::string_view key) const override
        {
            return m_json->operator[](key.data()).get<uint64_t>();
        }

        [[nodiscard]] double getDouble(std::string_view key) const override
        {
            return m_json->operator[](key.data()).get<double>();
        }

        [[nodiscard]] std::string getString(std::string_view key) const override
        {
            return m_json->operator[](key.data()).get<std::string>();
        }

        [[nodiscard]] bool getBool(std::string_view key) const override
        {
            return m_json->operator[](key.data()).get<bool>();
        }

        [[nodiscard]] int32_t getInt32() const override
        {
            return m_json->get<int32_t>();
        }

        [[nodiscard]] uint32_t getUInt32() const override
        {
            return m_json->get<uint32_t>();
        }

        [[nodiscard]] int64_t getInt64() const override
        {
            return m_json->get<int64_t>();
        }

        [[nodiscard]] uint64_t getUInt64() const override
        {
            return m_json->get<uint64_t>();
        }

        [[nodiscard]] double getDouble() const override
        {
            return m_json->get<double>();
        }

        [[nodiscard]] std::string getString() const override
        {
            return m_json->get<std::string>();
        }

        [[nodiscard]] bool getBool() const override
        {
            return m_json->get<bool>();
        }

    private:
        nlohmann::json *m_json = nullptr;
        std::unique_ptr<nlohmann::json> m_data = nullptr; //Only used if this is the owner json!
};

#endif //TILESON_NLOHMANNJSON_HPP
