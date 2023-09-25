//
// Created by robin on 14.01.2021.
//

#ifndef TILESON_GASON_HPP
#define TILESON_GASON_HPP

namespace tson
{
    class Gason : public tson::IJson
    {
        public:
            inline Gason() = default;

            IJson &operator[](std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                {
                    if(m_json.getTag() == gason::JSON_OBJECT)
                    {
                        m_arrayCache[key.data()] = std::make_unique<Gason>(m_objectCache[key.data()]);
                    }
                }

                return *m_arrayCache[key.data()].get();
            }

            inline explicit Gason(gason::JsonValue json) : m_json {json}
            {
                createObjCache();
            }

            inline IJson& at(std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                {
                    if(m_json.getTag() == gason::JSON_OBJECT)
                    {
                        m_arrayCache[key.data()] = std::make_unique<Gason>(m_objectCache[key.data()]);
                    }
                }
                return *m_arrayCache[key.data()].get();
            }

            inline IJson& at(size_t pos) override
            {
                if(m_arrayPosCache.count(pos) == 0)
                {
                    size_t i = 0;
                    for (auto item : m_json)
                    {
                        if(i == pos)
                        {
                            m_arrayPosCache[pos] = std::make_unique<Gason>(item->value);
                            break;
                        }
                    }
                }

                return *m_arrayPosCache[pos];
            }

            std::vector<std::unique_ptr<IJson>> array() override
            {
                std::vector<std::unique_ptr<IJson>> vec;
                if(m_json.getTag() == gason::JSON_ARRAY)
                {
                    for (auto item : m_json)
                    {
                        vec.emplace_back(std::make_unique<Gason>(item->value));
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
                            gason::JsonValue v = m_objectCache[key.data()];
                            if(v.getTag() == gason::JSON_ARRAY)
                            {
                                for (auto item : v)
                                {
                                    m_arrayListDataCache[key.data()].emplace_back(std::make_unique<Gason>(item->value));
                                }
                            }
                        }
                    }
                }


                return m_arrayListDataCache[key.data()];
            }

            [[nodiscard]] inline size_t size() const override
            {

                return m_objectCache.size();
            }

            inline bool parse(const fs::path &path) override
            {
                clearCache();
                m_data = nullptr;
                if (fs::exists(path) && fs::is_regular_file(path))
                {
                    std::ifstream file(path.generic_string());
                    std::string str;
                    m_path = path.parent_path();

                    file.seekg(0, std::ios::end);
                    str.reserve(static_cast<size_t>(file.tellg()));
                    file.seekg(0, std::ios::beg);

                    str.assign((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());

                    m_data = std::make_unique<gason::JsonValue>();
                    m_allocator = std::make_unique<gason::JsonAllocator>();

                    try
                    {
                        int status = gason::jsonParse(str.data(), &m_endptr, m_data.get(), *m_allocator);
                        if (status != gason::JSON_OK) {
                            fprintf(stderr, "%s at %zd\n", gason::jsonStrError(status), m_endptr - str.data());
                            return false;
                        }
                        m_json = *m_data;
                        createObjCache();
                    }
                    catch (const std::exception &error)
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

            inline bool parse(const void *data, size_t size) override
            {
                clearCache();
                std::string str;

                str.reserve(size);

                tson::MemoryStream mem{(uint8_t *) data, size};
                //mem >> str;
                str.assign((std::istreambuf_iterator<char>(mem)),
                           std::istreambuf_iterator<char>());

                m_data = std::make_unique<gason::JsonValue>();
                m_allocator = std::make_unique<gason::JsonAllocator>();

                try
                {
                    int status = gason::jsonParse(str.data(), &m_endptr, m_data.get(), *m_allocator);
                    if (status != gason::JSON_OK) {
                        fprintf(stderr, "%s at %zd\n", gason::jsonStrError(status), m_endptr - str.data());
                        return false;
                    }
                    m_json = *m_data;
                    createObjCache();
                }
                catch (const std::exception &error)
                {
                    std::string message = "Parse error: ";
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
                    return m_objectCache.count(key.data());
                }

                return 0;
            }

            [[nodiscard]] inline bool any(std::string_view key) const override
            {
                return count(key) > 0;
            }

            [[nodiscard]] inline bool isArray() const override
            {
                return m_json.getTag() == gason::JSON_ARRAY;
            }

            [[nodiscard]] inline bool isObject() const override
            {
                return m_json.getTag() == gason::JSON_OBJECT;
            }

            [[nodiscard]] inline bool isNull() const override
            {
                return m_json.getTag() == gason::JSON_NULL;
            }

            fs::path directory() const override
            {
                return m_path;
            }

            void directory(const fs::path &directory) override
            {
                m_path = directory;
            }

            std::unique_ptr<IJson> create() override
            {
                return std::make_unique<Gason>();
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
                return obj(key.data())->toNumber();
            }

            [[nodiscard]] inline std::string getString(std::string_view key) override
            {
                return obj(key.data())->toString(); // .get<std::string>();
            }

            [[nodiscard]] inline bool getBool(std::string_view key) override
            {
                return obj(key.data())->getTag() == gason::JSON_TRUE;
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
                return m_json.toNumber();
            }

            [[nodiscard]] inline std::string getString() override
            {
                return m_json.toString();
            }

            [[nodiscard]] inline bool getBool() override
            {
                return m_json.getTag() == gason::JSON_TRUE;
            }

            [[nodiscard]] float getFloat() override
            {
                return static_cast<float>(getDouble());
            }

        private:
            /*!
             * Creates a map of all objects if not already done, then returns them.
             * @return
             */
            const gason::JsonValue *obj(std::string_view key)
            {
                //createObjCache();
                return &m_objectCache[key.data()];
            }

            void createObjCache()
            {
                if(m_objectCache.empty())
                {
                    if(m_json.getTag() == gason::JSON_OBJECT)
                    {
                        for (auto i : m_json)
                        {
                            m_objectCache[i->key] = i->value;
                        }
                    }
                }
            }

            inline void clearCache()
            {
                m_objectCache.clear();
                m_arrayCache.clear();
                m_arrayPosCache.clear();
                m_arrayListDataCache.clear();
            }

            //Owner values
            char *m_endptr;
            std::unique_ptr<gason::JsonValue> m_data = nullptr; //Only used if this is the owner json!
            std::unique_ptr<gason::JsonAllocator> m_allocator = nullptr;

            gason::JsonValue m_json;
            fs::path m_path;

            //Cache!
            std::map<std::string, gason::JsonValue> m_objectCache;

            std::map<std::string, std::unique_ptr<IJson>> m_arrayCache;
            std::map<size_t, std::unique_ptr<IJson>> m_arrayPosCache;
            std::map<std::string, std::vector<std::unique_ptr<IJson>>> m_arrayListDataCache;

    };
}

#endif //TILESON_GASON_HPP
