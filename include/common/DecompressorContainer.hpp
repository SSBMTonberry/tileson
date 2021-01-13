//
// Created by robin on 30.07.2020.
//

#ifndef TILESON_DECOMPRESSORCONTAINER_HPP
#define TILESON_DECOMPRESSORCONTAINER_HPP

#include "../interfaces/IDecompressor.hpp"
#include <memory>
#include <vector>
#include <string_view>
#include <functional>
namespace tson
{
    class DecompressorContainer
    {
        public:
            inline DecompressorContainer() = default;
            template <typename T, typename... Args>
            inline void add(Args &&... args);
            inline void remove(std::string_view name);
            inline bool contains(std::string_view name) const;
            inline bool empty() const;
            inline size_t size() const;
            inline void clear();

            inline IDecompressor<std::string_view, std::string> *get(std::string_view name);
        private:
            //Key: name,
            std::vector<std::unique_ptr<IDecompressor<std::string_view, std::string>>> m_decompressors;
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
    bool DecompressorContainer::contains(std::string_view name) const
    {
        auto iter = std::find_if(m_decompressors.begin(), m_decompressors.end(), [&](const auto &item)
        {
            return item->name() == name;
        });

        return iter != m_decompressors.end();
    }

    /*!
     * Removed an element with the given name.
     * @param name The name of the decompressor
     */
    void DecompressorContainer::remove(std::string_view name)
    {
        auto iter = std::remove_if(m_decompressors.begin(), m_decompressors.end(), [&](const auto &item)
        {
            return item->name() == name;
        });
        m_decompressors.erase(iter);
    }

    size_t DecompressorContainer::size() const
    {
        return m_decompressors.size();
    }

    /*!
     *
     * @param name The name of the container
     * @return An ICompressor pointer if it exists. nullptr otherwise.
     */
    IDecompressor<std::string_view, std::string> *DecompressorContainer::get(std::string_view name)
    {
        auto iter = std::find_if(m_decompressors.begin(), m_decompressors.end(), [&](const auto &item)
        {
            return item->name() == name;
        });

        return (iter != m_decompressors.end()) ? iter->get() : nullptr;
    }

    /*!
     * Check if container is empty
     * @return Whether or not the container is empty
     */
    bool DecompressorContainer::empty() const
    {
        return m_decompressors.empty();
    }

    /*!
     * Clears all IDecompressor elements in the container
     */
    void DecompressorContainer::clear()
    {
        m_decompressors.clear();
    }
}
#endif //TILESON_DECOMPRESSORCONTAINER_HPP
