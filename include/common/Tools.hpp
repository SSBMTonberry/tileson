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
            //inline static std::vector<int> BytesToInts(const std::vector<uint8_t> &bytes);
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

    /*!
     * While the Tiled specification uses unsigned ints for their tiles, Tileson uses regular ints.
     * This may be changed in the future, but should in reality never really become an issue.
     *
     * Update 2020-11-09: This will cause problems when tiles has flip flags!
     *
     * int differences:
     * int max:  2147483647
     * uint max: 4294967295
     *
     * @param bytes A vector of bytes.
     * @return Bytes converted to ints
     */
    /*std::vector<int> Tools::BytesToInts(const std::vector<uint8_t> &bytes)
    {
        std::vector<int> ints;
        std::vector<uint8_t> toConvert;
        //uint32_t size8 = (compressed[55] << 24) | (compressed[56] << 16) | (compressed[57] << 8) | compressed[58]; //Should be 66000

        for(size_t i = 0; i < bytes.size(); ++i)
        {
            toConvert.push_back(bytes[i]);
            if(toConvert.size() == 4)
            {
                uint32_t u32 = (toConvert[3] << 24) | (toConvert[2] << 16) | (toConvert[1] << 8) | toConvert[0];
                ints.push_back(u32);
                toConvert.clear();
            }
        }

        return ints;
    }*/
}

#endif //TILESON_TOOLS_HPP
