//
// Created by robin on 06.06.22.
//

#ifndef TILESON_TILEDCLASS_HPP
#define TILESON_TILEDCLASS_HPP

namespace tson
{
    class TiledClass
    {
        public:
            inline explicit TiledClass(IJson &json);

            [[nodiscard]] inline uint32_t getId() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline const std::string &getType() const;
            [[nodiscard]] inline const PropertyCollection &getMembers() const;

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property *getMember(const std::string &name);

        private:
            uint32_t m_id {};
            std::string m_name {};
            std::string m_type {};
            PropertyCollection m_members {};

    };

    TiledClass::TiledClass(IJson &json)
    {
        if(json.count("id") > 0)
            m_id = json["id"].get<uint32_t>();

        if(json.count("name") > 0)
            m_name = json["name"].get<std::string>();
        if(json.count("type") > 0)
            m_type = json["type"].get<std::string>();

        if(json.count("members") > 0 && json["members"].isArray())
        {
            auto &array = json.array("members");
            std::for_each(array.begin(), array.end(), [&](std::unique_ptr<IJson> &item)
            {
                m_members.add(*item);
            });
        }
    }

    uint32_t TiledClass::getId() const
    {
        return m_id;
    }

    const std::string &TiledClass::getName() const
    {
        return m_name;
    }

    const std::string &TiledClass::getType() const
    {
        return m_type;
    }

    const PropertyCollection &TiledClass::getMembers() const
    {
        return m_members;
    }

    template<typename T>
    T TiledClass::get(const std::string &name)
    {
        return m_members.getValue<T>(name);
    }

    tson::Property *TiledClass::getMember(const std::string &name)
    {
        if(m_members.hasProperty(name))
            return m_members.getProperty(name);
        return nullptr;
    }
}

#endif //TILESON_TILEDCLASS_HPP
