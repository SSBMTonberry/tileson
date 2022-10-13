//
// Created by robin on 31.07.2020.
//

#ifndef TILESON_TOOLS_HPP
#define TILESON_TOOLS_HPP

#include <cstdint>
#include <vector>
#include <string_view>
namespace tson
{
    class Tools
    {

        public:
            Tools() = delete;
            ~Tools() = delete;
            inline static std::vector<uint8_t> Base64DecodedStringToBytes(std::string_view str);
            inline static std::vector<uint32_t> BytesToUnsignedInts(const std::vector<uint8_t> &bytes);
            inline static std::vector<std::string> SplitString(const std::string &s, char delim);
            inline static bool Equal(float a, float b, float precision = 8192.f);

        private:
            template<typename Out>
            static void split(const std::string &s, char delim, Out result)
            {
                std::stringstream ss;
                ss.str(s);
                std::string item;

                while (std::getline(ss, item, delim))
                {
                    *(result++) = item;
                }
            }
    };

    /*!
     * When you have decoded a Base64 string, you'll get a string representing bytes. This function turns them into actual bytes.
     * @param str
     * @return
     */
    std::vector<uint8_t> Tools::Base64DecodedStringToBytes(std::string_view str)
    {
        std::vector<uint8_t> bytes;
        for(size_t i = 0; i < str.size(); ++i)
        {
            uint8_t u8 = static_cast<uint8_t>(str[i]);
            bytes.push_back(u8);
        }
        return bytes;
    }

    /*!
     * Converts bytes into unsigned int values. The bytes are converted in the Little Endian byte order to fit Tiled's specs.
     * @param bytes A vector of bytes.
     * @return Bytes converted to unsigned ints
     */
    std::vector<uint32_t> Tools::BytesToUnsignedInts(const std::vector<uint8_t> &bytes)
    {
        std::vector<uint32_t> uints;
        std::vector<uint8_t> toConvert;
        //uint32_t size8 = (compressed[55] << 24) | (compressed[56] << 16) | (compressed[57] << 8) | compressed[58]; //Should be 66000

        for(size_t i = 0; i < bytes.size(); ++i)
        {
            toConvert.push_back(bytes[i]);
            if(toConvert.size() == 4)
            {
                uint32_t u32 = (toConvert[3] << 24) | (toConvert[2] << 16) | (toConvert[1] << 8) | toConvert[0];
                uints.push_back(u32);
                toConvert.clear();
            }
        }

        return uints;
    }

    std::vector<std::string> Tools::SplitString(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    /*!
     * Uses a threshold for comparing floats, as they are not precise in many cases.
     * @param a
     * @param b
     * @return true if equal based on the currently defined precision
     */
    bool Tools::Equal(float a, float b, float precision)
    {
        float threshold = 1.f / precision;
        float diff = fabsf(a - b);
        return diff <= threshold;
    }
}

#endif //TILESON_TOOLS_HPP
