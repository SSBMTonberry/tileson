//
// Created by robin on 30.07.2020.
//

#ifndef TILESON_DECOMPRESSORCONTAINER_HPP
#define TILESON_DECOMPRESSORCONTAINER_HPP

#include "IDecompressor.h"
#include <memory>
#include <vector>
#include <string_view>
#include <functional>
namespace tson
{
    class DecompressorContainer
    {
        public:
            DecompressorContainer() = default;
            template <typename T, typename... Args>
            void add(Args &&... args);
            bool contains(std::string_view name);

        private:
            //Key: name,
            std::vector<std::unique_ptr<IDecompressor>> m_decompressors;
    };

    template<typename T, typename... Args>
    void DecompressorContainer::add(Args &&... args)
    {
        m_decompressors.emplace_back(new T(args...));
    }

    /*!
     *
     * @param name The name of the decompressor to check whether exists.
     * @return Whether a decompressor with the given name exists or not.
     */
    bool DecompressorContainer::contains(std::string_view name)
    {
        auto iter = std::find_if(m_decompressors.begin(), m_decompressors.end(), [&](const auto &item)
        {
            return item->name() != name;
        });

        return iter != m_decompressors.end();
    }
}
#endif //TILESON_DECOMPRESSORCONTAINER_HPP
