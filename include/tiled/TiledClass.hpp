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
            inline explicit TiledClass() = default;
            inline explicit TiledClass(IJson &json, tson::Project *project = nullptr);

            [[nodiscard]] inline uint32_t getId() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline const std::string &getType() const;
            [[nodiscard]] inline PropertyCollection &getMembers();
            inline void update(IJson &json);
            inline void update(PropertyCollection &properties);

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property *getMember(const std::string &name);

        private:
            uint32_t m_id {};
            std::string m_name {};
            std::string m_type {};
            PropertyCollection m_members {};

    };

    TiledClass::TiledClass(IJson &json, tson::Project *project)
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
                m_members.add(*item, project);
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

    PropertyCollection &TiledClass::getMembers()
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

    /*!
     * Takes a json object from a particular map top update values if they differ from the original values of the class
     * @param json
     */
    void TiledClass::update(IJson &json)
    {
        for(Property *property : m_members.get())
        {
            if(json.any(property->getName()))
            {
                property->setValueByType(json[property->getName()]);
            }
        }
    }

    void TiledClass::update(PropertyCollection &properties)
    {
        std::vector<Property *> toUpdate;
        for(Property *member : m_members.get())
        {
            if(properties.hasProperty(member->getName()))
            {
                Property *property = properties.getProperty(member->getName());
                if(member->getType() == property->getType())
                {
                    toUpdate.push_back(property);
                }
            }
        }

        std::for_each(toUpdate.begin(), toUpdate.end(), [&](Property *p)
        {
           m_members.setProperty(p->getName(), *p);
        });
    }
}

#endif //TILESON_TILEDCLASS_HPP
