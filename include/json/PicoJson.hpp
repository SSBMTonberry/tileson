//
// Created by robin on 11.01.2021.
//

#ifndef TILESON_PICOJSON_HPP
#define TILESON_PICOJSON_HPP

#include "../external/picojson.hpp"

namespace tson
{
    class PicoJson : public tson::IJson
    {
        public:
            inline PicoJson() = default;

            IJson &operator[](std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                {
                    if(m_json->is<picojson::object>())
                    {
                        picojson::object &o = m_json->get<picojson::object>();
                        m_arrayCache[key.data()] = std::make_unique<PicoJson>(&o[key.data()]);
                        //m_arrayCache[key.data()] = std::make_unique<PicoJson>(&m_json->operator[](key.data()));
                    }
                    //else
                    //{
                    //    bool isArray = m_json->is<picojson::array>();
//
                    //    picojson::value &v = m_json->get(key.data());
                    //    m_arrayCache[key.data()] = std::make_unique<PicoJson>(&v);
                    //}
                }

                return *m_arrayCache[key.data()].get();
            }

            inline explicit PicoJson(picojson::value *json) : m_json {json}
            {

            }

            //inline explicit PicoJson(const fs::path &path)
            //{
            //    parse(path);
            //}
//
            //inline explicit PicoJson(const void *data, size_t size)
            //{
            //    parse(data, size);
            //}

            inline IJson& at(std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                {
                    if(m_json->is<picojson::object>())
                    {
                        picojson::object &o = m_json->get<picojson::object>();
                        m_arrayCache[key.data()] = std::make_unique<PicoJson>(&o[key.data()]);
                    }
                }
                return *m_arrayCache[key.data()].get();
            }

            inline IJson& at(size_t pos) override
            {
                if(m_arrayPosCache.count(pos) == 0)
                {
                    picojson::array &a = m_json->get<picojson::array>();
                    m_arrayPosCache[pos] = std::make_unique<PicoJson>(&a.at(pos));
                }

                return *m_arrayPosCache[pos];
            }

            std::vector<std::unique_ptr<IJson>> array() override
            {
                std::vector<std::unique_ptr<IJson>> vec;
                if(m_json->is<picojson::array>())
                {
                    picojson::array &a = m_json->get<picojson::array>();
                    for (auto &item : a)
                    {
                        picojson::value *ptr = &item;
                        vec.emplace_back(std::make_unique<PicoJson>(ptr));
                    }
                }

                return vec;
            }

            inline std::vector<std::unique_ptr<IJson>> &array(std::string_view key) override
            {
                if(m_arrayListDataCache.count(key.data()) == 0)
                {
                    //if (m_json->count(key.data()) > 0 && m_json->operator[](key.data()).is_array())
                    if(count(key.data()) > 0)
                    {
                        if (isObject())
                        {
                            picojson::object &obj = m_json->get<picojson::object>();
                            picojson::value &v = obj.at(key.data());
                            bool isArray = v.is<picojson::array>();
                            if (isArray)
                            {
                                picojson::array &a = v.get<picojson::array>();

                                //std::for_each(m_json->operator[](key.data()).begin(), m_json->operator[](key.data()).end(), [&](nlohmann::json &item)
                                std::for_each(a.begin(), a.end(), [&](picojson::value &item)
                                {
                                    picojson::value *ptr = &item;
                                    m_arrayListDataCache[key.data()].emplace_back(std::make_unique<PicoJson>(ptr));
                                    //m_arrayListRefCache[key.data()].emplace_back(*m_arrayListDataCache[key.data()].at(m_arrayListDataCache[key.data()].size() - 1));
                                });
                            }
                        }
                    }
                }


                return m_arrayListDataCache[key.data()];
            }

            [[nodiscard]] inline size_t size() const override
            {
                if (m_json->is<picojson::object>())
                {
                    picojson::object obj = m_json->get<picojson::object>();
                    return obj.size();
                }
                return 0;
            }

            inline bool parse(const fs::path &path) override
            {

                if (fs::exists(path) && fs::is_regular_file(path))
                {
                    m_data = std::make_unique<picojson::value>();
                    std::ifstream i(path.u8string());
                    try
                    {
                        std::string error = picojson::parse(*m_data, i);
                        if(!error.empty())
                        {
                            std::cerr << "PicoJson error: " << error << "\n";
                            return false;
                        }
                        //i >> *m_data;
                        m_json = m_data.get();
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
                m_data = std::make_unique<picojson::value>();
                tson::MemoryStream mem{(uint8_t *) data, size};
                try
                {
                    std::string error = picojson::parse(*m_data, mem);
                    if(!error.empty())
                    {
                        std::cerr << error << "\n";
                        return false;
                    }
                    //mem >> *m_data;
                    m_json = m_data.get();
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
                    picojson::object obj = m_json->get<picojson::object>();
                    return obj.count(key.data());
                }

                return m_json->contains(key.data()) ? 1 : 0;
            }

            [[nodiscard]] inline bool any(std::string_view key) const override
            {
                return count(key) > 0;
            }

            [[nodiscard]] inline bool isArray() const override
            {
                return m_json->is<picojson::array>();
            }

            [[nodiscard]] inline bool isObject() const override
            {
                return m_json->is<picojson::object>();
            }

            [[nodiscard]] inline bool isNull() const override
            {
                return m_json->is<picojson::null>();
            }

        protected:
            [[nodiscard]] inline int32_t getInt32(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return getDouble(key);
            }

            [[nodiscard]] inline uint32_t getUInt32(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return getDouble(key);
            }

            [[nodiscard]] inline int64_t getInt64(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return getDouble(key);
            }

            [[nodiscard]] inline uint64_t getUInt64(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return getDouble(key);
            }

            [[nodiscard]] inline double getDouble(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return obj[key.data()].get<double>();
            }

            [[nodiscard]] inline std::string getString(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return obj[key.data()].get<std::string>();
            }

            [[nodiscard]] inline bool getBool(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return obj[key.data()].get<bool>();
            }

            [[nodiscard]] float getFloat(std::string_view key) const override
            {
                picojson::object obj = m_json->get<picojson::object>();
                return static_cast<float>(getDouble(key));
            }

            [[nodiscard]] inline int32_t getInt32() const override
            {
                return getDouble();
            }

            [[nodiscard]] inline uint32_t getUInt32() const override
            {
                return getDouble();
            }

            [[nodiscard]] inline int64_t getInt64() const override
            {
                return getDouble();
            }

            [[nodiscard]] inline uint64_t getUInt64() const override
            {
                return getDouble();
            }

            [[nodiscard]] inline double getDouble() const override
            {
                return m_json->get<double>();
            }

            [[nodiscard]] inline std::string getString() const override
            {
                return m_json->get<std::string>();
            }

            [[nodiscard]] inline bool getBool() const override
            {
                return m_json->get<bool>();
            }

            [[nodiscard]] float getFloat() const override
            {
                return static_cast<float>(getDouble());
            }

        private:
            picojson::value *m_json = nullptr;
            std::unique_ptr<picojson::value> m_data = nullptr; //Only used if this is the owner json!

            //Cache!
            std::map<std::string, std::unique_ptr<IJson>> m_arrayCache;
            std::map<size_t, std::unique_ptr<IJson>> m_arrayPosCache;
            std::map<std::string, std::vector<std::unique_ptr<IJson>>> m_arrayListDataCache;
            //std::map<std::string, std::vector<std::reference_wrapper<IJson>>> m_arrayListRefCache;
            //std::vector<IJson &>

    };
}

#endif //TILESON_PICOJSON_HPP
