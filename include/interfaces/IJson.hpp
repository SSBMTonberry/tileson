//
// Created by robin on 06.01.2021.
//

#ifndef TILESON_IJSON_HPP
#define TILESON_IJSON_HPP

namespace tson
{
    class IJson
    {
        public:
            [[nodiscard]] virtual bool parse(const void *data, size_t size) = 0;

            template <typename T>
            [[nodiscard]] T get(std::string_view key) const;
            [[nodiscard]] virtual size_t count(std::string_view key) const = 0;
            [[nodiscard]] virtual bool any(std::string_view key) const = 0;
            [[nodiscard]] virtual bool isArray() const = 0;
            [[nodiscard]] virtual bool isObject() const = 0;
            [[nodiscard]] virtual bool isNull() const = 0;

        protected:
            [[nodiscard]] virtual int32_t getInt32(std::string_view key) const = 0;
            [[nodiscard]] virtual uint32_t getUInt32(std::string_view key) const = 0;
            [[nodiscard]] virtual int64_t getInt64(std::string_view key) const = 0;
            [[nodiscard]] virtual uint64_t getUInt64(std::string_view key) const = 0;

            [[nodiscard]] virtual double getDouble(std::string_view key) const = 0;
            [[nodiscard]] virtual const std::string &getString(std::string_view key) const = 0;
            [[nodiscard]] virtual bool getBool(std::string_view key) const = 0;
    };

    template<typename T>
    T IJson::get(std::string_view key) const
    {
        if constexpr (std::is_same<T, double>::value)
            return getDouble(key);
        else if constexpr (std::is_same<T, int32_t>::value)
            return getInt32(key);
        else if constexpr (std::is_same<T, uint32_t>::value)
            return getUInt32(key);
        else if constexpr (std::is_same<T, int64_t>::value)
            return getInt64(key);
        else if constexpr (std::is_same<T, uint64_t>::value)
            return getUInt64(key);
        else if constexpr (std::is_same<T, std::string>::value)
            return getString(key);
        else if constexpr (std::is_same<T, bool>::value)
            return getBool(key);
        else
            return nullptr;
    }

}

#endif //TILESON_IJSON_HPP
