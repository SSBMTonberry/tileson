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

            virtual IJson& operator[](std::string_view key) = 0;
            virtual IJson &at(std::string_view key) = 0;
            virtual IJson &at(size_t pos) = 0;
            /*!
             * If current json object is an array, this will get all elements of it!
             * @return An array
             */
            [[nodiscard]] virtual std::vector<std::unique_ptr<IJson>> array() = 0;
            [[nodiscard]] virtual std::vector<std::unique_ptr<IJson>> &array(std::string_view key) = 0;
            /*!
             * Get the size of an object. This will be equal to the number of
             * variables an object contains.
             * @return
             */
            [[nodiscard]] virtual size_t size() const = 0;
            [[nodiscard]] virtual bool parse(const fs::path &path) = 0;
            [[nodiscard]] virtual bool parse(const void *data, size_t size) = 0;

            template <typename T>
            [[nodiscard]] T get(std::string_view key);
            template <typename T>
            [[nodiscard]] T get();
            [[nodiscard]] virtual size_t count(std::string_view key) const = 0;
            [[nodiscard]] virtual bool any(std::string_view key) const = 0;
            [[nodiscard]] virtual bool isArray() const = 0;
            [[nodiscard]] virtual bool isObject() const = 0;
            [[nodiscard]] virtual bool isNull() const = 0;

            /*!
             * Get the directory where the json was loaded.
             * Only assigned if json is parsed by file.
             * @return
             */
            [[nodiscard]] virtual fs::path directory() const = 0;
            virtual void directory(const fs::path &directory) = 0;


        protected:
            [[nodiscard]] virtual int32_t getInt32(std::string_view key) = 0;
            [[nodiscard]] virtual uint32_t getUInt32(std::string_view key) = 0;
            [[nodiscard]] virtual int64_t getInt64(std::string_view key) = 0;
            [[nodiscard]] virtual uint64_t getUInt64(std::string_view key) = 0;
            [[nodiscard]] virtual double getDouble(std::string_view key) = 0;
            [[nodiscard]] virtual float getFloat(std::string_view key) = 0;
            [[nodiscard]] virtual std::string getString(std::string_view key) = 0;
            [[nodiscard]] virtual bool getBool(std::string_view key) = 0;

            [[nodiscard]] virtual int32_t getInt32() = 0;
            [[nodiscard]] virtual uint32_t getUInt32() = 0;
            [[nodiscard]] virtual int64_t getInt64() = 0;
            [[nodiscard]] virtual uint64_t getUInt64() = 0;
            [[nodiscard]] virtual double getDouble() = 0;
            [[nodiscard]] virtual float getFloat() = 0;
            [[nodiscard]] virtual std::string getString() = 0;
            [[nodiscard]] virtual bool getBool() = 0;
    };

    template<typename T>
    T IJson::get(std::string_view key)
    {
        if constexpr (std::is_same<T, double>::value)
            return getDouble(key);
        if constexpr (std::is_same<T, float>::value)
            return getFloat(key);
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

    template<typename T>
    T IJson::get()
    {
        if constexpr (std::is_same<T, double>::value)
            return getDouble();
        if constexpr (std::is_same<T, float>::value)
            return getFloat();
        else if constexpr (std::is_same<T, int32_t>::value)
            return getInt32();
        else if constexpr (std::is_same<T, uint32_t>::value)
            return getUInt32();
        else if constexpr (std::is_same<T, int64_t>::value)
            return getInt64();
        else if constexpr (std::is_same<T, uint64_t>::value)
            return getUInt64();
        else if constexpr (std::is_same<T, std::string>::value)
            return getString();
        else if constexpr (std::is_same<T, bool>::value)
            return getBool();
        else
            return nullptr;
    }

}

#endif //TILESON_IJSON_HPP
