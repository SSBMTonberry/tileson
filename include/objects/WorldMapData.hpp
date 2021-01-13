//
// Created by robin on 01.08.2020.
//

#ifndef TILESON_WORLDMAPDATA_HPP
#define TILESON_WORLDMAPDATA_HPP

namespace tson
{
    class WorldMapData
    {
        public:
            inline WorldMapData(const fs::path &folder_, IJson &json);
            inline void parse(const fs::path &folder_, IJson &json);
            //inline WorldMapData(fs::path folder_, std::string fileName_) : folder {std::move(folder_)}, fileName {fileName_}
            //{
            //    path = folder / fileName;
            //}

            fs::path folder;
            fs::path path;
            std::string fileName;
            tson::Vector2i size;
            tson::Vector2i position;
    };

    WorldMapData::WorldMapData(const fs::path &folder_, IJson &json)
    {
        parse(folder_, json);
    }

    void WorldMapData::parse(const fs::path &folder_, IJson &json)
    {
        folder = folder_;
        if(json.count("fileName") > 0) fileName = json["fileName"].get<std::string>();
        if(json.count("height") > 0) size = {json["width"].get<int>(), json["height"].get<int>()};
        if(json.count("x") > 0) position = {json["x"].get<int>(), json["y"].get<int>()};

        path = (!fileName.empty()) ? folder / fileName : folder;
    }
}

#endif //TILESON_WORLDMAPDATA_HPP