Also see the release notes of version 1.3.0 alpha to get the complete v1.3.0 release details.

## Features

- Tiled 1.9 support ([#68](https://github.com/SSBMTonberry/tileson/issues/68))
- Tiled 1.8 support ([#60](https://github.com/SSBMTonberry/tileson/issues/60))
  - Class and enum properties: When having maps related to a `Project`, you can use the `class` property with your own class definitions. Same goes for `Tiled` `enums``.
```c++
fs::path pathToUse = fs::path("path/to/project.tiled-project");
tson::Project project{pathToUse};
auto folderFiles = project.getFolders().at(0).getFiles();

for(fs::path &f: folderFiles)
{
    fs::path path = project.getFolders().at(0).getPath() / f.filename();
    std::string filename = f.filename().generic_string();
    if(filename == "map1.json")
    {
        tson::Tileson t{&project};
        std::unique_ptr<tson::Map> m = t.parse(path);
        tson::Layer *objectLayer = m->getLayer("Da Object Layer");

        //Get class from object
        tson::TiledClass *objectClass = objectLayer->firstObj("TestObject")->getClass(); 
        
        //Asserts as example how to use members
        REQUIRE(objectClass != nullptr);
        REQUIRE(objectClass->getName() == "Enemy");
        REQUIRE(objectClass->get<int>("hp") == 10);
        REQUIRE(objectClass->get<std::string>("name") == "Galderino");

        //Get class from tile
        tson::Tile *tile = m->getTileset("demo-tileset")->getTile(1);
        tson::TiledClass *tileClass = tile->getClass();

        //Example how to get member of different types with asserts
        REQUIRE(objectClass->getMember("Age")->getType() == tson::Type::Int);
        REQUIRE(objectClass->getMember("Age")->getValue<int>() == 49);
        REQUIRE(objectClass->get<int>("Age") == 49);
        REQUIRE(objectClass->getMember("CanDestroy")->getType() == tson::Type::Boolean);
        REQUIRE(objectClass->get<bool>("CanDestroy"));
        REQUIRE(objectClass->getMember("ExtraFile")->getType() == tson::Type::File);
        REQUIRE(objectClass->get<fs::path>("ExtraFile") == fs::path("../ultimate_test.json"));
        REQUIRE(objectClass->getMember("MoneyInBag")->getType() == tson::Type::Float);
        REQUIRE(tson::Tools::Equal(objectClass->get<float>("MoneyInBag"), 16.9344f));
        REQUIRE(objectClass->getMember("MyObject")->getType() == tson::Type::Object);
        REQUIRE(objectClass->get<uint32_t>("MyObject") == 39);
        REQUIRE(objectClass->getMember("Name")->getType() == tson::Type::String);
        REQUIRE(objectClass->get<std::string>("Name") == "James Testolini");
        REQUIRE(objectClass->getMember("ShoeColor")->getType() == tson::Type::Color);
        tson::Colori color = objectClass->get<tson::Colori>("ShoeColor");
        REQUIRE(color == "#ff069504");
        REQUIRE(color.a == 0xff);
        REQUIRE(color.r == 0x06);
        REQUIRE(color.g == 0x95);
        REQUIRE(color.b == 0x04);

        //Example of different enum properties stored within objects
        //Numeric and string based enums with and without flag properties
        tson::Object *enumObj = objectLayer->firstObj("TestObjectEnum");
        tson::TiledClass *objectClassEnum = enumObj->getClass(); //Object is changed from default values
        tson::TiledClass *tileClassEnum = tileClass;
        
        REQUIRE(enumObj->getProp("num_enum") != nullptr);
        tson::EnumValue objPropNumEnum = enumObj->get<tson::EnumValue>("num_enum");
        REQUIRE(enumObj->getProp("num_enum_flags") != nullptr);
        tson::EnumValue objPropNumEnumFlags = enumObj->get<tson::EnumValue>("num_enum_flags");
        REQUIRE(enumObj->getProp("str_enum") != nullptr);
        tson::EnumValue objPropStrEnum = enumObj->get<tson::EnumValue>("str_enum");
        REQUIRE(enumObj->getProp("str_enum_flags") != nullptr);
        tson::EnumValue objPropStrEnumFlags = enumObj->get<tson::EnumValue>("str_enum_flags");


        REQUIRE(objPropNumEnum.getValue() == 3); 
        REQUIRE(objPropNumEnum.getValueName() == "GetNumber"); 
        REQUIRE(objPropNumEnumFlags.getValue() == 9); 
        //Flags enums (numeric and string) may use custom enum classes, as long as they have applied flags logic applied to them. See details how this can be achieved below this code example
        REQUIRE(objPropNumEnumFlags.hasFlag(tson::TestEnumNumberFlags::HasCalculatorFlag | tson::TestEnumNumberFlags::HasInvisibilityFlag)); 
        REQUIRE(objPropStrEnum.getValue() == 2); 
        REQUIRE(objPropStrEnum.getValueName() == "DeletePlayer"); 
        REQUIRE(objPropStrEnumFlags.getValue() == 6); 
        REQUIRE(objPropStrEnumFlags.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));

        //Another example with flags more in depth
        tson::EnumValue numEnumC2 = someClass.getMember("NumFlag")->getValue<tson::EnumValue>();
        tson::EnumValue strEnumC2 = someClass.getMember("StrFlag")->getValue<tson::EnumValue>(); //Not used here, but will work in the same way

        REQUIRE(someClass.getMember("NumFlag")->getType() == tson::Type::Enum);
        REQUIRE(numEnumC2.getValue() == 10);
        REQUIRE(numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasInvisibilityFlag)); //Has both these flags - OK
        REQUIRE(numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag)); // Has this flag - OK
        REQUIRE(numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasInvisibilityFlag)); // Has this flag - OK
        REQUIRE(numEnumC2.hasAnyFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasHumorFlag)); //hasAnyFlag is okay as long as one of the flags here are set
        REQUIRE(!numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag)); //Doesn't have this flag - OK
    }
}
```
  

- Added `quom` as amalgamate tool for `OSX` ([#82](https://github.com/SSBMTonberry/tileson/issues/82)) - Thanks to [dmlary](https://github.com/dmlary)
- Now using `Github Actions` instead of `Travis` for CI ([#50](https://github.com/SSBMTonberry/tileson/issues/50)) - Thanks to [Laguna1989](https://github.com/Laguna1989)
- C++20 support ([#53](https://github.com/SSBMTonberry/tileson/issues/53)) - Thanks to [gamecoder-nz](https://github.com/gamecoder-nz)
  

## Improvements
- Only include `external_libs` folder if examples or tests are required ([#96](https://github.com/SSBMTonberry/tileson/pull/96)) - Thanks to [Laguna1989](https://github.com/Laguna1989)
- Tests are now stricter and treats warnings as errors ([#90](https://github.com/SSBMTonberry/tileson/pull/90)) - Thanks to [dmlary](https://github.com/dmlary)
- CI improvements: Added `MacOS`, separated CI by system and added Clang 12 and 13 support on Linux ([#88](https://github.com/SSBMTonberry/tileson/pull/88))  
- Fixed some Apple Clang 13 compile warnings ([#84](https://github.com/SSBMTonberry/tileson/issues/84)) - Thanks to [dmlary](https://github.com/dmlary)
- Updated Catch2 to support `GCC 11.2` ([#59](https://github.com/SSBMTonberry/tileson/issues/59))
- Added missing properties to `tson::Text` ([#75](https://github.com/SSBMTonberry/tileson/issues/75))


## Breaking changes
- Should be none.

## Bug-fixes
- Fixed bugs related to not being able to resolve `TiledEnum`s in certain contexts ([#98](https://github.com/SSBMTonberry/tileson/pull/98))
- Tile properties should now be properly loaded when using multiple tilesets. ([#54](https://github.com/SSBMTonberry/tileson/issues/54)) - Thanks to [Laguna1989](https://github.com/Laguna1989)
- Added missing virtual destructor to IJson and IDecompressor. ([#47](https://github.com/SSBMTonberry/tileson/issues/47)) - Thanks to [matthew-nagy](https://github.com/matthew-nagy)

## Misc









