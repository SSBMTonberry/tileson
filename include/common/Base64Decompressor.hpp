//
// Created by robin on 29.07.2020.
// The Base64 decoding logic is heavily based on: https://github.com/ReneNyffenegger/cpp-base64
//

#ifndef TILESON_BASE64DECOMPRESSOR_HPP
#define TILESON_BASE64DECOMPRESSOR_HPP

#include "IDecompressor.h"
#include <string>

class Base64Decompressor : public IDecompressor
{
    public:
        [[nodiscard]] inline const std::string &name() const override;
        inline std::string decompress(std::string_view s) override;

    private:
        inline static const std::string NAME = "base64";
};

const std::string &Base64Decompressor::name() const
{
    //static const std::string NAME = "base64";
    return NAME;
}

std::string Base64Decompressor::decompress(std::string_view s)
{
    return std::string();
}

#endif //TILESON_BASE64DECOMPRESSOR_HPP
