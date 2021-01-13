//
// Created by robin on 08.01.2021.
//

#ifndef TILESON_IJSONFAKES_HPP
#define TILESON_IJSONFAKES_HPP


class FakeSon : public tson::IJson
{
    public:
        //IJson &operator[](std::string_view key) override
        //{
        //    return *this;
        //}
        std::unique_ptr<IJson> at(std::string_view key) override
        {
            return std::unique_ptr<FakeSon>();
        }

        std::unique_ptr<IJson> at(size_t pos) override
        {
            return std::unique_ptr<FakeSon>();
        }

        std::vector<std::unique_ptr<IJson>> array(std::string_view key) override
        {
            return std::vector<std::unique_ptr<IJson>>();
        }

        [[nodiscard]] size_t size() const override
        {
            return 1;
        }

        bool parse(const fs::path &path) override
        {
            return false;
        }

        bool parse(const void *data, size_t size) override
        {
            return false;
        }

        [[nodiscard]] size_t count(std::string_view key) const override
        {
            return (key == "test") ? 1 : 0;
        }

        [[nodiscard]] bool any(std::string_view key) const override
        {
            return (key == "test");
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
            return (key == "test") ? -32 : -1;
        }

        [[nodiscard]] uint32_t getUInt32(std::string_view key) const override
        {
            return (key == "test") ? 32 : 0;
        }

        [[nodiscard]] int64_t getInt64(std::string_view key) const override
        {
            return (key == "test") ? -64 : -1;
        }

        [[nodiscard]] uint64_t getUInt64(std::string_view key) const override
        {
            return (key == "test") ? 64 : 0;
        }

        [[nodiscard]] double getDouble(std::string_view key) const override
        {
            return (key == "test") ? 1337.1337 : 1.0;
        }

        [[nodiscard]] std::string getString(std::string_view key) const override
        {
            return (key == "test") ? m_str : m_str2;
        }

        [[nodiscard]] bool getBool(std::string_view key) const override
        {
            return (key == "test");
        }

    private:
        std::string m_str {"Test"};
        std::string m_str2 {"No value"};
};

#endif //TILESON_IJSONFAKES_HPP
