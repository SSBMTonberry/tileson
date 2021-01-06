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
            //[[nodiscard]] virtual const std::string &name() const = 0;

    };

}

#endif //TILESON_IJSON_HPP
