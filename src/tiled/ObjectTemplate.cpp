//
// Created by robin on 03.08.2019.
//

#include "ObjectTemplate.h"

tson::ObjectTemplate::ObjectTemplate(const nlohmann::json &json)
{
    parse(json);
}

bool tson::ObjectTemplate::parse(const nlohmann::json &json)
{
    return false;
}
