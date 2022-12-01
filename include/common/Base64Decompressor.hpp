//
// Created by robin on 29.07.2020.
// The Base64 decoding logic is heavily based on: https://github.com/ReneNyffenegger/cpp-base64
//

#ifndef TILESON_BASE64DECOMPRESSOR_HPP
#define TILESON_BASE64DECOMPRESSOR_HPP

#include "../interfaces/IDecompressor.hpp"
#include <string>

namespace tson
{
    class Base64Decompressor : public IDecompressor<std::string_view, std::string>
    {
        public:
            [[nodiscard]] inline const std::string &name() const override;

            inline std::string decompress(const std::string_view &s) override;

            inline std::string decompressFile(const fs::path &path) override;
            inline std::string decompress(const void *data, size_t size) override;

        private:
            inline unsigned int pos_of_char(const unsigned char chr);
            inline static const std::string NAME = "base64";
    };

    const std::string &Base64Decompressor::name() const
    {
        return NAME;
    }

    std::string Base64Decompressor::decompress(const std::string_view &s)
    {

        size_t length_of_string = s.length();
        if (!length_of_string) return std::string("");

        size_t in_len = length_of_string;
        size_t pos = 0;

        //
        // The approximate length (bytes) of the decoded string might be one ore
        // two bytes smaller, depending on the amount of trailing equal signs
        // in the encoded string. This approximation is needed to reserve
        // enough space in the string to be returned.
        //
        size_t approx_length_of_decoded_string = length_of_string / 4 * 3;
        std::string ret;
        ret.reserve(approx_length_of_decoded_string);

        while (pos < in_len) {

            unsigned int pos_of_char_1 = pos_of_char(s[pos+1] );

            ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char(s[pos+0]) ) << 2 ) + ( (pos_of_char_1 & 0x30 ) >> 4)));

            if (s[pos+2] != '=' && s[pos+2] != '.') { // accept URL-safe base 64 strings, too, so check for '.' also.

                unsigned int pos_of_char_2 = pos_of_char(s[pos+2] );
                ret.push_back(static_cast<std::string::value_type>( (( pos_of_char_1 & 0x0f) << 4) + (( pos_of_char_2 & 0x3c) >> 2)));

                if (s[pos+3] != '=' && s[pos+3] != '.') {
                    ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char_2 & 0x03 ) << 6 ) + pos_of_char(s[pos+3])   ));
                }
            }

            pos += 4;
        }

        return ret;
    }

    unsigned int Base64Decompressor::pos_of_char(const unsigned char chr)
    {
        //
        // Return the position of chr within base64_encode()
        //

        if      (chr >= 'A' && chr <= 'Z') return chr - 'A';
        else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A')               + 1;
        else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
        else if (chr == '+' || chr == '-') return 62; // Be liberal with input and accept both url ('-') and non-url ('+') base 64 characters (
        else if (chr == '/' || chr == '_') return 63; // Ditto for '/' and '_'

        throw "If input is correct, this line should never be reached.";
    }

    /*!
     * UNUSED! Does nothing
     * @param path
     * @return
     */
    std::string Base64Decompressor::decompressFile(const fs::path &)
    {
        return std::string();
    }

    /*!
     * UNUSED! Does nothing
     * @param path
     * @return
     */
    std::string Base64Decompressor::decompress(const void *, size_t)
    {
        return std::string();
    }
}


#endif //TILESON_BASE64DECOMPRESSOR_HPP
