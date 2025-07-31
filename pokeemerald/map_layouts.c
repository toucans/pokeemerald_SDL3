#include "map_layouts.h"
#include "tilesets.h"
#include <stddef.h>

// Total layouts: 441
MapLayout PetalburgCity_Layout = {
    .id = "LAYOUT_PETALBURG_CITY",
    .width = 30,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Petalburg,
    .border_filepath = "data/layouts/PetalburgCity/border.bin",
    .blockdata_offset = 4062748,
};

MapLayout SlateportCity_Layout = {
    .id = "LAYOUT_SLATEPORT_CITY",
    .width = 40,
    .height = 60,
    .overworld_pos_x = 200,
    .overworld_pos_y = 260,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Slateport,
    .border_filepath = "data/layouts/SlateportCity/border.bin",
    .blockdata_offset = 4064580,
};

MapLayout MauvilleCity_Layout = {
    .id = "LAYOUT_MAUVILLE_CITY",
    .width = 40,
    .height = 20,
    .overworld_pos_x = 200,
    .overworld_pos_y = 140,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mauville,
    .border_filepath = "data/layouts/MauvilleCity/border.bin",
    .blockdata_offset = 4069412,
};

MapLayout RustboroCity_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY",
    .width = 40,
    .height = 60,
    .overworld_pos_x = 0,
    .overworld_pos_y = 120,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/RustboroCity/border.bin",
    .blockdata_offset = 4071044,
};

MapLayout FortreeCity_Layout = {
    .id = "LAYOUT_FORTREE_CITY",
    .width = 40,
    .height = 20,
    .overworld_pos_x = 320,
    .overworld_pos_y = 0,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fortree,
    .border_filepath = "data/layouts/FortreeCity/border.bin",
    .blockdata_offset = 4075876,
};

MapLayout LilycoveCity_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/LilycoveCity/border.bin",
    .blockdata_offset = 4077508,
};

MapLayout MossdeepCity_Layout = {
    .id = "LAYOUT_MOSSDEEP_CITY",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mossdeep,
    .border_filepath = "data/layouts/MossdeepCity/border.bin",
    .blockdata_offset = 4083940,
};

MapLayout SootopolisCity_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY",
    .width = 60,
    .height = 60,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Sootopolis,
    .border_filepath = "data/layouts/SootopolisCity/border.bin",
    .blockdata_offset = 4090372,
};

MapLayout EverGrandeCity_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY",
    .width = 40,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_EverGrande,
    .border_filepath = "data/layouts/EverGrandeCity/border.bin",
    .blockdata_offset = 4097604,
};

MapLayout LittlerootTown_Layout = {
    .id = "LAYOUT_LITTLEROOT_TOWN",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Petalburg,
    .border_filepath = "data/layouts/LittlerootTown/border.bin",
    .blockdata_offset = 4104036,
};

MapLayout OldaleTown_Layout = {
    .id = "LAYOUT_OLDALE_TOWN",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Petalburg,
    .border_filepath = "data/layouts/OldaleTown/border.bin",
    .blockdata_offset = 4104868,
};

MapLayout DewfordTown_Layout = {
    .id = "LAYOUT_DEWFORD_TOWN",
    .width = 20,
    .height = 20,
    .overworld_pos_x = 60,
    .overworld_pos_y = 360,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Dewford,
    .border_filepath = "data/layouts/DewfordTown/border.bin",
    .blockdata_offset = 4105700,
};

MapLayout LavaridgeTown_Layout = {
    .id = "LAYOUT_LAVARIDGE_TOWN",
    .width = 20,
    .height = 20,
    .overworld_pos_x = 120,
    .overworld_pos_y = 60,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/LavaridgeTown/border.bin",
    .blockdata_offset = 4106532,
};

MapLayout FallarborTown_Layout = {
    .id = "LAYOUT_FALLARBOR_TOWN",
    .width = 20,
    .height = 20,
    .overworld_pos_x = 80,
    .overworld_pos_y = 0,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fallarbor,
    .border_filepath = "data/layouts/FallarborTown/border.bin",
    .blockdata_offset = 4107364,
};

MapLayout VerdanturfTown_Layout = {
    .id = "LAYOUT_VERDANTURF_TOWN",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mauville,
    .border_filepath = "data/layouts/VerdanturfTown/border.bin",
    .blockdata_offset = 4108196,
};

MapLayout PacifidlogTown_Layout = {
    .id = "LAYOUT_PACIFIDLOG_TOWN",
    .width = 20,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/PacifidlogTown/border.bin",
    .blockdata_offset = 4109028,
};

MapLayout Route101_Layout = {
    .id = "LAYOUT_ROUTE101",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Petalburg,
    .border_filepath = "data/layouts/Route101/border.bin",
    .blockdata_offset = 4110660,
};

MapLayout Route102_Layout = {
    .id = "LAYOUT_ROUTE102",
    .width = 50,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Petalburg,
    .border_filepath = "data/layouts/Route102/border.bin",
    .blockdata_offset = 4111492,
};

MapLayout Route103_Layout = {
    .id = "LAYOUT_ROUTE103",
    .width = 80,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Petalburg,
    .border_filepath = "data/layouts/Route103/border.bin",
    .blockdata_offset = 4113524,
};

MapLayout Route104_Layout = {
    .id = "LAYOUT_ROUTE104",
    .width = 40,
    .height = 80,
    .overworld_pos_x = 0,
    .overworld_pos_y = 180,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/Route104/border.bin",
    .blockdata_offset = 4117076,
};

MapLayout Route105_Layout = {
    .id = "LAYOUT_ROUTE105",
    .width = 40,
    .height = 80,
    .overworld_pos_x = 0,
    .overworld_pos_y = 260,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Dewford,
    .border_filepath = "data/layouts/Route105/border.bin",
    .blockdata_offset = 4123508,
};

MapLayout Route106_Layout = {
    .id = "LAYOUT_ROUTE106",
    .width = 80,
    .height = 20,
    .overworld_pos_x = 0,
    .overworld_pos_y = 340,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Dewford,
    .border_filepath = "data/layouts/Route106/border.bin",
    .blockdata_offset = 4129940,
};

MapLayout Route107_Layout = {
    .id = "LAYOUT_ROUTE107",
    .width = 60,
    .height = 20,
    .overworld_pos_x = 80,
    .overworld_pos_y = 360,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Dewford,
    .border_filepath = "data/layouts/Route107/border.bin",
    .blockdata_offset = 4133172,
};

MapLayout Route108_Layout = {
    .id = "LAYOUT_ROUTE108",
    .width = 60,
    .height = 20,
    .overworld_pos_x = 140,
    .overworld_pos_y = 360,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Slateport,
    .border_filepath = "data/layouts/Route108/border.bin",
    .blockdata_offset = 4135604,
};

MapLayout Route109_Layout = {
    .id = "LAYOUT_ROUTE109",
    .width = 40,
    .height = 63,
    .overworld_pos_x = 200,
    .overworld_pos_y = 320,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Slateport,
    .border_filepath = "data/layouts/Route109/border.bin",
    .blockdata_offset = 4138036,
};

MapLayout Route110_Layout = {
    .id = "LAYOUT_ROUTE110",
    .width = 40,
    .height = 100,
    .overworld_pos_x = 200,
    .overworld_pos_y = 160,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mauville,
    .border_filepath = "data/layouts/Route110/border.bin",
    .blockdata_offset = 4143108,
};

MapLayout Route111_Layout = {
    .id = "LAYOUT_ROUTE111",
    .width = 40,
    .height = 140,
    .overworld_pos_x = 200,
    .overworld_pos_y = 0,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mauville,
    .border_filepath = "data/layouts/Route111/border.bin",
    .blockdata_offset = 4151140,
};

MapLayout Route112_Layout = {
    .id = "LAYOUT_ROUTE112",
    .width = 40,
    .height = 60,
    .overworld_pos_x = 160,
    .overworld_pos_y = 20,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/Route112/border.bin",
    .blockdata_offset = 4162372,
};

MapLayout Route113_Layout = {
    .id = "LAYOUT_ROUTE113",
    .width = 100,
    .height = 20,
    .overworld_pos_x = 100,
    .overworld_pos_y = 0,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fallarbor,
    .border_filepath = "data/layouts/Route113/border.bin",
    .blockdata_offset = 4167204,
};

MapLayout Route114_Layout = {
    .id = "LAYOUT_ROUTE114",
    .width = 40,
    .height = 80,
    .overworld_pos_x = 40,
    .overworld_pos_y = 0,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fallarbor,
    .border_filepath = "data/layouts/Route114/border.bin",
    .blockdata_offset = 4171236,
};

MapLayout Route115_Layout = {
    .id = "LAYOUT_ROUTE115",
    .width = 40,
    .height = 80,
    .overworld_pos_x = 0,
    .overworld_pos_y = 40,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fallarbor,
    .border_filepath = "data/layouts/Route115/border.bin",
    .blockdata_offset = 4177668,
};

MapLayout Route116_Layout = {
    .id = "LAYOUT_ROUTE116",
    .width = 100,
    .height = 20,
    .overworld_pos_x = 40,
    .overworld_pos_y = 120,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/Route116/border.bin",
    .blockdata_offset = 4184100,
};

MapLayout Route117_Layout = {
    .id = "LAYOUT_ROUTE117",
    .width = 60,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mauville,
    .border_filepath = "data/layouts/Route117/border.bin",
    .blockdata_offset = 4188132,
};

MapLayout Route118_Layout = {
    .id = "LAYOUT_ROUTE118",
    .width = 80,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mauville,
    .border_filepath = "data/layouts/Route118/border.bin",
    .blockdata_offset = 4190564,
};

MapLayout Route119_Layout = {
    .id = "LAYOUT_ROUTE119",
    .width = 40,
    .height = 140,
    .overworld_pos_x = 280,
    .overworld_pos_y = 0,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fortree,
    .border_filepath = "data/layouts/Route119/border.bin",
    .blockdata_offset = 4193796,
};

MapLayout Route120_Layout = {
    .id = "LAYOUT_ROUTE120",
    .width = 40,
    .height = 100,
    .overworld_pos_x = 360,
    .overworld_pos_y = 0,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fortree,
    .border_filepath = "data/layouts/Route120/border.bin",
    .blockdata_offset = 4205028,
};

MapLayout Route121_Layout = {
    .id = "LAYOUT_ROUTE121",
    .width = 80,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/Route121/border.bin",
    .blockdata_offset = 4213060,
};

MapLayout Route122_Layout = {
    .id = "LAYOUT_ROUTE122",
    .width = 40,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/Route122/border.bin",
    .blockdata_offset = 4216292,
};

MapLayout Route123_Layout = {
    .id = "LAYOUT_ROUTE123",
    .width = 140,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/Route123/border.bin",
    .blockdata_offset = 4219524,
};

MapLayout Route124_Layout = {
    .id = "LAYOUT_ROUTE124",
    .width = 80,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mossdeep,
    .border_filepath = "data/layouts/Route124/border.bin",
    .blockdata_offset = 4225156,
};

MapLayout Route125_Layout = {
    .id = "LAYOUT_ROUTE125",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mossdeep,
    .border_filepath = "data/layouts/Route125/border.bin",
    .blockdata_offset = 4237988,
};

MapLayout Route126_Layout = {
    .id = "LAYOUT_ROUTE126",
    .width = 80,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mossdeep,
    .border_filepath = "data/layouts/Route126/border.bin",
    .blockdata_offset = 4244420,
};

MapLayout Route127_Layout = {
    .id = "LAYOUT_ROUTE127",
    .width = 80,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mossdeep,
    .border_filepath = "data/layouts/Route127/border.bin",
    .blockdata_offset = 4257252,
};

MapLayout Route128_Layout = {
    .id = "LAYOUT_ROUTE128",
    .width = 120,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mossdeep,
    .border_filepath = "data/layouts/Route128/border.bin",
    .blockdata_offset = 4270084,
};

MapLayout Route129_Layout = {
    .id = "LAYOUT_ROUTE129",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mossdeep,
    .border_filepath = "data/layouts/Route129/border.bin",
    .blockdata_offset = 4279716,
};

MapLayout Route130_MirageIsland_Layout = {
    .id = "LAYOUT_ROUTE130_MIRAGE_ISLAND",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/Route130_MirageIsland/border.bin",
    .blockdata_offset = 4286148,
};

MapLayout Route131_Layout = {
    .id = "LAYOUT_ROUTE131",
    .width = 60,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/Route131/border.bin",
    .blockdata_offset = 4292580,
};

MapLayout Route132_Layout = {
    .id = "LAYOUT_ROUTE132",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/Route132/border.bin",
    .blockdata_offset = 4297412,
};

MapLayout Route133_Layout = {
    .id = "LAYOUT_ROUTE133",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/Route133/border.bin",
    .blockdata_offset = 4303844,
};

MapLayout Route134_Layout = {
    .id = "LAYOUT_ROUTE134",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/Route134/border.bin",
    .blockdata_offset = 4310276,
};

MapLayout Underwater_Route126_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE126",
    .width = 80,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route126/border.bin",
    .blockdata_offset = 4316708,
};

MapLayout Underwater_Route127_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE127",
    .width = 80,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route127/border.bin",
    .blockdata_offset = 4329540,
};

MapLayout Underwater_Route128_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE128",
    .width = 120,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route128/border.bin",
    .blockdata_offset = 4342372,
};

MapLayout LittlerootTown_BrendansHouse_1F_Layout = {
    .id = "LAYOUT_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BrendansMaysHouse,
    .border_filepath = "data/layouts/LittlerootTown_BrendansHouse_1F/border.bin",
    .blockdata_offset = 4352004,
};

MapLayout LittlerootTown_BrendansHouse_2F_Layout = {
    .id = "LAYOUT_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F",
    .width = 9,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BrendansMaysHouse,
    .border_filepath = "data/layouts/LittlerootTown_BrendansHouse_2F/border.bin",
    .blockdata_offset = 4352236,
};

MapLayout LittlerootTown_MaysHouse_1F_Layout = {
    .id = "LAYOUT_LITTLEROOT_TOWN_MAYS_HOUSE_1F",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BrendansMaysHouse,
    .border_filepath = "data/layouts/LittlerootTown_MaysHouse_1F/border.bin",
    .blockdata_offset = 4352412,
};

MapLayout LittlerootTown_MaysHouse_2F_Layout = {
    .id = "LAYOUT_LITTLEROOT_TOWN_MAYS_HOUSE_2F",
    .width = 9,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BrendansMaysHouse,
    .border_filepath = "data/layouts/LittlerootTown_MaysHouse_2F/border.bin",
    .blockdata_offset = 4352644,
};

MapLayout LittlerootTown_ProfessorBirchsLab_Layout = {
    .id = "LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Lab,
    .border_filepath = "data/layouts/LittlerootTown_ProfessorBirchsLab/border.bin",
    .blockdata_offset = 4352820,
};

MapLayout House1_Layout = {
    .id = "LAYOUT_HOUSE1",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/House1/border.bin",
    .blockdata_offset = 4353192,
};

MapLayout House2_Layout = {
    .id = "LAYOUT_HOUSE2",
    .width = 11,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/House2/border.bin",
    .blockdata_offset = 4353404,
};

MapLayout PokemonCenter_1F_Layout = {
    .id = "LAYOUT_POKEMON_CENTER_1F",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PokemonCenter,
    .border_filepath = "data/layouts/PokemonCenter_1F/border.bin",
    .blockdata_offset = 4353612,
};

MapLayout PokemonCenter_2F_Layout = {
    .id = "LAYOUT_POKEMON_CENTER_2F",
    .width = 14,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PokemonCenter,
    .border_filepath = "data/layouts/PokemonCenter_2F/border.bin",
    .blockdata_offset = 4353896,
};

MapLayout Mart_Layout = {
    .id = "LAYOUT_MART",
    .width = 11,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/Mart/border.bin",
    .blockdata_offset = 4354208,
};

MapLayout House3_Layout = {
    .id = "LAYOUT_HOUSE3",
    .width = 10,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/House3/border.bin",
    .blockdata_offset = 4354416,
};

MapLayout DewfordTown_Gym_Layout = {
    .id = "LAYOUT_DEWFORD_TOWN_GYM",
    .width = 18,
    .height = 28,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_DewfordGym,
    .border_filepath = "data/layouts/DewfordTown_Gym/border.bin",
    .blockdata_offset = 4354608,
};

MapLayout DewfordTown_Hall_Layout = {
    .id = "LAYOUT_DEWFORD_TOWN_HALL",
    .width = 17,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/DewfordTown_Hall/border.bin",
    .blockdata_offset = 4355648,
};

MapLayout House4_Layout = {
    .id = "LAYOUT_HOUSE4",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/House4/border.bin",
    .blockdata_offset = 4355988,
};

MapLayout LavaridgeTown_HerbShop_Layout = {
    .id = "LAYOUT_LAVARIDGE_TOWN_HERB_SHOP",
    .width = 11,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/LavaridgeTown_HerbShop/border.bin",
    .blockdata_offset = 4356200,
};

MapLayout LavaridgeTown_Gym_1F_Layout = {
    .id = "LAYOUT_LAVARIDGE_TOWN_GYM_1F",
    .width = 17,
    .height = 19,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_LavaridgeGym,
    .border_filepath = "data/layouts/LavaridgeTown_Gym_1F/border.bin",
    .blockdata_offset = 4356408,
};

MapLayout LavaridgeTown_Gym_B1F_Layout = {
    .id = "LAYOUT_LAVARIDGE_TOWN_GYM_B1F",
    .width = 17,
    .height = 19,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_LavaridgeGym,
    .border_filepath = "data/layouts/LavaridgeTown_Gym_B1F/border.bin",
    .blockdata_offset = 4357088,
};

MapLayout LavaridgeTown_PokemonCenter_1F_Layout = {
    .id = "LAYOUT_LAVARIDGE_TOWN_POKEMON_CENTER_1F",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PokemonCenter,
    .border_filepath = "data/layouts/LavaridgeTown_PokemonCenter_1F/border.bin",
    .blockdata_offset = 4357768,
};

MapLayout FallarborTown_LeftoverRSContestLobby_Layout = {
    .id = "LAYOUT_FALLARBOR_TOWN_LEFTOVER_RSCONTEST_LOBBY",
    .width = 15,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/FallarborTown_LeftoverRSContestLobby/border.bin",
    .blockdata_offset = 4358052,
};

MapLayout FallarborTown_LeftoverRSContestHall_Layout = {
    .id = "LAYOUT_FALLARBOR_TOWN_LEFTOVER_RSCONTEST_HALL",
    .width = 21,
    .height = 18,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/FallarborTown_LeftoverRSContestHall/border.bin",
    .blockdata_offset = 4358296,
};

MapLayout LilycoveCity_House2_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_HOUSE2",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/LilycoveCity_House2/border.bin",
    .blockdata_offset = 4359084,
};

MapLayout UnusedContestRoom1_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_ROOM1",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestRoom1/border.bin",
    .blockdata_offset = 1383014,
};

MapLayout VerdanturfTown_WandasHouse_Layout = {
    .id = "LAYOUT_VERDANTURF_TOWN_WANDAS_HOUSE",
    .width = 17,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/VerdanturfTown_WandasHouse/border.bin",
    .blockdata_offset = 4359280,
};

MapLayout PacifidlogTown_House1_Layout = {
    .id = "LAYOUT_PACIFIDLOG_TOWN_HOUSE1",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/PacifidlogTown_House1/border.bin",
    .blockdata_offset = 4359584,
};

MapLayout PacifidlogTown_House2_Layout = {
    .id = "LAYOUT_PACIFIDLOG_TOWN_HOUSE2",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/PacifidlogTown_House2/border.bin",
    .blockdata_offset = 4359796,
};

MapLayout PetalburgCity_Gym_Layout = {
    .id = "LAYOUT_PETALBURG_CITY_GYM",
    .width = 9,
    .height = 112,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PetalburgGym,
    .border_filepath = "data/layouts/PetalburgCity_Gym/border.bin",
    .blockdata_offset = 4360008,
};

MapLayout HouseWithBed_Layout = {
    .id = "LAYOUT_HOUSE_WITH_BED",
    .width = 10,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/HouseWithBed/border.bin",
    .blockdata_offset = 4362056,
};

MapLayout SlateportCity_SternsShipyard_1F_Layout = {
    .id = "LAYOUT_SLATEPORT_CITY_STERNS_SHIPYARD_1F",
    .width = 21,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/SlateportCity_SternsShipyard_1F/border.bin",
    .blockdata_offset = 4362248,
};

MapLayout SlateportCity_SternsShipyard_2F_Layout = {
    .id = "LAYOUT_SLATEPORT_CITY_STERNS_SHIPYARD_2F",
    .width = 17,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/SlateportCity_SternsShipyard_2F/border.bin",
    .blockdata_offset = 4362912,
};

MapLayout UnusedContestRoom2_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_ROOM2",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestRoom2/border.bin",
    .blockdata_offset = 4363456,
};

MapLayout UnusedContestRoom3_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_ROOM3",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestRoom3/border.bin",
    .blockdata_offset = 1383014,
};

MapLayout SlateportCity_PokemonFanClub_Layout = {
    .id = "LAYOUT_SLATEPORT_CITY_POKEMON_FAN_CLUB",
    .width = 14,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PokemonFanClub,
    .border_filepath = "data/layouts/SlateportCity_PokemonFanClub/border.bin",
    .blockdata_offset = 4363528,
};

MapLayout SlateportCity_OceanicMuseum_1F_Layout = {
    .id = "LAYOUT_SLATEPORT_CITY_OCEANIC_MUSEUM_1F",
    .width = 20,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_OceanicMuseum,
    .border_filepath = "data/layouts/SlateportCity_OceanicMuseum_1F/border.bin",
    .blockdata_offset = 4363868,
};

MapLayout SlateportCity_OceanicMuseum_2F_Layout = {
    .id = "LAYOUT_SLATEPORT_CITY_OCEANIC_MUSEUM_2F",
    .width = 20,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_OceanicMuseum,
    .border_filepath = "data/layouts/SlateportCity_OceanicMuseum_2F/border.bin",
    .blockdata_offset = 4364260,
};

MapLayout Harbor_Layout = {
    .id = "LAYOUT_HARBOR",
    .width = 24,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/Harbor/border.bin",
    .blockdata_offset = 4364652,
};

MapLayout MauvilleCity_Gym_Layout = {
    .id = "LAYOUT_MAUVILLE_CITY_GYM",
    .width = 10,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_MauvilleGym,
    .border_filepath = "data/layouts/MauvilleCity_Gym/border.bin",
    .blockdata_offset = 4365404,
};

MapLayout MauvilleCity_BikeShop_Layout = {
    .id = "LAYOUT_MAUVILLE_CITY_BIKE_SHOP",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BikeShop,
    .border_filepath = "data/layouts/MauvilleCity_BikeShop/border.bin",
    .blockdata_offset = 4365856,
};

MapLayout MauvilleCity_GameCorner_Layout = {
    .id = "LAYOUT_MAUVILLE_CITY_GAME_CORNER",
    .width = 22,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_MauvilleGameCorner,
    .border_filepath = "data/layouts/MauvilleCity_GameCorner/border.bin",
    .blockdata_offset = 4366104,
};

MapLayout RustboroCity_DevonCorp_1F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_DEVON_CORP_1F",
    .width = 19,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/RustboroCity_DevonCorp_1F/border.bin",
    .blockdata_offset = 4366620,
};

MapLayout RustboroCity_DevonCorp_2F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_DEVON_CORP_2F",
    .width = 19,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/RustboroCity_DevonCorp_2F/border.bin",
    .blockdata_offset = 4366996,
};

MapLayout RustboroCity_Gym_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_GYM",
    .width = 11,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_RustboroGym,
    .border_filepath = "data/layouts/RustboroCity_Gym/border.bin",
    .blockdata_offset = 4367372,
};

MapLayout RustboroCity_PokemonSchool_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_POKEMON_SCHOOL",
    .width = 12,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PokemonSchool,
    .border_filepath = "data/layouts/RustboroCity_PokemonSchool/border.bin",
    .blockdata_offset = 4367844,
};

MapLayout RustboroCity_House_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_HOUSE",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_House/border.bin",
    .blockdata_offset = 4368140,
};

MapLayout RustboroCity_House1_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_HOUSE1",
    .width = 13,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_House1/border.bin",
    .blockdata_offset = 4368388,
};

MapLayout RustboroCity_CuttersHouse_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_CUTTERS_HOUSE",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_CuttersHouse/border.bin",
    .blockdata_offset = 4368628,
};

MapLayout FortreeCity_House1_Layout = {
    .id = "LAYOUT_FORTREE_CITY_HOUSE1",
    .width = 8,
    .height = 6,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/FortreeCity_House1/border.bin",
    .blockdata_offset = 4368860,
};

MapLayout FortreeCity_Gym_Layout = {
    .id = "LAYOUT_FORTREE_CITY_GYM",
    .width = 20,
    .height = 25,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_FortreeGym,
    .border_filepath = "data/layouts/FortreeCity_Gym/border.bin",
    .blockdata_offset = 4368988,
};

MapLayout FortreeCity_House2_Layout = {
    .id = "LAYOUT_FORTREE_CITY_HOUSE2",
    .width = 8,
    .height = 6,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/FortreeCity_House2/border.bin",
    .blockdata_offset = 4370020,
};

MapLayout Route104_MrBrineysHouse_Layout = {
    .id = "LAYOUT_ROUTE104_MR_BRINEYS_HOUSE",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/Route104_MrBrineysHouse/border.bin",
    .blockdata_offset = 4370148,
};

MapLayout LilycoveCity_LilycoveMuseum_1F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_LILYCOVE_MUSEUM_1F",
    .width = 21,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_LilycoveMuseum,
    .border_filepath = "data/layouts/LilycoveCity_LilycoveMuseum_1F/border.bin",
    .blockdata_offset = 4370396,
};

MapLayout LilycoveCity_LilycoveMuseum_2F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_LILYCOVE_MUSEUM_2F",
    .width = 22,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_LilycoveMuseum,
    .border_filepath = "data/layouts/LilycoveCity_LilycoveMuseum_2F/border.bin",
    .blockdata_offset = 4371016,
};

MapLayout LilycoveCity_ContestLobby_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_CONTEST_LOBBY",
    .width = 31,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/LilycoveCity_ContestLobby/border.bin",
    .blockdata_offset = 4371620,
};

MapLayout LilycoveCity_ContestHall_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_CONTEST_HALL",
    .width = 51,
    .height = 33,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/LilycoveCity_ContestHall/border.bin",
    .blockdata_offset = 4372396,
};

MapLayout LilycoveCity_PokemonTrainerFanClub_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_POKEMON_TRAINER_FAN_CLUB",
    .width = 12,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/LilycoveCity_PokemonTrainerFanClub/border.bin",
    .blockdata_offset = 4375796,
};

MapLayout MossdeepCity_Gym_Layout = {
    .id = "LAYOUT_MOSSDEEP_CITY_GYM",
    .width = 26,
    .height = 36,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_MossdeepGym,
    .border_filepath = "data/layouts/MossdeepCity_Gym/border.bin",
    .blockdata_offset = 4376164,
};

MapLayout SootopolisCity_Gym_1F_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_GYM_1F",
    .width = 17,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_SootopolisGym,
    .border_filepath = "data/layouts/SootopolisCity_Gym_1F/border.bin",
    .blockdata_offset = 4378068,
};

MapLayout SootopolisCity_Gym_B1F_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_GYM_B1F",
    .width = 17,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_SootopolisGym,
    .border_filepath = "data/layouts/SootopolisCity_Gym_B1F/border.bin",
    .blockdata_offset = 4378984,
};

MapLayout EverGrandeCity_SidneysRoom_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_SIDNEYS_ROOM",
    .width = 13,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_EliteFour,
    .border_filepath = "data/layouts/EverGrandeCity_SidneysRoom/border.bin",
    .blockdata_offset = 4379900,
};

MapLayout EverGrandeCity_PhoebesRoom_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_PHOEBES_ROOM",
    .width = 13,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_EliteFour,
    .border_filepath = "data/layouts/EverGrandeCity_PhoebesRoom/border.bin",
    .blockdata_offset = 4380296,
};

MapLayout EverGrandeCity_GlaciasRoom_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_GLACIAS_ROOM",
    .width = 13,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_EliteFour,
    .border_filepath = "data/layouts/EverGrandeCity_GlaciasRoom/border.bin",
    .blockdata_offset = 4380692,
};

MapLayout EverGrandeCity_DrakesRoom_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_DRAKES_ROOM",
    .width = 13,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_EliteFour,
    .border_filepath = "data/layouts/EverGrandeCity_DrakesRoom/border.bin",
    .blockdata_offset = 4381088,
};

MapLayout EverGrandeCity_ChampionsRoom_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_CHAMPIONS_ROOM",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_EliteFour,
    .border_filepath = "data/layouts/EverGrandeCity_ChampionsRoom/border.bin",
    .blockdata_offset = 4381484,
};

MapLayout EverGrandeCity_ShortHall_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_SHORT_HALL",
    .width = 11,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_EliteFour,
    .border_filepath = "data/layouts/EverGrandeCity_ShortHall/border.bin",
    .blockdata_offset = 4381856,
};

MapLayout Route104_PrettyPetalFlowerShop_Layout = {
    .id = "LAYOUT_ROUTE104_PRETTY_PETAL_FLOWER_SHOP",
    .width = 15,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PrettyPetalFlowerShop,
    .border_filepath = "data/layouts/Route104_PrettyPetalFlowerShop/border.bin",
    .blockdata_offset = 4382176,
};

MapLayout CableCarStation_Layout = {
    .id = "LAYOUT_CABLE_CAR_STATION",
    .width = 13,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/CableCarStation/border.bin",
    .blockdata_offset = 4382480,
};

MapLayout Route114_FossilManiacsHouse_Layout = {
    .id = "LAYOUT_ROUTE114_FOSSIL_MANIACS_HOUSE",
    .width = 10,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/Route114_FossilManiacsHouse/border.bin",
    .blockdata_offset = 4382824,
};

MapLayout Route114_FossilManiacsTunnel_Layout = {
    .id = "LAYOUT_ROUTE114_FOSSIL_MANIACS_TUNNEL",
    .width = 13,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fallarbor,
    .border_filepath = "data/layouts/Route114_FossilManiacsTunnel/border.bin",
    .blockdata_offset = 4383016,
};

MapLayout Route114_LanettesHouse_Layout = {
    .id = "LAYOUT_ROUTE114_LANETTES_HOUSE",
    .width = 11,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Lab,
    .border_filepath = "data/layouts/Route114_LanettesHouse/border.bin",
    .blockdata_offset = 4383724,
};

MapLayout Route116_TunnelersRestHouse_Layout = {
    .id = "LAYOUT_ROUTE116_TUNNELERS_REST_HOUSE",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/Route116_TunnelersRestHouse/border.bin",
    .blockdata_offset = 4383932,
};

MapLayout Route117_PokemonDayCare_Layout = {
    .id = "LAYOUT_ROUTE117_POKEMON_DAY_CARE",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PokemonDayCare,
    .border_filepath = "data/layouts/Route117_PokemonDayCare/border.bin",
    .blockdata_offset = 4384144,
};

MapLayout Route121_SafariZoneEntrance_Layout = {
    .id = "LAYOUT_ROUTE121_SAFARI_ZONE_ENTRANCE",
    .width = 18,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/Route121_SafariZoneEntrance/border.bin",
    .blockdata_offset = 4384392,
};

MapLayout MeteorFalls_1F_1R_Layout = {
    .id = "LAYOUT_METEOR_FALLS_1F_1R",
    .width = 30,
    .height = 42,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MeteorFalls,
    .border_filepath = "data/layouts/MeteorFalls_1F_1R/border.bin",
    .blockdata_offset = 4384928,
};

MapLayout MeteorFalls_1F_2R_Layout = {
    .id = "LAYOUT_METEOR_FALLS_1F_2R",
    .width = 30,
    .height = 32,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MeteorFalls,
    .border_filepath = "data/layouts/MeteorFalls_1F_2R/border.bin",
    .blockdata_offset = 4387480,
};

MapLayout MeteorFalls_B1F_1R_Layout = {
    .id = "LAYOUT_METEOR_FALLS_B1F_1R",
    .width = 29,
    .height = 38,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MeteorFalls,
    .border_filepath = "data/layouts/MeteorFalls_B1F_1R/border.bin",
    .blockdata_offset = 4389432,
};

MapLayout MeteorFalls_B1F_2R_Layout = {
    .id = "LAYOUT_METEOR_FALLS_B1F_2R",
    .width = 11,
    .height = 18,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MeteorFalls,
    .border_filepath = "data/layouts/MeteorFalls_B1F_2R/border.bin",
    .blockdata_offset = 4391668,
};

MapLayout RusturfTunnel_Layout = {
    .id = "LAYOUT_RUSTURF_TUNNEL",
    .width = 36,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_RusturfTunnel,
    .border_filepath = "data/layouts/RusturfTunnel/border.bin",
    .blockdata_offset = 4392096,
};

MapLayout Underwater_SootopolisCity_Layout = {
    .id = "LAYOUT_UNDERWATER_SOOTOPOLIS_CITY",
    .width = 20,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_SootopolisCity/border.bin",
    .blockdata_offset = 4393856,
};

MapLayout DesertRuins_Layout = {
    .id = "LAYOUT_DESERT_RUINS",
    .width = 17,
    .height = 33,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/DesertRuins/border.bin",
    .blockdata_offset = 4394288,
};

MapLayout GraniteCave_1F_Layout = {
    .id = "LAYOUT_GRANITE_CAVE_1F",
    .width = 42,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/GraniteCave_1F/border.bin",
    .blockdata_offset = 4395444,
};

MapLayout GraniteCave_B1F_Layout = {
    .id = "LAYOUT_GRANITE_CAVE_B1F",
    .width = 32,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/GraniteCave_B1F/border.bin",
    .blockdata_offset = 4396736,
};

MapLayout GraniteCave_B2F_Layout = {
    .id = "LAYOUT_GRANITE_CAVE_B2F",
    .width = 32,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/GraniteCave_B2F/border.bin",
    .blockdata_offset = 4398432,
};

MapLayout PetalburgWoods_Layout = {
    .id = "LAYOUT_PETALBURG_WOODS",
    .width = 48,
    .height = 44,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/PetalburgWoods/border.bin",
    .blockdata_offset = 4400128,
};

MapLayout MtChimney_Layout = {
    .id = "LAYOUT_MT_CHIMNEY",
    .width = 40,
    .height = 47,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MtChimney/border.bin",
    .blockdata_offset = 4404384,
};

MapLayout MtPyre_1F_Layout = {
    .id = "LAYOUT_MT_PYRE_1F",
    .width = 22,
    .height = 19,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_1F/border.bin",
    .blockdata_offset = 4408176,
};

MapLayout MtPyre_2F_Layout = {
    .id = "LAYOUT_MT_PYRE_2F",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_2F/border.bin",
    .blockdata_offset = 4409044,
};

MapLayout MtPyre_3F_Layout = {
    .id = "LAYOUT_MT_PYRE_3F",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_3F/border.bin",
    .blockdata_offset = 4409416,
};

MapLayout MtPyre_4F_Layout = {
    .id = "LAYOUT_MT_PYRE_4F",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_4F/border.bin",
    .blockdata_offset = 4409788,
};

MapLayout MtPyre_5F_Layout = {
    .id = "LAYOUT_MT_PYRE_5F",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_5F/border.bin",
    .blockdata_offset = 4410160,
};

MapLayout MtPyre_6F_Layout = {
    .id = "LAYOUT_MT_PYRE_6F",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_6F/border.bin",
    .blockdata_offset = 4410532,
};

MapLayout AquaHideout_1F_Layout = {
    .id = "LAYOUT_AQUA_HIDEOUT_1F",
    .width = 28,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AquaHideout_1F/border.bin",
    .blockdata_offset = 4410904,
};

MapLayout AquaHideout_B1F_Layout = {
    .id = "LAYOUT_AQUA_HIDEOUT_B1F",
    .width = 51,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AquaHideout_B1F/border.bin",
    .blockdata_offset = 4412616,
};

MapLayout AquaHideout_B2F_Layout = {
    .id = "LAYOUT_AQUA_HIDEOUT_B2F",
    .width = 34,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AquaHideout_B2F/border.bin",
    .blockdata_offset = 4415096,
};

MapLayout Underwater_SeafloorCavern_Layout = {
    .id = "LAYOUT_UNDERWATER_SEAFLOOR_CAVERN",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_SeafloorCavern/border.bin",
    .blockdata_offset = 4416760,
};

MapLayout SeafloorCavern_Entrance_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ENTRANCE",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Entrance/border.bin",
    .blockdata_offset = 4417044,
};

MapLayout SeafloorCavern_Room1_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM1",
    .width = 20,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room1/border.bin",
    .blockdata_offset = 4417876,
};

MapLayout SeafloorCavern_Room2_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM2",
    .width = 18,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room2/border.bin",
    .blockdata_offset = 4418748,
};

MapLayout SeafloorCavern_Room3_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM3",
    .width = 16,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room3/border.bin",
    .blockdata_offset = 4419212,
};

MapLayout SeafloorCavern_Room4_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM4",
    .width = 18,
    .height = 19,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room4/border.bin",
    .blockdata_offset = 4419788,
};

MapLayout SeafloorCavern_Room5_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM5",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room5/border.bin",
    .blockdata_offset = 4420504,
};

MapLayout SeafloorCavern_Room6_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM6",
    .width = 24,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SeafloorCavern_Room6/border.bin",
    .blockdata_offset = 4421336,
};

MapLayout SeafloorCavern_Room7_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM7",
    .width = 23,
    .height = 25,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SeafloorCavern_Room7/border.bin",
    .blockdata_offset = 4422472,
};

MapLayout SeafloorCavern_Room8_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM8",
    .width = 11,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room8/border.bin",
    .blockdata_offset = 4423656,
};

MapLayout SeafloorCavern_Room9_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM9",
    .width = 27,
    .height = 46,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room9/border.bin",
    .blockdata_offset = 4423996,
};

MapLayout CaveOfOrigin_Entrance_Layout = {
    .id = "LAYOUT_CAVE_OF_ORIGIN_ENTRANCE",
    .width = 19,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/CaveOfOrigin_Entrance/border.bin",
    .blockdata_offset = 4426512,
};

MapLayout CaveOfOrigin_1F_Layout = {
    .id = "LAYOUT_CAVE_OF_ORIGIN_1F",
    .width = 23,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/CaveOfOrigin_1F/border.bin",
    .blockdata_offset = 4427532,
};

MapLayout CaveOfOrigin_UnusedRubySapphireMap1_Layout = {
    .id = "LAYOUT_CAVE_OF_ORIGIN_UNUSED_RUBY_SAPPHIRE_MAP1",
    .width = 23,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/CaveOfOrigin_UnusedRubySapphireMap1/border.bin",
    .blockdata_offset = 4428624,
};

MapLayout CaveOfOrigin_UnusedRubySapphireMap2_Layout = {
    .id = "LAYOUT_CAVE_OF_ORIGIN_UNUSED_RUBY_SAPPHIRE_MAP2",
    .width = 21,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/CaveOfOrigin_UnusedRubySapphireMap2/border.bin",
    .blockdata_offset = 4429716,
};

MapLayout CaveOfOrigin_UnusedRubySapphireMap3_Layout = {
    .id = "LAYOUT_CAVE_OF_ORIGIN_UNUSED_RUBY_SAPPHIRE_MAP3",
    .width = 19,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/CaveOfOrigin_UnusedRubySapphireMap3/border.bin",
    .blockdata_offset = 4430632,
};

MapLayout CaveOfOrigin_B1F_Layout = {
    .id = "LAYOUT_CAVE_OF_ORIGIN_B1F",
    .width = 19,
    .height = 19,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/CaveOfOrigin_B1F/border.bin",
    .blockdata_offset = 4431464,
};

MapLayout VictoryRoad_1F_Layout = {
    .id = "LAYOUT_VICTORY_ROAD_1F",
    .width = 46,
    .height = 45,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/VictoryRoad_1F/border.bin",
    .blockdata_offset = 4432220,
};

MapLayout ShoalCave_LowTideEntranceRoom_Layout = {
    .id = "LAYOUT_SHOAL_CAVE_LOW_TIDE_ENTRANCE_ROOM",
    .width = 35,
    .height = 35,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ShoalCave_LowTideEntranceRoom/border.bin",
    .blockdata_offset = 4436392,
};

MapLayout ShoalCave_LowTideInnerRoom_Layout = {
    .id = "LAYOUT_SHOAL_CAVE_LOW_TIDE_INNER_ROOM",
    .width = 46,
    .height = 38,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ShoalCave_LowTideInnerRoom/border.bin",
    .blockdata_offset = 4438876,
};

MapLayout ShoalCave_LowTideStairsRoom_Layout = {
    .id = "LAYOUT_SHOAL_CAVE_LOW_TIDE_STAIRS_ROOM",
    .width = 21,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ShoalCave_LowTideStairsRoom/border.bin",
    .blockdata_offset = 4442404,
};

MapLayout ShoalCave_LowTideLowerRoom_Layout = {
    .id = "LAYOUT_SHOAL_CAVE_LOW_TIDE_LOWER_ROOM",
    .width = 31,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ShoalCave_LowTideLowerRoom/border.bin",
    .blockdata_offset = 4443068,
};

MapLayout ShoalCave_HighTideEntranceRoom_Layout = {
    .id = "LAYOUT_SHOAL_CAVE_HIGH_TIDE_ENTRANCE_ROOM",
    .width = 35,
    .height = 35,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ShoalCave_HighTideEntranceRoom/border.bin",
    .blockdata_offset = 4443968,
};

MapLayout ShoalCave_HighTideInnerRoom_Layout = {
    .id = "LAYOUT_SHOAL_CAVE_HIGH_TIDE_INNER_ROOM",
    .width = 46,
    .height = 38,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ShoalCave_HighTideInnerRoom/border.bin",
    .blockdata_offset = 4446452,
};

MapLayout UnusedCave1_Layout = {
    .id = "LAYOUT_UNUSED_CAVE1",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave1/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave2_Layout = {
    .id = "LAYOUT_UNUSED_CAVE2",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave2/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave3_Layout = {
    .id = "LAYOUT_UNUSED_CAVE3",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave3/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave4_Layout = {
    .id = "LAYOUT_UNUSED_CAVE4",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave4/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave5_Layout = {
    .id = "LAYOUT_UNUSED_CAVE5",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave5/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave6_Layout = {
    .id = "LAYOUT_UNUSED_CAVE6",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave6/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave7_Layout = {
    .id = "LAYOUT_UNUSED_CAVE7",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave7/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave8_Layout = {
    .id = "LAYOUT_UNUSED_CAVE8",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave8/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave9_Layout = {
    .id = "LAYOUT_UNUSED_CAVE9",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave9/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave10_Layout = {
    .id = "LAYOUT_UNUSED_CAVE10",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave10/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave11_Layout = {
    .id = "LAYOUT_UNUSED_CAVE11",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave11/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave12_Layout = {
    .id = "LAYOUT_UNUSED_CAVE12",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave12/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave13_Layout = {
    .id = "LAYOUT_UNUSED_CAVE13",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave13/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout UnusedCave14_Layout = {
    .id = "LAYOUT_UNUSED_CAVE14",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/UnusedCave14/border.bin",
    .blockdata_offset = 3333448,
};

MapLayout NewMauville_Entrance_Layout = {
    .id = "LAYOUT_NEW_MAUVILLE_ENTRANCE",
    .width = 9,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/NewMauville_Entrance/border.bin",
    .blockdata_offset = 4450484,
};

MapLayout NewMauville_Inside_Layout = {
    .id = "LAYOUT_NEW_MAUVILLE_INSIDE",
    .width = 41,
    .height = 41,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BikeShop,
    .border_filepath = "data/layouts/NewMauville_Inside/border.bin",
    .blockdata_offset = 4450680,
};

MapLayout AbandonedShip_Deck_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_DECK",
    .width = 23,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AbandonedShip_Deck/border.bin",
    .blockdata_offset = 4454076,
};

MapLayout AbandonedShip_Corridors_1F_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_CORRIDORS_1F",
    .width = 18,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Corridors_1F/border.bin",
    .blockdata_offset = 4455076,
};

MapLayout AbandonedShip_Rooms_1F_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_ROOMS_1F",
    .width = 18,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Rooms_1F/border.bin",
    .blockdata_offset = 4455540,
};

MapLayout AbandonedShip_Corridors_B1F_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_CORRIDORS_B1F",
    .width = 13,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Corridors_B1F/border.bin",
    .blockdata_offset = 4456184,
};

MapLayout AbandonedShip_Rooms_B1F_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_ROOMS_B1F",
    .width = 27,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Rooms_B1F/border.bin",
    .blockdata_offset = 4456476,
};

MapLayout AbandonedShip_Rooms2_B1F_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_ROOMS2_B1F",
    .width = 18,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Rooms2_B1F/border.bin",
    .blockdata_offset = 4456940,
};

MapLayout AbandonedShip_Underwater1_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_UNDERWATER1",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Underwater1/border.bin",
    .blockdata_offset = 4457260,
};

MapLayout AbandonedShip_Room_B1F_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_ROOM_B1F",
    .width = 9,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Room_B1F/border.bin",
    .blockdata_offset = 4457420,
};

MapLayout AbandonedShip_Rooms2_1F_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_ROOMS2_1F",
    .width = 9,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Rooms2_1F/border.bin",
    .blockdata_offset = 4457596,
};

MapLayout AbandonedShip_CaptainsOffice_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_CAPTAINS_OFFICE",
    .width = 9,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AbandonedShip_CaptainsOffice/border.bin",
    .blockdata_offset = 4457936,
};

MapLayout AbandonedShip_Underwater2_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_UNDERWATER2",
    .width = 21,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_Underwater2/border.bin",
    .blockdata_offset = 4458096,
};

MapLayout SecretBase_RedCave1_Layout = {
    .id = "LAYOUT_SECRET_BASE_RED_CAVE1",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseRedCave,
    .border_filepath = "data/layouts/SecretBase_RedCave1/border.bin",
    .blockdata_offset = 4458424,
};

MapLayout SecretBase_BrownCave1_Layout = {
    .id = "LAYOUT_SECRET_BASE_BROWN_CAVE1",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBrownCave,
    .border_filepath = "data/layouts/SecretBase_BrownCave1/border.bin",
    .blockdata_offset = 4458656,
};

MapLayout SecretBase_BlueCave1_Layout = {
    .id = "LAYOUT_SECRET_BASE_BLUE_CAVE1",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBlueCave,
    .border_filepath = "data/layouts/SecretBase_BlueCave1/border.bin",
    .blockdata_offset = 4458888,
};

MapLayout SecretBase_YellowCave1_Layout = {
    .id = "LAYOUT_SECRET_BASE_YELLOW_CAVE1",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseYellowCave,
    .border_filepath = "data/layouts/SecretBase_YellowCave1/border.bin",
    .blockdata_offset = 4459120,
};

MapLayout SecretBase_Tree1_Layout = {
    .id = "LAYOUT_SECRET_BASE_TREE1",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseTree,
    .border_filepath = "data/layouts/SecretBase_Tree1/border.bin",
    .blockdata_offset = 4459352,
};

MapLayout SecretBase_Shrub1_Layout = {
    .id = "LAYOUT_SECRET_BASE_SHRUB1",
    .width = 11,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseShrub,
    .border_filepath = "data/layouts/SecretBase_Shrub1/border.bin",
    .blockdata_offset = 4459584,
};

MapLayout SecretBase_RedCave2_Layout = {
    .id = "LAYOUT_SECRET_BASE_RED_CAVE2",
    .width = 7,
    .height = 16,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseRedCave,
    .border_filepath = "data/layouts/SecretBase_RedCave2/border.bin",
    .blockdata_offset = 4459816,
};

MapLayout SecretBase_BrownCave2_Layout = {
    .id = "LAYOUT_SECRET_BASE_BROWN_CAVE2",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBrownCave,
    .border_filepath = "data/layouts/SecretBase_BrownCave2/border.bin",
    .blockdata_offset = 4460072,
};

MapLayout SecretBase_BlueCave2_Layout = {
    .id = "LAYOUT_SECRET_BASE_BLUE_CAVE2",
    .width = 15,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBlueCave,
    .border_filepath = "data/layouts/SecretBase_BlueCave2/border.bin",
    .blockdata_offset = 4460356,
};

MapLayout SecretBase_YellowCave2_Layout = {
    .id = "LAYOUT_SECRET_BASE_YELLOW_CAVE2",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseYellowCave,
    .border_filepath = "data/layouts/SecretBase_YellowCave2/border.bin",
    .blockdata_offset = 4460600,
};

MapLayout SecretBase_Tree2_Layout = {
    .id = "LAYOUT_SECRET_BASE_TREE2",
    .width = 7,
    .height = 16,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseTree,
    .border_filepath = "data/layouts/SecretBase_Tree2/border.bin",
    .blockdata_offset = 4460884,
};

MapLayout SecretBase_Shrub2_Layout = {
    .id = "LAYOUT_SECRET_BASE_SHRUB2",
    .width = 15,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseShrub,
    .border_filepath = "data/layouts/SecretBase_Shrub2/border.bin",
    .blockdata_offset = 4461140,
};

MapLayout SecretBase_RedCave3_Layout = {
    .id = "LAYOUT_SECRET_BASE_RED_CAVE3",
    .width = 15,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseRedCave,
    .border_filepath = "data/layouts/SecretBase_RedCave3/border.bin",
    .blockdata_offset = 4461384,
};

MapLayout SecretBase_BrownCave3_Layout = {
    .id = "LAYOUT_SECRET_BASE_BROWN_CAVE3",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBrownCave,
    .border_filepath = "data/layouts/SecretBase_BrownCave3/border.bin",
    .blockdata_offset = 4461656,
};

MapLayout SecretBase_BlueCave3_Layout = {
    .id = "LAYOUT_SECRET_BASE_BLUE_CAVE3",
    .width = 10,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBlueCave,
    .border_filepath = "data/layouts/SecretBase_BlueCave3/border.bin",
    .blockdata_offset = 4462020,
};

MapLayout SecretBase_YellowCave3_Layout = {
    .id = "LAYOUT_SECRET_BASE_YELLOW_CAVE3",
    .width = 12,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseYellowCave,
    .border_filepath = "data/layouts/SecretBase_YellowCave3/border.bin",
    .blockdata_offset = 4462392,
};

MapLayout SecretBase_Tree3_Layout = {
    .id = "LAYOUT_SECRET_BASE_TREE3",
    .width = 17,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseTree,
    .border_filepath = "data/layouts/SecretBase_Tree3/border.bin",
    .blockdata_offset = 4462688,
};

MapLayout SecretBase_Shrub3_Layout = {
    .id = "LAYOUT_SECRET_BASE_SHRUB3",
    .width = 13,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseShrub,
    .border_filepath = "data/layouts/SecretBase_Shrub3/border.bin",
    .blockdata_offset = 4462992,
};

MapLayout SecretBase_RedCave4_Layout = {
    .id = "LAYOUT_SECRET_BASE_RED_CAVE4",
    .width = 9,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseRedCave,
    .border_filepath = "data/layouts/SecretBase_RedCave4/border.bin",
    .blockdata_offset = 4463312,
};

MapLayout SecretBase_BrownCave4_Layout = {
    .id = "LAYOUT_SECRET_BASE_BROWN_CAVE4",
    .width = 14,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBrownCave,
    .border_filepath = "data/layouts/SecretBase_BrownCave4/border.bin",
    .blockdata_offset = 4463616,
};

MapLayout SecretBase_BlueCave4_Layout = {
    .id = "LAYOUT_SECRET_BASE_BLUE_CAVE4",
    .width = 9,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseBlueCave,
    .border_filepath = "data/layouts/SecretBase_BlueCave4/border.bin",
    .blockdata_offset = 4463984,
};

MapLayout SecretBase_YellowCave4_Layout = {
    .id = "LAYOUT_SECRET_BASE_YELLOW_CAVE4",
    .width = 13,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseYellowCave,
    .border_filepath = "data/layouts/SecretBase_YellowCave4/border.bin",
    .blockdata_offset = 4464324,
};

MapLayout SecretBase_Tree4_Layout = {
    .id = "LAYOUT_SECRET_BASE_TREE4",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseTree,
    .border_filepath = "data/layouts/SecretBase_Tree4/border.bin",
    .blockdata_offset = 4464720,
};

MapLayout SecretBase_Shrub4_Layout = {
    .id = "LAYOUT_SECRET_BASE_SHRUB4",
    .width = 14,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_SecretBase,
    .secondary_tileset = &gTileset_SecretBaseShrub,
    .border_filepath = "data/layouts/SecretBase_Shrub4/border.bin",
    .blockdata_offset = 4465144,
};

MapLayout BattleColosseum_2P_Layout = {
    .id = "LAYOUT_BATTLE_COLOSSEUM_2P",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_CableClub,
    .border_filepath = "data/layouts/BattleColosseum_2P/border.bin",
    .blockdata_offset = 4465484,
};

MapLayout TradeCenter_Layout = {
    .id = "LAYOUT_TRADE_CENTER",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_CableClub,
    .border_filepath = "data/layouts/TradeCenter/border.bin",
    .blockdata_offset = 4465768,
};

MapLayout RecordCorner_Layout = {
    .id = "LAYOUT_RECORD_CORNER",
    .width = 20,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_CableClub,
    .border_filepath = "data/layouts/RecordCorner/border.bin",
    .blockdata_offset = 4466016,
};

MapLayout BattleColosseum_4P_Layout = {
    .id = "LAYOUT_BATTLE_COLOSSEUM_4P",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_CableClub,
    .border_filepath = "data/layouts/BattleColosseum_4P/border.bin",
    .blockdata_offset = 4466448,
};

MapLayout ContestHall_Layout = {
    .id = "LAYOUT_CONTEST_HALL",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/ContestHall/border.bin",
    .blockdata_offset = 4466732,
};

MapLayout UnusedContestHall1_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_HALL1",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestHall1/border.bin",
    .blockdata_offset = 377,
};

MapLayout UnusedContestHall2_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_HALL2",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestHall2/border.bin",
    .blockdata_offset = 377,
};

MapLayout UnusedContestHall3_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_HALL3",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestHall3/border.bin",
    .blockdata_offset = 377,
};

MapLayout UnusedContestHall4_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_HALL4",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestHall4/border.bin",
    .blockdata_offset = 377,
};

MapLayout UnusedContestHall5_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_HALL5",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestHall5/border.bin",
    .blockdata_offset = 377,
};

MapLayout UnusedContestHall6_Layout = {
    .id = "LAYOUT_UNUSED_CONTEST_HALL6",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/UnusedContestHall6/border.bin",
    .blockdata_offset = 377,
};

MapLayout ContestHallBeauty_Layout = {
    .id = "LAYOUT_CONTEST_HALL_BEAUTY",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/ContestHallBeauty/border.bin",
    .blockdata_offset = 4467312,
};

MapLayout ContestHallTough_Layout = {
    .id = "LAYOUT_CONTEST_HALL_TOUGH",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/ContestHallTough/border.bin",
    .blockdata_offset = 4467676,
};

MapLayout ContestHallCool_Layout = {
    .id = "LAYOUT_CONTEST_HALL_COOL",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/ContestHallCool/border.bin",
    .blockdata_offset = 4468040,
};

MapLayout ContestHallSmart_Layout = {
    .id = "LAYOUT_CONTEST_HALL_SMART",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/ContestHallSmart/border.bin",
    .blockdata_offset = 4468404,
};

MapLayout ContestHallCute_Layout = {
    .id = "LAYOUT_CONTEST_HALL_CUTE",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Contest,
    .border_filepath = "data/layouts/ContestHallCute/border.bin",
    .blockdata_offset = 4468768,
};

MapLayout InsideOfTruck_Layout = {
    .id = "LAYOUT_INSIDE_OF_TRUCK",
    .width = 5,
    .height = 5,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideOfTruck,
    .border_filepath = "data/layouts/InsideOfTruck/border.bin",
    .blockdata_offset = 4469132,
};

MapLayout SafariZone_Northwest_Layout = {
    .id = "LAYOUT_SAFARI_ZONE_NORTHWEST",
    .width = 40,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/SafariZone_Northwest/border.bin",
    .blockdata_offset = 4469216,
};

MapLayout SafariZone_North_Layout = {
    .id = "LAYOUT_SAFARI_ZONE_NORTH",
    .width = 40,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/SafariZone_North/border.bin",
    .blockdata_offset = 4472448,
};

MapLayout SafariZone_Southwest_Layout = {
    .id = "LAYOUT_SAFARI_ZONE_SOUTHWEST",
    .width = 40,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/SafariZone_Southwest/border.bin",
    .blockdata_offset = 4475680,
};

MapLayout SafariZone_South_Layout = {
    .id = "LAYOUT_SAFARI_ZONE_SOUTH",
    .width = 40,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/SafariZone_South/border.bin",
    .blockdata_offset = 4478912,
};

MapLayout UnusedOutdoorArea_Layout = {
    .id = "LAYOUT_UNUSED_OUTDOOR_AREA",
    .width = 58,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_General,
    .border_filepath = "data/layouts/UnusedOutdoorArea/border.bin",
    .blockdata_offset = 4482144,
};

MapLayout Route109_SeashoreHouse_Layout = {
    .id = "LAYOUT_ROUTE109_SEASHORE_HOUSE",
    .width = 15,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_SeashoreHouse,
    .border_filepath = "data/layouts/Route109_SeashoreHouse/border.bin",
    .blockdata_offset = 4485192,
};

MapLayout Route110_TrickHouseEntrance_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_ENTRANCE",
    .width = 12,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/Route110_TrickHouseEntrance/border.bin",
    .blockdata_offset = 4485524,
};

MapLayout Route110_TrickHouseEnd_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_END",
    .width = 12,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/Route110_TrickHouseEnd/border.bin",
    .blockdata_offset = 4485748,
};

MapLayout Route110_TrickHouseCorridor_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_CORRIDOR",
    .width = 15,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/Route110_TrickHouseCorridor/border.bin",
    .blockdata_offset = 4485972,
};

MapLayout Route110_TrickHousePuzzle1_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE1",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle1/border.bin",
    .blockdata_offset = 4486724,
};

MapLayout Route110_TrickHousePuzzle2_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE2",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle2/border.bin",
    .blockdata_offset = 4487416,
};

MapLayout Route110_TrickHousePuzzle3_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE3",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle3/border.bin",
    .blockdata_offset = 4488108,
};

MapLayout Route110_TrickHousePuzzle4_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE4",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle4/border.bin",
    .blockdata_offset = 4488800,
};

MapLayout Route110_TrickHousePuzzle5_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE5",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle5/border.bin",
    .blockdata_offset = 4489492,
};

MapLayout Route110_TrickHousePuzzle6_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE6",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle6/border.bin",
    .blockdata_offset = 4490184,
};

MapLayout Route110_TrickHousePuzzle7_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE7",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle7/border.bin",
    .blockdata_offset = 4490876,
};

MapLayout Route110_TrickHousePuzzle8_Layout = {
    .id = "LAYOUT_ROUTE110_TRICK_HOUSE_PUZZLE8",
    .width = 15,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrickHousePuzzle,
    .border_filepath = "data/layouts/Route110_TrickHousePuzzle8/border.bin",
    .blockdata_offset = 4491568,
};

MapLayout FortreeCity_DecorationShop_Layout = {
    .id = "LAYOUT_FORTREE_CITY_DECORATION_SHOP",
    .width = 8,
    .height = 6,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/FortreeCity_DecorationShop/border.bin",
    .blockdata_offset = 4492260,
};

MapLayout Route110_SeasideCyclingRoadEntrance_Layout = {
    .id = "LAYOUT_ROUTE110_SEASIDE_CYCLING_ROAD_ENTRANCE",
    .width = 15,
    .height = 6,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/Route110_SeasideCyclingRoadEntrance/border.bin",
    .blockdata_offset = 4492388,
};

MapLayout LilycoveCity_DepartmentStore_1F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_DEPARTMENT_STORE_1F",
    .width = 18,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/LilycoveCity_DepartmentStore_1F/border.bin",
    .blockdata_offset = 4492600,
};

MapLayout LilycoveCity_DepartmentStore_2F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_DEPARTMENT_STORE_2F",
    .width = 18,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/LilycoveCity_DepartmentStore_2F/border.bin",
    .blockdata_offset = 4492920,
};

MapLayout LilycoveCity_DepartmentStore_3F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_DEPARTMENT_STORE_3F",
    .width = 18,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/LilycoveCity_DepartmentStore_3F/border.bin",
    .blockdata_offset = 4493240,
};

MapLayout LilycoveCity_DepartmentStore_4F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_DEPARTMENT_STORE_4F",
    .width = 18,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/LilycoveCity_DepartmentStore_4F/border.bin",
    .blockdata_offset = 4493560,
};

MapLayout LilycoveCity_DepartmentStore_5F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_DEPARTMENT_STORE_5F",
    .width = 18,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/LilycoveCity_DepartmentStore_5F/border.bin",
    .blockdata_offset = 4493880,
};

MapLayout LilycoveCity_DepartmentStoreRooftop_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_DEPARTMENT_STORE_ROOFTOP",
    .width = 18,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Shop,
    .border_filepath = "data/layouts/LilycoveCity_DepartmentStoreRooftop/border.bin",
    .blockdata_offset = 4494200,
};

MapLayout Route130_Layout = {
    .id = "LAYOUT_ROUTE130",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/Route130/border.bin",
    .blockdata_offset = 4494664,
};

MapLayout BattleFrontier_BattleTowerLobby_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY",
    .width = 25,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_BattleTowerLobby/border.bin",
    .blockdata_offset = 4501096,
};

MapLayout BattleFrontier_OutsideWest_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_OUTSIDE_WEST",
    .width = 56,
    .height = 72,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BattleFrontierOutsideWest,
    .border_filepath = "data/layouts/BattleFrontier_OutsideWest/border.bin",
    .blockdata_offset = 4501628,
};

MapLayout BattleElevator_Layout = {
    .id = "LAYOUT_BATTLE_ELEVATOR",
    .width = 5,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleElevator/border.bin",
    .blockdata_offset = 4509724,
};

MapLayout BattleFrontier_BattleTowerCorridor_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_TOWER_CORRIDOR",
    .width = 17,
    .height = 5,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_BattleTowerCorridor/border.bin",
    .blockdata_offset = 4509828,
};

MapLayout BattleFrontier_BattleTowerBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_TOWER_BATTLE_ROOM",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_BattleTowerBattleRoom/border.bin",
    .blockdata_offset = 4510032,
};

MapLayout RustboroCity_DevonCorp_3F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_DEVON_CORP_3F",
    .width = 19,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/RustboroCity_DevonCorp_3F/border.bin",
    .blockdata_offset = 4510244,
};

MapLayout EverGrandeCity_PokemonLeague_1F_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_POKEMON_LEAGUE_1F",
    .width = 19,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_PokemonCenter,
    .border_filepath = "data/layouts/EverGrandeCity_PokemonLeague_1F/border.bin",
    .blockdata_offset = 4510620,
};

MapLayout Route119_WeatherInstitute_1F_Layout = {
    .id = "LAYOUT_ROUTE119_WEATHER_INSTITUTE_1F",
    .width = 20,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Lab,
    .border_filepath = "data/layouts/Route119_WeatherInstitute_1F/border.bin",
    .blockdata_offset = 4511108,
};

MapLayout Route119_WeatherInstitute_2F_Layout = {
    .id = "LAYOUT_ROUTE119_WEATHER_INSTITUTE_2F",
    .width = 20,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Lab,
    .border_filepath = "data/layouts/Route119_WeatherInstitute_2F/border.bin",
    .blockdata_offset = 4511660,
};

MapLayout LilycoveCity_DepartmentStoreElevator_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_DEPARTMENT_STORE_ELEVATOR",
    .width = 5,
    .height = 6,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/LilycoveCity_DepartmentStoreElevator/border.bin",
    .blockdata_offset = 4512132,
};

MapLayout Underwater_Route124_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE124",
    .width = 80,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route124/border.bin",
    .blockdata_offset = 4512224,
};

MapLayout MossdeepCity_SpaceCenter_1F_Layout = {
    .id = "LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_1F",
    .width = 16,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MossdeepCity_SpaceCenter_1F/border.bin",
    .blockdata_offset = 4525056,
};

MapLayout MossdeepCity_SpaceCenter_2F_Layout = {
    .id = "LAYOUT_MOSSDEEP_CITY_SPACE_CENTER_2F",
    .width = 16,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MossdeepCity_SpaceCenter_2F/border.bin",
    .blockdata_offset = 4525408,
};

MapLayout SSTidalCorridor_Layout = {
    .id = "LAYOUT_SS_TIDAL_CORRIDOR",
    .width = 18,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/SSTidalCorridor/border.bin",
    .blockdata_offset = 4525760,
};

MapLayout SSTidalLowerDeck_Layout = {
    .id = "LAYOUT_SS_TIDAL_LOWER_DECK",
    .width = 17,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/SSTidalLowerDeck/border.bin",
    .blockdata_offset = 4526260,
};

MapLayout SSTidalRooms_Layout = {
    .id = "LAYOUT_SS_TIDAL_ROOMS",
    .width = 36,
    .height = 18,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/SSTidalRooms/border.bin",
    .blockdata_offset = 4526736,
};

MapLayout IslandCave_Layout = {
    .id = "LAYOUT_ISLAND_CAVE",
    .width = 17,
    .height = 33,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/IslandCave/border.bin",
    .blockdata_offset = 4528064,
};

MapLayout AncientTomb_Layout = {
    .id = "LAYOUT_ANCIENT_TOMB",
    .width = 17,
    .height = 33,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/AncientTomb/border.bin",
    .blockdata_offset = 4394288,
};

MapLayout Underwater_Route134_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE134",
    .width = 18,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route134/border.bin",
    .blockdata_offset = 4530376,
};

MapLayout Underwater_SealedChamber_Layout = {
    .id = "LAYOUT_UNDERWATER_SEALED_CHAMBER",
    .width = 22,
    .height = 48,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_SealedChamber/border.bin",
    .blockdata_offset = 4530768,
};

MapLayout SealedChamber_OuterRoom_Layout = {
    .id = "LAYOUT_SEALED_CHAMBER_OUTER_ROOM",
    .width = 21,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SealedChamber_OuterRoom/border.bin",
    .blockdata_offset = 4532912,
};

MapLayout VictoryRoad_B1F_Layout = {
    .id = "LAYOUT_VICTORY_ROAD_B1F",
    .width = 46,
    .height = 31,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/VictoryRoad_B1F/border.bin",
    .blockdata_offset = 4533912,
};

MapLayout VictoryRoad_B2F_Layout = {
    .id = "LAYOUT_VICTORY_ROAD_B2F",
    .width = 46,
    .height = 31,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/VictoryRoad_B2F/border.bin",
    .blockdata_offset = 4536796,
};

MapLayout Route104_Prototype_Layout = {
    .id = "LAYOUT_ROUTE104_PROTOTYPE",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/Route104_Prototype/border.bin",
    .blockdata_offset = 4827,
};

MapLayout GraniteCave_StevensRoom_Layout = {
    .id = "LAYOUT_GRANITE_CAVE_STEVENS_ROOM",
    .width = 15,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/GraniteCave_StevensRoom/border.bin",
    .blockdata_offset = 4539716,
};

MapLayout AbandonedShip_HiddenFloorCorridors_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_HIDDEN_FLOOR_CORRIDORS",
    .width = 13,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_HiddenFloorCorridors/border.bin",
    .blockdata_offset = 4540168,
};

MapLayout SouthernIsland_Exterior_Layout = {
    .id = "LAYOUT_SOUTHERN_ISLAND_EXTERIOR",
    .width = 33,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/SouthernIsland_Exterior/border.bin",
    .blockdata_offset = 4540488,
};

MapLayout SouthernIsland_Interior_Layout = {
    .id = "LAYOUT_SOUTHERN_ISLAND_INTERIOR",
    .width = 27,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/SouthernIsland_Interior/border.bin",
    .blockdata_offset = 4542500,
};

MapLayout JaggedPass_Layout = {
    .id = "LAYOUT_JAGGED_PASS",
    .width = 30,
    .height = 46,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/JaggedPass/border.bin",
    .blockdata_offset = 4543828,
};

MapLayout FieryPath_Layout = {
    .id = "LAYOUT_FIERY_PATH",
    .width = 35,
    .height = 38,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/FieryPath/border.bin",
    .blockdata_offset = 4546620,
};

MapLayout RustboroCity_Flat2_1F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_FLAT2_1F",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_Flat2_1F/border.bin",
    .blockdata_offset = 4549312,
};

MapLayout RustboroCity_Flat2_2F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_FLAT2_2F",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_Flat2_2F/border.bin",
    .blockdata_offset = 4549596,
};

MapLayout RustboroCity_Flat2_3F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_FLAT2_3F",
    .width = 14,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_Flat2_3F/border.bin",
    .blockdata_offset = 4549880,
};

MapLayout SootopolisCity_LotadAndSeedotHouse_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_LOTAD_AND_SEEDOT_HOUSE",
    .width = 8,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/SootopolisCity_LotadAndSeedotHouse/border.bin",
    .blockdata_offset = 4550164,
};

MapLayout EverGrandeCity_HallOfFame_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_HALL_OF_FAME",
    .width = 15,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_CableClub,
    .border_filepath = "data/layouts/EverGrandeCity_HallOfFame/border.bin",
    .blockdata_offset = 4550308,
};

MapLayout LilycoveCity_CoveLilyMotel_1F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_COVE_LILY_MOTEL_1F",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/LilycoveCity_CoveLilyMotel_1F/border.bin",
    .blockdata_offset = 4550852,
};

MapLayout LilycoveCity_CoveLilyMotel_2F_Layout = {
    .id = "LAYOUT_LILYCOVE_CITY_COVE_LILY_MOTEL_2F",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/LilycoveCity_CoveLilyMotel_2F/border.bin",
    .blockdata_offset = 4551100,
};

MapLayout Route124_DivingTreasureHuntersHouse_Layout = {
    .id = "LAYOUT_ROUTE124_DIVING_TREASURE_HUNTERS_HOUSE",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/Route124_DivingTreasureHuntersHouse/border.bin",
    .blockdata_offset = 4551348,
};

MapLayout MtPyre_Exterior_Layout = {
    .id = "LAYOUT_MT_PYRE_EXTERIOR",
    .width = 38,
    .height = 51,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_Exterior/border.bin",
    .blockdata_offset = 4551560,
};

MapLayout MtPyre_Summit_Layout = {
    .id = "LAYOUT_MT_PYRE_SUMMIT",
    .width = 50,
    .height = 37,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/MtPyre_Summit/border.bin",
    .blockdata_offset = 4555468,
};

MapLayout SealedChamber_InnerRoom_Layout = {
    .id = "LAYOUT_SEALED_CHAMBER_INNER_ROOM",
    .width = 21,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SealedChamber_InnerRoom/border.bin",
    .blockdata_offset = 4559200,
};

MapLayout MossdeepCity_GameCorner_1F_Layout = {
    .id = "LAYOUT_MOSSDEEP_CITY_GAME_CORNER_1F",
    .width = 12,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_MossdeepGameCorner,
    .border_filepath = "data/layouts/MossdeepCity_GameCorner_1F/border.bin",
    .blockdata_offset = 4560200,
};

MapLayout MossdeepCity_GameCorner_B1F_Layout = {
    .id = "LAYOUT_MOSSDEEP_CITY_GAME_CORNER_B1F",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/MossdeepCity_GameCorner_B1F/border.bin",
    .blockdata_offset = 4560472,
};

MapLayout SootopolisCity_House1_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_HOUSE1",
    .width = 8,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/SootopolisCity_House1/border.bin",
    .blockdata_offset = 4560720,
};

MapLayout SootopolisCity_House2_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_HOUSE2",
    .width = 8,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/SootopolisCity_House2/border.bin",
    .blockdata_offset = 4560864,
};

MapLayout SootopolisCity_House3_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_HOUSE3",
    .width = 8,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/SootopolisCity_House3/border.bin",
    .blockdata_offset = 4561008,
};

MapLayout AbandonedShip_HiddenFloorRooms_Layout = {
    .id = "LAYOUT_ABANDONED_SHIP_HIDDEN_FLOOR_ROOMS",
    .width = 44,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_InsideShip,
    .border_filepath = "data/layouts/AbandonedShip_HiddenFloorRooms/border.bin",
    .blockdata_offset = 4561152,
};

MapLayout ScorchedSlab_Layout = {
    .id = "LAYOUT_SCORCHED_SLAB",
    .width = 15,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ScorchedSlab/border.bin",
    .blockdata_offset = 4562504,
};

MapLayout CaveOfOrigin_Unused_B4F_Lava_Layout = {
    .id = "LAYOUT_CAVE_OF_ORIGIN_UNUSED_B4F_LAVA",
    .width = 19,
    .height = 19,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/CaveOfOrigin_Unused_B4F_Lava/border.bin",
    .blockdata_offset = 4563136,
};

MapLayout RustboroCity_Flat1_1F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_FLAT1_1F",
    .width = 14,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_Flat1_1F/border.bin",
    .blockdata_offset = 4563892,
};

MapLayout RustboroCity_Flat1_2F_Layout = {
    .id = "LAYOUT_RUSTBORO_CITY_FLAT1_2F",
    .width = 14,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/RustboroCity_Flat1_2F/border.bin",
    .blockdata_offset = 4564148,
};

MapLayout EverGrandeCity_Hall4_Layout = {
    .id = "LAYOUT_EVER_GRANDE_CITY_HALL4",
    .width = 11,
    .height = 34,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_EliteFour,
    .border_filepath = "data/layouts/EverGrandeCity_Hall4/border.bin",
    .blockdata_offset = 4564404,
};

MapLayout AquaHideout_UnusedRubyMap1_Layout = {
    .id = "LAYOUT_AQUA_HIDEOUT_UNUSED_RUBY_MAP1",
    .width = 28,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AquaHideout_UnusedRubyMap1/border.bin",
    .blockdata_offset = 4565184,
};

MapLayout AquaHideout_UnusedRubyMap2_Layout = {
    .id = "LAYOUT_AQUA_HIDEOUT_UNUSED_RUBY_MAP2",
    .width = 62,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AquaHideout_UnusedRubyMap2/border.bin",
    .blockdata_offset = 4566896,
};

MapLayout AquaHideout_UnusedRubyMap3_Layout = {
    .id = "LAYOUT_AQUA_HIDEOUT_UNUSED_RUBY_MAP3",
    .width = 34,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Facility,
    .border_filepath = "data/layouts/AquaHideout_UnusedRubyMap3/border.bin",
    .blockdata_offset = 4569904,
};

MapLayout Route131_SkyPillar_Layout = {
    .id = "LAYOUT_ROUTE131_SKY_PILLAR",
    .width = 60,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/Route131_SkyPillar/border.bin",
    .blockdata_offset = 4571568,
};

MapLayout SkyPillar_Entrance_Layout = {
    .id = "LAYOUT_SKY_PILLAR_ENTRANCE",
    .width = 18,
    .height = 18,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SkyPillar_Entrance/border.bin",
    .blockdata_offset = 4576400,
};

MapLayout SkyPillar_Outside_Layout = {
    .id = "LAYOUT_SKY_PILLAR_OUTSIDE",
    .width = 28,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_Outside/border.bin",
    .blockdata_offset = 4577080,
};

MapLayout SkyPillar_1F_Layout = {
    .id = "LAYOUT_SKY_PILLAR_1F",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_1F/border.bin",
    .blockdata_offset = 4578400,
};

MapLayout SkyPillar_2F_Layout = {
    .id = "LAYOUT_SKY_PILLAR_2F",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_2F/border.bin",
    .blockdata_offset = 4578824,
};

MapLayout SkyPillar_3F_Layout = {
    .id = "LAYOUT_SKY_PILLAR_3F",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_3F/border.bin",
    .blockdata_offset = 4579248,
};

MapLayout SkyPillar_4F_Layout = {
    .id = "LAYOUT_SKY_PILLAR_4F",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_4F/border.bin",
    .blockdata_offset = 4579672,
};

MapLayout SeafloorCavern_Room9_Lava_Layout = {
    .id = "LAYOUT_SEAFLOOR_CAVERN_ROOM9_LAVA",
    .width = 27,
    .height = 46,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/SeafloorCavern_Room9_Lava/border.bin",
    .blockdata_offset = 4580096,
};

MapLayout MossdeepCity_StevensHouse_Layout = {
    .id = "LAYOUT_MOSSDEEP_CITY_STEVENS_HOUSE",
    .width = 11,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/MossdeepCity_StevensHouse/border.bin",
    .blockdata_offset = 4582612,
};

MapLayout ShoalCave_LowTideIceRoom_Layout = {
    .id = "LAYOUT_SHOAL_CAVE_LOW_TIDE_ICE_ROOM",
    .width = 20,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ShoalCave_LowTideIceRoom/border.bin",
    .blockdata_offset = 4582820,
};

MapLayout SafariZone_RestHouse_Layout = {
    .id = "LAYOUT_SAFARI_ZONE_REST_HOUSE",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_GenericBuilding,
    .border_filepath = "data/layouts/SafariZone_RestHouse/border.bin",
    .blockdata_offset = 4584052,
};

MapLayout SkyPillar_5F_Layout = {
    .id = "LAYOUT_SKY_PILLAR_5F",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_5F/border.bin",
    .blockdata_offset = 4584264,
};

MapLayout SkyPillar_Top_Layout = {
    .id = "LAYOUT_SKY_PILLAR_TOP",
    .width = 27,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_Top/border.bin",
    .blockdata_offset = 4584688,
};

MapLayout BattleFrontier_BattleDomeLobby_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_DOME_LOBBY",
    .width = 23,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleDome,
    .border_filepath = "data/layouts/BattleFrontier_BattleDomeLobby/border.bin",
    .blockdata_offset = 4586016,
};

MapLayout BattleFrontier_BattleDomeCorridor_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_DOME_CORRIDOR",
    .width = 48,
    .height = 7,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleDome,
    .border_filepath = "data/layouts/BattleFrontier_BattleDomeCorridor/border.bin",
    .blockdata_offset = 4586832,
};

MapLayout BattleFrontier_BattleDomePreBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_DOME_PRE_BATTLE_ROOM",
    .width = 9,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleDome,
    .border_filepath = "data/layouts/BattleFrontier_BattleDomePreBattleRoom/border.bin",
    .blockdata_offset = 4587536,
};

MapLayout BattleFrontier_BattleDomeBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_DOME_BATTLE_ROOM",
    .width = 20,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleDome,
    .border_filepath = "data/layouts/BattleFrontier_BattleDomeBattleRoom/border.bin",
    .blockdata_offset = 4587712,
};

MapLayout MagmaHideout_1F_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_1F",
    .width = 37,
    .height = 38,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_1F/border.bin",
    .blockdata_offset = 4588144,
};

MapLayout MagmaHideout_2F_1R_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_2F_1R",
    .width = 33,
    .height = 39,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_2F_1R/border.bin",
    .blockdata_offset = 4590988,
};

MapLayout MagmaHideout_2F_2R_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_2F_2R",
    .width = 49,
    .height = 28,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_2F_2R/border.bin",
    .blockdata_offset = 4593596,
};

MapLayout MagmaHideout_3F_1R_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_3F_1R",
    .width = 28,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_3F_1R/border.bin",
    .blockdata_offset = 4596372,
};

MapLayout MagmaHideout_3F_2R_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_3F_2R",
    .width = 24,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_3F_2R/border.bin",
    .blockdata_offset = 4597748,
};

MapLayout MagmaHideout_4F_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_4F",
    .width = 59,
    .height = 28,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_4F/border.bin",
    .blockdata_offset = 4598596,
};

MapLayout BattleFrontier_BattlePalaceLobby_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY",
    .width = 25,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePalace,
    .border_filepath = "data/layouts/BattleFrontier_BattlePalaceLobby/border.bin",
    .blockdata_offset = 4601932,
};

MapLayout BattleFrontier_BattlePalaceCorridor_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PALACE_CORRIDOR",
    .width = 17,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BattlePalace,
    .border_filepath = "data/layouts/BattleFrontier_BattlePalaceCorridor/border.bin",
    .blockdata_offset = 4602564,
};

MapLayout BattleFrontier_BattlePalaceBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PALACE_BATTLE_ROOM",
    .width = 15,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BattlePalace,
    .border_filepath = "data/layouts/BattleFrontier_BattlePalaceBattleRoom/border.bin",
    .blockdata_offset = 4603072,
};

MapLayout BattleFrontier_OutsideEast_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_OUTSIDE_EAST",
    .width = 72,
    .height = 72,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BattleFrontierOutsideEast,
    .border_filepath = "data/layouts/BattleFrontier_OutsideEast/border.bin",
    .blockdata_offset = 4603404,
};

MapLayout BattleFrontier_BattleFactoryLobby_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY",
    .width = 19,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFactory,
    .border_filepath = "data/layouts/BattleFrontier_BattleFactoryLobby/border.bin",
    .blockdata_offset = 4613804,
};

MapLayout BattleFrontier_BattleFactoryPreBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_FACTORY_PRE_BATTLE_ROOM",
    .width = 17,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFactory,
    .border_filepath = "data/layouts/BattleFrontier_BattleFactoryPreBattleRoom/border.bin",
    .blockdata_offset = 4614292,
};

MapLayout BattleFrontier_BattleFactoryBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_FACTORY_BATTLE_ROOM",
    .width = 13,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFactory,
    .border_filepath = "data/layouts/BattleFrontier_BattleFactoryBattleRoom/border.bin",
    .blockdata_offset = 4614800,
};

MapLayout BattleFrontier_BattlePikeLobby_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY",
    .width = 11,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePike,
    .border_filepath = "data/layouts/BattleFrontier_BattlePikeLobby/border.bin",
    .blockdata_offset = 4615144,
};

MapLayout BattleFrontier_BattlePikeCorridor_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_CORRIDOR",
    .width = 14,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePike,
    .border_filepath = "data/layouts/BattleFrontier_BattlePikeCorridor/border.bin",
    .blockdata_offset = 4615464,
};

MapLayout BattleFrontier_BattlePikeThreePathRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_THREE_PATH_ROOM",
    .width = 13,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePike,
    .border_filepath = "data/layouts/BattleFrontier_BattlePikeThreePathRoom/border.bin",
    .blockdata_offset = 4615720,
};

MapLayout BattleFrontier_BattlePikeRoomNormal_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_NORMAL",
    .width = 9,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePike,
    .border_filepath = "data/layouts/BattleFrontier_BattlePikeRoomNormal/border.bin",
    .blockdata_offset = 4616040,
};

MapLayout BattleFrontier_BattlePikeRoomFinal_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_FINAL",
    .width = 5,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePike,
    .border_filepath = "data/layouts/BattleFrontier_BattlePikeRoomFinal/border.bin",
    .blockdata_offset = 4616216,
};

MapLayout BattleFrontier_BattleArenaLobby_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY",
    .width = 16,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleArena,
    .border_filepath = "data/layouts/BattleFrontier_BattleArenaLobby/border.bin",
    .blockdata_offset = 4616328,
};

MapLayout BattleFrontier_BattleArenaCorridor_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_ARENA_CORRIDOR",
    .width = 18,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleArena,
    .border_filepath = "data/layouts/BattleFrontier_BattleArenaCorridor/border.bin",
    .blockdata_offset = 4616776,
};

MapLayout BattleFrontier_BattleArenaBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_ARENA_BATTLE_ROOM",
    .width = 16,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleArena,
    .border_filepath = "data/layouts/BattleFrontier_BattleArenaBattleRoom/border.bin",
    .blockdata_offset = 4617312,
};

MapLayout SootopolisCity_LegendsBattle_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_LEGENDS_BATTLE",
    .width = 60,
    .height = 60,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Sootopolis,
    .border_filepath = "data/layouts/SootopolisCity_LegendsBattle/border.bin",
    .blockdata_offset = 4617696,
};

MapLayout BattleFrontier_BattlePikeRoomWildMons_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS",
    .width = 9,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePike,
    .border_filepath = "data/layouts/BattleFrontier_BattlePikeRoomWildMons/border.bin",
    .blockdata_offset = 4624928,
};

MapLayout BattleFrontier_BattlePikeRoomUnused_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_UNUSED",
    .width = 1,
    .height = 1,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePike,
    .border_filepath = "data/layouts/BattleFrontier_BattlePikeRoomUnused/border.bin",
    .blockdata_offset = 1978128,
};

MapLayout BattleFrontier_BattlePyramidLobby_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY",
    .width = 15,
    .height = 18,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattleFrontier_BattlePyramidLobby/border.bin",
    .blockdata_offset = 4625356,
};

MapLayout BattleFrontier_BattlePyramidFloor_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattleFrontier_BattlePyramidFloor/border.bin",
    .blockdata_offset = 4625928,
};

MapLayout BattlePyramidSquare01_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE01",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare01/border.bin",
    .blockdata_offset = 4626088,
};

MapLayout BattlePyramidSquare02_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE02",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare02/border.bin",
    .blockdata_offset = 4626248,
};

MapLayout BattlePyramidSquare03_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE03",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare03/border.bin",
    .blockdata_offset = 4626408,
};

MapLayout BattlePyramidSquare04_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE04",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare04/border.bin",
    .blockdata_offset = 4626568,
};

MapLayout BattlePyramidSquare05_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE05",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare05/border.bin",
    .blockdata_offset = 4626728,
};

MapLayout BattlePyramidSquare06_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE06",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare06/border.bin",
    .blockdata_offset = 4626888,
};

MapLayout BattlePyramidSquare07_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE07",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare07/border.bin",
    .blockdata_offset = 4627048,
};

MapLayout BattlePyramidSquare08_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE08",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare08/border.bin",
    .blockdata_offset = 4627208,
};

MapLayout BattlePyramidSquare09_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE09",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare09/border.bin",
    .blockdata_offset = 4627368,
};

MapLayout BattlePyramidSquare10_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE10",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare10/border.bin",
    .blockdata_offset = 4627528,
};

MapLayout BattlePyramidSquare11_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE11",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare11/border.bin",
    .blockdata_offset = 4627688,
};

MapLayout BattlePyramidSquare12_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE12",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare12/border.bin",
    .blockdata_offset = 4627848,
};

MapLayout BattlePyramidSquare13_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE13",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare13/border.bin",
    .blockdata_offset = 4628008,
};

MapLayout BattlePyramidSquare14_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE14",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare14/border.bin",
    .blockdata_offset = 4628168,
};

MapLayout BattlePyramidSquare15_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE15",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare15/border.bin",
    .blockdata_offset = 4628328,
};

MapLayout BattlePyramidSquare16_Layout = {
    .id = "LAYOUT_BATTLE_PYRAMID_SQUARE16",
    .width = 8,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattlePyramidSquare16/border.bin",
    .blockdata_offset = 4628488,
};

MapLayout BattleFrontier_BattlePyramidTop_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP",
    .width = 34,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattlePyramid,
    .border_filepath = "data/layouts/BattleFrontier_BattlePyramidTop/border.bin",
    .blockdata_offset = 4628648,
};

MapLayout MagmaHideout_3F_3R_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_3F_3R",
    .width = 33,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_3F_3R/border.bin",
    .blockdata_offset = 4630244,
};

MapLayout MagmaHideout_2F_3R_Layout = {
    .id = "LAYOUT_MAGMA_HIDEOUT_2F_3R",
    .width = 60,
    .height = 19,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lavaridge,
    .border_filepath = "data/layouts/MagmaHideout_2F_3R/border.bin",
    .blockdata_offset = 4631860,
};

MapLayout MirageTower_1F_Layout = {
    .id = "LAYOUT_MIRAGE_TOWER_1F",
    .width = 21,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MirageTower,
    .border_filepath = "data/layouts/MirageTower_1F/border.bin",
    .blockdata_offset = 4634172,
};

MapLayout MirageTower_2F_Layout = {
    .id = "LAYOUT_MIRAGE_TOWER_2F",
    .width = 21,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MirageTower,
    .border_filepath = "data/layouts/MirageTower_2F/border.bin",
    .blockdata_offset = 4634920,
};

MapLayout MirageTower_3F_Layout = {
    .id = "LAYOUT_MIRAGE_TOWER_3F",
    .width = 21,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MirageTower,
    .border_filepath = "data/layouts/MirageTower_3F/border.bin",
    .blockdata_offset = 4635668,
};

MapLayout BattleTentLobby_Layout = {
    .id = "LAYOUT_BATTLE_TENT_LOBBY",
    .width = 13,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleTent,
    .border_filepath = "data/layouts/BattleTentLobby/border.bin",
    .blockdata_offset = 4636416,
};

MapLayout BattleTentCorridor_Layout = {
    .id = "LAYOUT_BATTLE_TENT_CORRIDOR",
    .width = 5,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleTent,
    .border_filepath = "data/layouts/BattleTentCorridor/border.bin",
    .blockdata_offset = 4636708,
};

MapLayout BattleTentBattleRoom_Layout = {
    .id = "LAYOUT_BATTLE_TENT_BATTLE_ROOM",
    .width = 10,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleTent,
    .border_filepath = "data/layouts/BattleTentBattleRoom/border.bin",
    .blockdata_offset = 4636820,
};

MapLayout VerdanturfTown_BattleTentBattleRoom_Layout = {
    .id = "LAYOUT_VERDANTURF_TOWN_BATTLE_TENT_BATTLE_ROOM",
    .width = 13,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BattleTent,
    .border_filepath = "data/layouts/VerdanturfTown_BattleTentBattleRoom/border.bin",
    .blockdata_offset = 4637032,
};

MapLayout MirageTower_4F_Layout = {
    .id = "LAYOUT_MIRAGE_TOWER_4F",
    .width = 13,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MirageTower,
    .border_filepath = "data/layouts/MirageTower_4F/border.bin",
    .blockdata_offset = 4637300,
};

MapLayout DesertUnderpass_Layout = {
    .id = "LAYOUT_DESERT_UNDERPASS",
    .width = 139,
    .height = 23,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/DesertUnderpass/border.bin",
    .blockdata_offset = 4637592,
};

MapLayout BattleFrontier_BattleTowerMultiPartnerRoom_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_PARTNER_ROOM",
    .width = 21,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_BattleTowerMultiPartnerRoom/border.bin",
    .blockdata_offset = 4644020,
};

MapLayout BattleFrontier_BattleTowerMultiCorridor_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_CORRIDOR",
    .width = 17,
    .height = 5,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_BattleTowerMultiCorridor/border.bin",
    .blockdata_offset = 4644684,
};

MapLayout Route111_NoMirageTower_Layout = {
    .id = "LAYOUT_ROUTE111_NO_MIRAGE_TOWER",
    .width = 40,
    .height = 140,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Mauville,
    .border_filepath = "data/layouts/Route111_NoMirageTower/border.bin",
    .blockdata_offset = 4644888,
};

MapLayout UnionRoom_Layout = {
    .id = "LAYOUT_UNION_ROOM",
    .width = 15,
    .height = 12,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_UnionRoom,
    .border_filepath = "data/layouts/UnionRoom/border.bin",
    .blockdata_offset = 4656120,
};

MapLayout SafariZone_Northeast_Layout = {
    .id = "LAYOUT_SAFARI_ZONE_NORTHEAST",
    .width = 40,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/SafariZone_Northeast/border.bin",
    .blockdata_offset = 4656512,
};

MapLayout SafariZone_Southeast_Layout = {
    .id = "LAYOUT_SAFARI_ZONE_SOUTHEAST",
    .width = 40,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Lilycove,
    .border_filepath = "data/layouts/SafariZone_Southeast/border.bin",
    .blockdata_offset = 4659744,
};

MapLayout BattleFrontier_RankingHall_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_RANKING_HALL",
    .width = 53,
    .height = 15,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontierRankingHall,
    .border_filepath = "data/layouts/BattleFrontier_RankingHall/border.bin",
    .blockdata_offset = 4662976,
};

MapLayout BattleFrontier_Lounge1_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_LOUNGE1",
    .width = 13,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_Lounge1/border.bin",
    .blockdata_offset = 4664600,
};

MapLayout BattleFrontier_ExchangeServiceCorner_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_EXCHANGE_SERVICE_CORNER",
    .width = 15,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_ExchangeServiceCorner/border.bin",
    .blockdata_offset = 4664840,
};

MapLayout BattleFrontier_ReceptionGate_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_RECEPTION_GATE",
    .width = 9,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_ReceptionGate/border.bin",
    .blockdata_offset = 4665204,
};

MapLayout ArtisanCave_B1F_Layout = {
    .id = "LAYOUT_ARTISAN_CAVE_B1F",
    .width = 46,
    .height = 54,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ArtisanCave_B1F/border.bin",
    .blockdata_offset = 4665488,
};

MapLayout ArtisanCave_1F_Layout = {
    .id = "LAYOUT_ARTISAN_CAVE_1F",
    .width = 21,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/ArtisanCave_1F/border.bin",
    .blockdata_offset = 4670488,
};

MapLayout FarawayIsland_Entrance_Layout = {
    .id = "LAYOUT_FARAWAY_ISLAND_ENTRANCE",
    .width = 34,
    .height = 46,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Rustboro,
    .border_filepath = "data/layouts/FarawayIsland_Entrance/border.bin",
    .blockdata_offset = 4671444,
};

MapLayout FarawayIsland_Interior_Layout = {
    .id = "LAYOUT_FARAWAY_ISLAND_INTERIOR",
    .width = 29,
    .height = 26,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Fortree,
    .border_filepath = "data/layouts/FarawayIsland_Interior/border.bin",
    .blockdata_offset = 4674604,
};

MapLayout BirthIsland_Exterior_Layout = {
    .id = "LAYOUT_BIRTH_ISLAND_EXTERIOR",
    .width = 30,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Dewford,
    .border_filepath = "data/layouts/BirthIsland_Exterior/border.bin",
    .blockdata_offset = 4676144,
};

MapLayout IslandHarbor_Layout = {
    .id = "LAYOUT_ISLAND_HARBOR",
    .width = 17,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_IslandHarbor,
    .border_filepath = "data/layouts/IslandHarbor/border.bin",
    .blockdata_offset = 4677976,
};

MapLayout Underwater_MarineCave_Layout = {
    .id = "LAYOUT_UNDERWATER_MARINE_CAVE",
    .width = 20,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_MarineCave/border.bin",
    .blockdata_offset = 4678452,
};

MapLayout MarineCave_Entrance_Layout = {
    .id = "LAYOUT_MARINE_CAVE_ENTRANCE",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/MarineCave_Entrance/border.bin",
    .blockdata_offset = 4678884,
};

MapLayout TerraCave_Entrance_Layout = {
    .id = "LAYOUT_TERRA_CAVE_ENTRANCE",
    .width = 20,
    .height = 20,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/TerraCave_Entrance/border.bin",
    .blockdata_offset = 4679716,
};

MapLayout TerraCave_End_Layout = {
    .id = "LAYOUT_TERRA_CAVE_END",
    .width = 27,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/TerraCave_End/border.bin",
    .blockdata_offset = 4680548,
};

MapLayout Underwater_Route105_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE105",
    .width = 40,
    .height = 80,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route105/border.bin",
    .blockdata_offset = 4682200,
};

MapLayout Underwater_Route125_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE125",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route125/border.bin",
    .blockdata_offset = 4688632,
};

MapLayout Underwater_Route129_Layout = {
    .id = "LAYOUT_UNDERWATER_ROUTE129",
    .width = 80,
    .height = 40,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Underwater,
    .border_filepath = "data/layouts/Underwater_Route129/border.bin",
    .blockdata_offset = 4695064,
};

MapLayout MarineCave_End_Layout = {
    .id = "LAYOUT_MARINE_CAVE_END",
    .width = 27,
    .height = 30,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/MarineCave_End/border.bin",
    .blockdata_offset = 4701496,
};

MapLayout TrainerHill_Entrance_Layout = {
    .id = "LAYOUT_TRAINER_HILL_ENTRANCE",
    .width = 19,
    .height = 17,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrainerHill,
    .border_filepath = "data/layouts/TrainerHill_Entrance/border.bin",
    .blockdata_offset = 4703148,
};

MapLayout TrainerHill_1F_Layout = {
    .id = "LAYOUT_TRAINER_HILL_1F",
    .width = 16,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrainerHill,
    .border_filepath = "data/layouts/TrainerHill_1F/border.bin",
    .blockdata_offset = 4703828,
};

MapLayout TrainerHill_2F_Layout = {
    .id = "LAYOUT_TRAINER_HILL_2F",
    .width = 16,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrainerHill,
    .border_filepath = "data/layouts/TrainerHill_2F/border.bin",
    .blockdata_offset = 4704532,
};

MapLayout TrainerHill_3F_Layout = {
    .id = "LAYOUT_TRAINER_HILL_3F",
    .width = 16,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrainerHill,
    .border_filepath = "data/layouts/TrainerHill_3F/border.bin",
    .blockdata_offset = 4705236,
};

MapLayout TrainerHill_4F_Layout = {
    .id = "LAYOUT_TRAINER_HILL_4F",
    .width = 16,
    .height = 21,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrainerHill,
    .border_filepath = "data/layouts/TrainerHill_4F/border.bin",
    .blockdata_offset = 4705940,
};

MapLayout TrainerHill_Roof_Layout = {
    .id = "LAYOUT_TRAINER_HILL_ROOF",
    .width = 25,
    .height = 16,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_TrainerHill,
    .border_filepath = "data/layouts/TrainerHill_Roof/border.bin",
    .blockdata_offset = 4706644,
};

MapLayout AlteringCave_Layout = {
    .id = "LAYOUT_ALTERING_CAVE",
    .width = 32,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Cave,
    .border_filepath = "data/layouts/AlteringCave/border.bin",
    .blockdata_offset = 4707476,
};

MapLayout NavelRock_Exterior_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_EXTERIOR",
    .width = 21,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Dewford,
    .border_filepath = "data/layouts/NavelRock_Exterior/border.bin",
    .blockdata_offset = 4709044,
};

MapLayout NavelRock_Entrance_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_ENTRANCE",
    .width = 21,
    .height = 32,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_NavelRock,
    .border_filepath = "data/layouts/NavelRock_Entrance/border.bin",
    .blockdata_offset = 4710084,
};

MapLayout NavelRock_Top_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_TOP",
    .width = 25,
    .height = 28,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_NavelRock,
    .border_filepath = "data/layouts/NavelRock_Top/border.bin",
    .blockdata_offset = 4711460,
};

MapLayout NavelRock_Bottom_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_BOTTOM",
    .width = 22,
    .height = 22,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_NavelRock,
    .border_filepath = "data/layouts/NavelRock_Bottom/border.bin",
    .blockdata_offset = 4712892,
};

MapLayout NavelRock_LadderRoom1_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_LADDER_ROOM1",
    .width = 9,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_NavelRock,
    .border_filepath = "data/layouts/NavelRock_LadderRoom1/border.bin",
    .blockdata_offset = 4713892,
};

MapLayout NavelRock_LadderRoom2_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_LADDER_ROOM2",
    .width = 9,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_NavelRock,
    .border_filepath = "data/layouts/NavelRock_LadderRoom2/border.bin",
    .blockdata_offset = 4714068,
};

MapLayout NavelRock_B1F_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_B1F",
    .width = 23,
    .height = 11,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_NavelRock,
    .border_filepath = "data/layouts/NavelRock_B1F/border.bin",
    .blockdata_offset = 4714244,
};

MapLayout NavelRock_Fork_Layout = {
    .id = "LAYOUT_NAVEL_ROCK_FORK",
    .width = 27,
    .height = 86,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_NavelRock,
    .border_filepath = "data/layouts/NavelRock_Fork/border.bin",
    .blockdata_offset = 4714784,
};

MapLayout BattleFrontier_Lounge2_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_LOUNGE2",
    .width = 9,
    .height = 10,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_Lounge2/border.bin",
    .blockdata_offset = 4719460,
};

MapLayout BattleFrontier_ScottsHouse_Layout = {
    .id = "LAYOUT_BATTLE_FRONTIER_SCOTTS_HOUSE",
    .width = 6,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_BattleFrontier,
    .border_filepath = "data/layouts/BattleFrontier_ScottsHouse/border.bin",
    .blockdata_offset = 4719672,
};

MapLayout MeteorFalls_StevensCave_Layout = {
    .id = "LAYOUT_METEOR_FALLS_STEVENS_CAVE",
    .width = 30,
    .height = 32,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_MeteorFalls,
    .border_filepath = "data/layouts/MeteorFalls_StevensCave/border.bin",
    .blockdata_offset = 4719800,
};

MapLayout LittlerootTown_ProfessorBirchsLabWithTable_Layout = {
    .id = "LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB_WITH_TABLE",
    .width = 13,
    .height = 13,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_Lab,
    .border_filepath = "data/layouts/LittlerootTown_ProfessorBirchsLabWithTable/border.bin",
    .blockdata_offset = 4721752,
};

MapLayout SkyPillar_1F_Clean_Layout = {
    .id = "LAYOUT_SKY_PILLAR_1F_CLEAN",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_1F_Clean/border.bin",
    .blockdata_offset = 4722124,
};

MapLayout SkyPillar_2F_Clean_Layout = {
    .id = "LAYOUT_SKY_PILLAR_2F_CLEAN",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_2F_Clean/border.bin",
    .blockdata_offset = 4722548,
};

MapLayout SkyPillar_3F_Clean_Layout = {
    .id = "LAYOUT_SKY_PILLAR_3F_CLEAN",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_3F_Clean/border.bin",
    .blockdata_offset = 4722972,
};

MapLayout SkyPillar_4F_Clean_Layout = {
    .id = "LAYOUT_SKY_PILLAR_4F_CLEAN",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_4F_Clean/border.bin",
    .blockdata_offset = 4723396,
};

MapLayout SkyPillar_5F_Clean_Layout = {
    .id = "LAYOUT_SKY_PILLAR_5F_CLEAN",
    .width = 14,
    .height = 14,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_5F_Clean/border.bin",
    .blockdata_offset = 4723820,
};

MapLayout SkyPillar_Top_Clean_Layout = {
    .id = "LAYOUT_SKY_PILLAR_TOP_CLEAN",
    .width = 27,
    .height = 24,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_General,
    .secondary_tileset = &gTileset_Pacifidlog,
    .border_filepath = "data/layouts/SkyPillar_Top_Clean/border.bin",
    .blockdata_offset = 4724244,
};

MapLayout SootopolisCity_MysteryEventsHouse_1F_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_MYSTERY_EVENTS_HOUSE_1F",
    .width = 11,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_MysteryEventsHouse,
    .border_filepath = "data/layouts/SootopolisCity_MysteryEventsHouse_1F/border.bin",
    .blockdata_offset = 4725572,
};

MapLayout SootopolisCity_MysteryEventsHouse_B1F_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_MYSTERY_EVENTS_HOUSE_B1F",
    .width = 12,
    .height = 9,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_MysteryEventsHouse,
    .border_filepath = "data/layouts/SootopolisCity_MysteryEventsHouse_B1F/border.bin",
    .blockdata_offset = 4725780,
};

MapLayout SootopolisCity_MysteryEventsHouse_1F_StairsUnblocked_Layout = {
    .id = "LAYOUT_SOOTOPOLIS_CITY_MYSTERY_EVENTS_HOUSE_1F_STAIRS_UNBLOCKED",
    .width = 11,
    .height = 8,
    .overworld_pos_x = -1,
    .overworld_pos_y = -1,
    .primary_tileset = &gTileset_Building,
    .secondary_tileset = &gTileset_MysteryEventsHouse,
    .border_filepath = "data/layouts/SootopolisCity_MysteryEventsHouse_1F_StairsUnblocked/border.bin",
    .blockdata_offset = 4726028,
};

