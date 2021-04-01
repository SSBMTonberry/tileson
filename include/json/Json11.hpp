//
// Created by robin on 16.01.2021.
//

#ifndef TILESON_JSON11_HPP
#define TILESON_JSON11_HPP

namespace tson
{
    class Json11 : public tson::IJson
    {
        public:
            inline Json11() = default;

            IJson &operator[](std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                {
                    if(m_json->is_object())
                    {
                        m_arrayCache[key.data()] = std::make_unique<Json11>(m_json->operator[](key.data()));
                    }
                }

                return *m_arrayCache[key.data()].get();
            }

            inline explicit Json11(const json11::Json &json) : m_json {&json}
            {

            }

            inline IJson& at(std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                {
                    if(m_json->is_object())
                    {
                        m_arrayCache[key.data()] = std::make_unique<Json11>(m_json->operator[](key.data()));
                    }
                }
                return *m_arrayCache[key.data()].get();
            }

            inline IJson& at(size_t pos) override
            {
                if(m_arrayPosCache.count(pos) == 0)
                {
                    const std::vector<json11::Json> &a = m_json->array_items();
                    m_arrayPosCache[pos] = std::make_unique<Json11>(a.at(pos));
                }

                return *m_arrayPosCache[pos];
            }

            std::vector<std::unique_ptr<IJson>> array() override
            {
                std::vector<std::unique_ptr<IJson>> vec;
                if(m_json->is_array())
                {
                    for (const json11::Json &item : m_json->array_items())
                    {
                        vec.emplace_back(std::make_unique<Json11>(item));
                    }
                }

                return vec;
            }

            inline std::vector<std::unique_ptr<IJson>> &array(std::string_view key) override
            {
                if(m_arrayListDataCache.count(key.data()) == 0)
                {
                    if(count(key.data()) > 0)
                    {
                        if(isObject())
                        {
                            const json11::Json &v = m_json->operator[](key.data());
                            if(v.is_array())
                            {
                                for (const json11::Json &item : v.array_items())
                                {
                                    m_arrayListDataCache[key.data()].emplace_back(std::make_unique<Json11>(item));
                                }
                            }
                        }
                    }
                }


                return m_arrayListDataCache[key.data()];
            }

            [[nodiscard]] inline size_t size() const override
            {
                if(m_json->is_object())
                    return m_json->object_items().size();
                else if(m_json->is_array())
                    return m_json->array_items().size();

                return 0;
            }

            inline bool parse(const fs::path &path) override
            {
                clearCache();
                m_data = nullptr;
                m_json = nullptr;
                if (fs::exists(path) && fs::is_regular_file(path))
                {
                    std::ifstream file(path.u8string());
                    std::string str;
                    m_path = path.parent_path();

                    file.seekg(0, std::ios::end);
                    str.reserve(file.tellg());
                    file.seekg(0, std::ios::beg);

                    str.assign((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());

                    m_data = std::make_unique<json11::Json>();

                    try
                    {
                        std::string strError;
                        *m_data = json11::Json::parse(str, strError);
                        if(!strError.empty())
                        {
                            std::cerr << strError << "\n";
                            return false;
                        }
                        m_json = m_data.get();
                    }
                    catch (const std::exception &error)
                    {
                        std::string message = "Json11 parse error: ";
                        message += std::string(error.what());
                        message += std::string("\n");
                        std::cerr << message;
                        return false;
                    }
                    return true;
                }
                return false;
            }

            inline bool parse(const void *data, size_t size) override
            {
                clearCache();
                m_json = nullptr;
                std::string str;

                str.reserve(size);

                tson::MemoryStream mem{(uint8_t *) data, size};

                str.assign((std::istreambuf_iterator<char>(mem)),
                           std::istreambuf_iterator<char>());

                m_data = std::make_unique<json11::Json>();

                try
                {
                    std::string strError;

                    *m_data = json11::Json::parse(str, strError);
                    if(!strError.empty())
                    {
                        std::cout << strError << "\n";
                        return false;
                    }
                    m_json = m_data.get();
                }
                catch (const std::exception &error)
                {
                    std::string message = "Json11 parse error: ";
                    message += std::string(error.what());
                    message += std::string("\n");
                    std::cerr << message;
                    return false;
                }
                return true;
            }

            [[nodiscard]] inline size_t count(std::string_view key) const override
            {
                if (isObject())
                {
                    //const json11::Json &j = m_json->operator[](key.data());
                    //size_t s1 = j.object_items().size();
                    return m_json->object_items().count(key.data());
                }

                return 0;
            }

            [[nodiscard]] inline bool any(std::string_view key) const override
            {
                return count(key) > 0;
            }

            [[nodiscard]] inline bool isArray() const override
            {
                return m_json->is_array();
            }

            [[nodiscard]] inline bool isObject() const override
            {
                return m_json->is_object();
            }

            [[nodiscard]] inline bool isNull() const override
            {
                return m_json->is_null();
            }

            fs::path directory() const override
            {
                return m_path;
            }

            void directory(const fs::path &directory) override
            {
                m_path = directory;
            }

        protected:
            [[nodiscard]] inline int32_t getInt32(std::string_view key) override
            {
                return static_cast<int32_t>(getDouble(key));
            }

            [[nodiscard]] inline uint32_t getUInt32(std::string_view key) override
            {
                return static_cast<uint32_t>(getDouble(key));
            }

            [[nodiscard]] inline int64_t getInt64(std::string_view key) override
            {
                return static_cast<int64_t>(getDouble(key));
            }

            [[nodiscard]] inline uint64_t getUInt64(std::string_view key) override
            {
                return static_cast<uint64_t>(getDouble(key));
            }

            [[nodiscard]] inline double getDouble(std::string_view key) override
            {
                return m_json->operator[](key.data()).number_value();
            }

            [[nodiscard]] inline std::string getString(std::string_view key) override
            {
                return m_json->operator[](key.data()).string_value(); // .get<std::string>();
            }

            [[nodiscard]] inline bool getBool(std::string_view key) override
            {
                return m_json->operator[](key.data()).bool_value();
            }

            [[nodiscard]] float getFloat(std::string_view key) override
            {
                return static_cast<float>(getDouble(key));
            }

            [[nodiscard]] inline int32_t getInt32() override
            {
                return static_cast<int32_t>(getDouble());
            }

            [[nodiscard]] inline uint32_t getUInt32() override
            {
                return static_cast<uint32_t>(getDouble());
            }

            [[nodiscard]] inline int64_t getInt64() override
            {
                return static_cast<int64_t>(getDouble());
            }

            [[nodiscard]] inline uint64_t getUInt64() override
            {
                return static_cast<uint64_t>(getDouble());
            }

            [[nodiscard]] inline double getDouble() override
            {
                return m_json->number_value();
            }

            [[nodiscard]] inline std::string getString() override
            {
                return m_json->string_value();
            }

            [[nodiscard]] inline bool getBool() override
            {
                return m_json->bool_value();
            }

            [[nodiscard]] float getFloat() override
            {
                return static_cast<float>(getDouble());
            }

        private:

            inline void clearCache()
            {
                m_arrayCache.clear();
                m_arrayPosCache.clear();
                m_arrayListDataCache.clear();
            }

            //Owner values
            char *m_endptr;
            std::unique_ptr<json11::Json> m_data = nullptr; //Only used if this is the owner json!

            const json11::Json *m_json = nullptr;
            fs::path m_path;

            //Cache!
            std::map<std::string, std::unique_ptr<IJson>> m_arrayCache;
            std::map<size_t, std::unique_ptr<IJson>> m_arrayPosCache;
            std::map<std::string, std::vector<std::unique_ptr<IJson>>> m_arrayListDataCache;

    };
}

#endif //TILESON_JSON11_HPP
