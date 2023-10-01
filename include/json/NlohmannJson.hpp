//
// Created by robin on 08.01.2021.
//

#ifdef INCLUDE_NLOHMANN_JSON_HPP_

#ifndef TILESON_NLOHMANNJSON_HPP
#define TILESON_NLOHMANNJSON_HPP

#include <memory>

namespace tson
{
    class NlohmannJson : public tson::IJson
    {
        public:
            inline NlohmannJson() = default;


            IJson &operator[](std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                    m_arrayCache[key.data()] = std::make_unique<NlohmannJson>(&m_json->operator[](key.data()));//.front());

                return *m_arrayCache[key.data()].get();
            }

            inline explicit NlohmannJson(nlohmann::json *json) : m_json {json}
            {

            }

            inline IJson& at(std::string_view key) override
            {
                if(m_arrayCache.count(key.data()) == 0)
                    m_arrayCache[key.data()] = std::make_unique<NlohmannJson>(&m_json->operator[](key.data()));//.front());

                return *m_arrayCache[key.data()].get();
            }

            inline IJson& at(size_t pos) override
            {
                if(m_arrayPosCache.count(pos) == 0)
                    m_arrayPosCache[pos] = std::make_unique<NlohmannJson>(&m_json->at(pos));

                return *m_arrayPosCache[pos];
            }

            std::vector<std::unique_ptr<IJson>> array() override
            {
                std::vector<std::unique_ptr<IJson>> vec;
                for(auto &item : *m_json)
                {
                    nlohmann::json *ptr = &item;
                    vec.emplace_back(std::make_unique<NlohmannJson>(ptr));
                }

                return vec;
            }

            inline std::vector<std::unique_ptr<IJson>> &array(std::string_view key) override
            {
                if(m_arrayListDataCache.count(key.data()) == 0)
                {
                    if (m_json->count(key.data()) > 0 && m_json->operator[](key.data()).is_array())
                    {
                        std::for_each(m_json->operator[](key.data()).begin(), m_json->operator[](key.data()).end(), [&](nlohmann::json &item)
                        {
                            nlohmann::json *ptr = &item;
                            m_arrayListDataCache[key.data()].emplace_back(std::make_unique<NlohmannJson>(ptr));
                        });
                    }
                }


                return m_arrayListDataCache[key.data()];
            }

            [[nodiscard]] inline size_t size() const override
            {
                return m_json->size();
            }

            inline bool parse(const fs::path &path) override
            {
                clearCache();
                m_data = nullptr;
                m_json = nullptr;
                if (fs::exists(path) && fs::is_regular_file(path))
                {
                    m_path = path.parent_path();
                    m_data = std::make_unique<nlohmann::json>();
                    std::ifstream i(path.generic_string());
                    try
                    {
                        i >> *m_data;
                        m_json = m_data.get();
                    }
                    catch (const nlohmann::json::parse_error &error)
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
                m_json = nullptr;
                m_data = std::make_unique<nlohmann::json>();
                tson::MemoryStream mem{(uint8_t *) data, size};
                try
                {
                    mem >> *m_data;
                    m_json = m_data.get();
                }
                catch (const nlohmann::json::parse_error &error)
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
                return m_json->count(key);
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

            std::unique_ptr<IJson> create() override
            {
                return std::make_unique<NlohmannJson>();
            }

        protected:
            [[nodiscard]] inline int32_t getInt32(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<int32_t>();
            }

            [[nodiscard]] inline uint32_t getUInt32(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<uint32_t>();
            }

            [[nodiscard]] inline int64_t getInt64(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<int64_t>();
            }

            [[nodiscard]] inline uint64_t getUInt64(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<uint64_t>();
            }

            [[nodiscard]] inline double getDouble(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<double>();
            }

            [[nodiscard]] inline std::string getString(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<std::string>();
            }

            [[nodiscard]] inline bool getBool(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<bool>();
            }

            [[nodiscard]] float getFloat(std::string_view key) override
            {
                return m_json->operator[](key.data()).get<float>();
            }

            [[nodiscard]] inline int32_t getInt32() override
            {
                return m_json->get<int32_t>();
            }

            [[nodiscard]] inline uint32_t getUInt32() override
            {
                return m_json->get<uint32_t>();
            }

            [[nodiscard]] inline int64_t getInt64() override
            {
                return m_json->get<int64_t>();
            }

            [[nodiscard]] inline uint64_t getUInt64() override
            {
                return m_json->get<uint64_t>();
            }

            [[nodiscard]] inline double getDouble() override
            {
                return m_json->get<double>();
            }

            [[nodiscard]] inline std::string getString() override
            {
                return m_json->get<std::string>();
            }

            [[nodiscard]] inline bool getBool() override
            {
                return m_json->get<bool>();
            }

            [[nodiscard]] float getFloat() override
            {
                return m_json->get<float>();
            }

        private:
            inline void clearCache()
            {
                m_arrayCache.clear();
                m_arrayPosCache.clear();
                m_arrayListDataCache.clear();
            }

            nlohmann::json *m_json = nullptr;
            std::unique_ptr<nlohmann::json> m_data = nullptr; //Only used if this is the owner json!
            fs::path m_path;

            //Cache!
            std::map<std::string, std::unique_ptr<IJson>> m_arrayCache;
            std::map<size_t, std::unique_ptr<IJson>> m_arrayPosCache;
            std::map<std::string, std::vector<std::unique_ptr<IJson>>> m_arrayListDataCache;

    };
}
#endif //TILESON_NLOHMANNJSON_HPP

#endif //INCLUDE_NLOHMANN_JSON_HPP_