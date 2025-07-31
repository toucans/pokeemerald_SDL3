
#pragma once
#include <stdbool.h>
#include "map_layouts.h"

typedef struct Map Map;
typedef struct MapConnection MapConnection;
struct MapConnection {
    const Map *map;
    u16 offset;
    const char *direction;
};

typedef struct {
    const char *local_id;
    const char *graphics_id;
    int x;
    int y;
    int elevation;
    const char *movement_type;
    int movement_range_x;
    int movement_range_y;
    const char *trainer_type;
    const char *trainer_sight_or_berry_tree_id;
    const char *script;
    const char *flag;
} ObjectEvent;

typedef struct {
    int x;
    int y;
    int elevation;
    const char *dest_map;
    const char *dest_warp_id;
} WarpEvent;

typedef struct {
    const char *type;
    int x;
    int y;
    int elevation;
    const char *var;
    const char *var_value;
    const char *script;
} CoordEvent;

typedef struct {
    const char *type;
    int x;
    int y;
    int elevation;
    const char *player_facing_dir;
    const char *script;
} BgEvent;

struct Map {
    const char *id;
    const char *name;
    MapLayout *layout;
    const char *music;
    const char *region_map_section;
    bool requires_flash;
    const char *weather;
    const char *map_type;
    bool allow_cycling;
    bool allow_escaping;
    bool allow_running;
    bool show_map_name;
    const char *battle_scene;

    MapConnection *connections;
    u8 connections_count;

    ObjectEvent *object_events;
    int object_events_count;

    WarpEvent *warp_events;
    int warp_events_count;

    CoordEvent *coord_events;
    int coord_events_count;

    BgEvent *bg_events;
    int bg_events_count;
};

extern const Map **MapGroups[];

enum MapGroupId {
    MAP_GROUP_TOWNS_AND_ROUTES,
    MAP_GROUP_INDOOR_LITTLEROOT,
    MAP_GROUP_INDOOR_OLDALE,
    MAP_GROUP_INDOOR_DEWFORD,
    MAP_GROUP_INDOOR_LAVARIDGE,
    MAP_GROUP_INDOOR_FALLARBOR,
    MAP_GROUP_INDOOR_VERDANTURF,
    MAP_GROUP_INDOOR_PACIFIDLOG,
    MAP_GROUP_INDOOR_PETALBURG,
    MAP_GROUP_INDOOR_SLATEPORT,
    MAP_GROUP_INDOOR_MAUVILLE,
    MAP_GROUP_INDOOR_RUSTBORO,
    MAP_GROUP_INDOOR_FORTREE,
    MAP_GROUP_INDOOR_LILYCOVE,
    MAP_GROUP_INDOOR_MOSSDEEP,
    MAP_GROUP_INDOOR_SOOTOPOLIS,
    MAP_GROUP_INDOOR_EVER_GRANDE,
    MAP_GROUP_INDOOR_ROUTE_104,
    MAP_GROUP_INDOOR_ROUTE_111,
    MAP_GROUP_INDOOR_ROUTE_112,
    MAP_GROUP_INDOOR_ROUTE_114,
    MAP_GROUP_INDOOR_ROUTE_116,
    MAP_GROUP_INDOOR_ROUTE_117,
    MAP_GROUP_INDOOR_ROUTE_121,
    MAP_GROUP_DUNGEONS,
    MAP_GROUP_INDOOR_DYNAMIC,
    MAP_GROUP_SPECIAL_AREA,
    MAP_GROUP_INDOOR_ROUTE_104_PROTOTYPE,
    MAP_GROUP_INDOOR_ROUTE_109,
    MAP_GROUP_INDOOR_ROUTE_110,
    MAP_GROUP_INDOOR_ROUTE_113,
    MAP_GROUP_INDOOR_ROUTE_123,
    MAP_GROUP_INDOOR_ROUTE_119,
    MAP_GROUP_INDOOR_ROUTE_124,

    MAP_GROUP_COUNT
};

enum TownsAndRoutesMapIds {
    MAP_PETALBURG_CITY = 0,
    MAP_SLATEPORT_CITY = 1,
    MAP_MAUVILLE_CITY = 2,
    MAP_RUSTBORO_CITY = 3,
    MAP_FORTREE_CITY = 4,
    MAP_LILYCOVE_CITY = 5,
    MAP_MOSSDEEP_CITY = 6,
    MAP_SOOTOPOLIS_CITY = 7,
    MAP_EVER_GRANDE_CITY = 8,
    MAP_LITTLEROOT_TOWN = 9,
    MAP_OLDALE_TOWN = 10,
    MAP_DEWFORD_TOWN = 11,
    MAP_LAVARIDGE_TOWN = 12,
    MAP_FALLARBOR_TOWN = 13,
    MAP_VERDANTURF_TOWN = 14,
    MAP_PACIFIDLOG_TOWN = 15,
    MAP_ROUTE101 = 16,
    MAP_ROUTE102 = 17,
    MAP_ROUTE103 = 18,
    MAP_ROUTE104 = 19,
    MAP_ROUTE105 = 20,
    MAP_ROUTE106 = 21,
    MAP_ROUTE107 = 22,
    MAP_ROUTE108 = 23,
    MAP_ROUTE109 = 24,
    MAP_ROUTE110 = 25,
    MAP_ROUTE111 = 26,
    MAP_ROUTE112 = 27,
    MAP_ROUTE113 = 28,
    MAP_ROUTE114 = 29,
    MAP_ROUTE115 = 30,
    MAP_ROUTE116 = 31,
    MAP_ROUTE117 = 32,
    MAP_ROUTE118 = 33,
    MAP_ROUTE119 = 34,
    MAP_ROUTE120 = 35,
    MAP_ROUTE121 = 36,
    MAP_ROUTE122 = 37,
    MAP_ROUTE123 = 38,
    MAP_ROUTE124 = 39,
    MAP_ROUTE125 = 40,
    MAP_ROUTE126 = 41,
    MAP_ROUTE127 = 42,
    MAP_ROUTE128 = 43,
    MAP_ROUTE129 = 44,
    MAP_ROUTE130 = 45,
    MAP_ROUTE131 = 46,
    MAP_ROUTE132 = 47,
    MAP_ROUTE133 = 48,
    MAP_ROUTE134 = 49,
    MAP_UNDERWATER_ROUTE124 = 50,
    MAP_UNDERWATER_ROUTE126 = 51,
    MAP_UNDERWATER_ROUTE127 = 52,
    MAP_UNDERWATER_ROUTE128 = 53,
    MAP_UNDERWATER_ROUTE129 = 54,
    MAP_UNDERWATER_ROUTE105 = 55,
    MAP_UNDERWATER_ROUTE125 = 56,

    TOWNS_AND_ROUTES_COUNT
};

enum IndoorLittlerootMapIds {
    MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F = 0,
    MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F = 1,
    MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F = 2,
    MAP_LITTLEROOT_TOWN_MAYS_HOUSE_2F = 3,
    MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB = 4,
};

enum IndoorOldaleMapIds {
    MAP_OLDALE_TOWN_HOUSE1 = 0,
    MAP_OLDALE_TOWN_HOUSE2 = 1,
    MAP_OLDALE_TOWN_POKEMON_CENTER_1F = 2,
    MAP_OLDALE_TOWN_POKEMON_CENTER_2F = 3,
    MAP_OLDALE_TOWN_MART = 4,
};

enum IndoorDewfordMapIds {
    MAP_DEWFORD_TOWN_HOUSE1 = 0,
    MAP_DEWFORD_TOWN_POKEMON_CENTER_1F = 1,
    MAP_DEWFORD_TOWN_POKEMON_CENTER_2F = 2,
    MAP_DEWFORD_TOWN_GYM = 3,
    MAP_DEWFORD_TOWN_HALL = 4,
    MAP_DEWFORD_TOWN_HOUSE2 = 5,
};

enum IndoorLavaridgeMapIds {
    MAP_LAVARIDGE_TOWN_HERB_SHOP = 0,
    MAP_LAVARIDGE_TOWN_GYM_1F = 1,
    MAP_LAVARIDGE_TOWN_GYM_B1F = 2,
    MAP_LAVARIDGE_TOWN_HOUSE = 3,
    MAP_LAVARIDGE_TOWN_MART = 4,
    MAP_LAVARIDGE_TOWN_POKEMON_CENTER_1F = 5,
    MAP_LAVARIDGE_TOWN_POKEMON_CENTER_2F = 6,
};

enum IndoorFallarborMapIds {
    MAP_FALLARBOR_TOWN_MART = 0,
    MAP_FALLARBOR_TOWN_BATTLE_TENT_LOBBY = 1,
    MAP_FALLARBOR_TOWN_BATTLE_TENT_CORRIDOR = 2,
    MAP_FALLARBOR_TOWN_BATTLE_TENT_BATTLE_ROOM = 3,
    MAP_FALLARBOR_TOWN_POKEMON_CENTER_1F = 4,
    MAP_FALLARBOR_TOWN_POKEMON_CENTER_2F = 5,
    MAP_FALLARBOR_TOWN_COZMOS_HOUSE = 6,
    MAP_FALLARBOR_TOWN_MOVE_RELEARNERS_HOUSE = 7,
};

enum IndoorVerdanturfMapIds {
    MAP_VERDANTURF_TOWN_BATTLE_TENT_LOBBY = 0,
    MAP_VERDANTURF_TOWN_BATTLE_TENT_CORRIDOR = 1,
    MAP_VERDANTURF_TOWN_BATTLE_TENT_BATTLE_ROOM = 2,
    MAP_VERDANTURF_TOWN_MART = 3,
    MAP_VERDANTURF_TOWN_POKEMON_CENTER_1F = 4,
    MAP_VERDANTURF_TOWN_POKEMON_CENTER_2F = 5,
    MAP_VERDANTURF_TOWN_WANDAS_HOUSE = 6,
    MAP_VERDANTURF_TOWN_FRIENDSHIP_RATERS_HOUSE = 7,
    MAP_VERDANTURF_TOWN_HOUSE = 8,
};

enum IndoorPacifidlogMapIds {
    MAP_PACIFIDLOG_TOWN_POKEMON_CENTER_1F = 0,
    MAP_PACIFIDLOG_TOWN_POKEMON_CENTER_2F = 1,
    MAP_PACIFIDLOG_TOWN_HOUSE1 = 2,
    MAP_PACIFIDLOG_TOWN_HOUSE2 = 3,
    MAP_PACIFIDLOG_TOWN_HOUSE3 = 4,
    MAP_PACIFIDLOG_TOWN_HOUSE4 = 5,
    MAP_PACIFIDLOG_TOWN_HOUSE5 = 6,
};

enum IndoorPetalburgMapIds {
    MAP_PETALBURG_CITY_WALLYS_HOUSE = 0,
    MAP_PETALBURG_CITY_GYM = 1,
    MAP_PETALBURG_CITY_HOUSE1 = 2,
    MAP_PETALBURG_CITY_HOUSE2 = 3,
    MAP_PETALBURG_CITY_POKEMON_CENTER_1F = 4,
    MAP_PETALBURG_CITY_POKEMON_CENTER_2F = 5,
    MAP_PETALBURG_CITY_MART = 6,
};

enum IndoorSlateportMapIds {
    MAP_SLATEPORT_CITY_STERNS_SHIPYARD_1F = 0,
    MAP_SLATEPORT_CITY_STERNS_SHIPYARD_2F = 1,
    MAP_SLATEPORT_CITY_BATTLE_TENT_LOBBY = 2,
    MAP_SLATEPORT_CITY_BATTLE_TENT_CORRIDOR = 3,
    MAP_SLATEPORT_CITY_BATTLE_TENT_BATTLE_ROOM = 4,
    MAP_SLATEPORT_CITY_NAME_RATERS_HOUSE = 5,
    MAP_SLATEPORT_CITY_POKEMON_FAN_CLUB = 6,
    MAP_SLATEPORT_CITY_OCEANIC_MUSEUM_1F = 7,
    MAP_SLATEPORT_CITY_OCEANIC_MUSEUM_2F = 8,
    MAP_SLATEPORT_CITY_HARBOR = 9,
    MAP_SLATEPORT_CITY_HOUSE = 10,
    MAP_SLATEPORT_CITY_POKEMON_CENTER_1F = 11,
    MAP_SLATEPORT_CITY_POKEMON_CENTER_2F = 12,
    MAP_SLATEPORT_CITY_MART = 13,
};

enum IndoorMauvilleMapIds {
    MAP_MAUVILLE_CITY_GYM = 0,
    MAP_MAUVILLE_CITY_BIKE_SHOP = 1,
    MAP_MAUVILLE_CITY_HOUSE1 = 2,
    MAP_MAUVILLE_CITY_GAME_CORNER = 3,
    MAP_MAUVILLE_CITY_HOUSE2 = 4,
    MAP_MAUVILLE_CITY_POKEMON_CENTER_1F = 5,
    MAP_MAUVILLE_CITY_POKEMON_CENTER_2F = 6,
    MAP_MAUVILLE_CITY_MART = 7,
};

enum IndoorRustboroMapIds {
    MAP_RUSTBORO_CITY_DEVON_CORP_1F = 0,
    MAP_RUSTBORO_CITY_DEVON_CORP_2F = 1,
    MAP_RUSTBORO_CITY_DEVON_CORP_3F = 2,
    MAP_RUSTBORO_CITY_GYM = 3,
    MAP_RUSTBORO_CITY_POKEMON_SCHOOL = 4,
    MAP_RUSTBORO_CITY_POKEMON_CENTER_1F = 5,
    MAP_RUSTBORO_CITY_POKEMON_CENTER_2F = 6,
    MAP_RUSTBORO_CITY_MART = 7,
    MAP_RUSTBORO_CITY_FLAT1_1F = 8,
    MAP_RUSTBORO_CITY_FLAT1_2F = 9,
    MAP_RUSTBORO_CITY_HOUSE1 = 10,
    MAP_RUSTBORO_CITY_CUTTERS_HOUSE = 11,
    MAP_RUSTBORO_CITY_HOUSE2 = 12,
    MAP_RUSTBORO_CITY_FLAT2_1F = 13,
    MAP_RUSTBORO_CITY_FLAT2_2F = 14,
    MAP_RUSTBORO_CITY_FLAT2_3F = 15,
    MAP_RUSTBORO_CITY_HOUSE3 = 16,
};

enum IndoorFortreeMapIds {
    MAP_FORTREE_CITY_HOUSE1 = 0,
    MAP_FORTREE_CITY_GYM = 1,
    MAP_FORTREE_CITY_POKEMON_CENTER_1F = 2,
    MAP_FORTREE_CITY_POKEMON_CENTER_2F = 3,
    MAP_FORTREE_CITY_MART = 4,
    MAP_FORTREE_CITY_HOUSE2 = 5,
    MAP_FORTREE_CITY_HOUSE3 = 6,
    MAP_FORTREE_CITY_HOUSE4 = 7,
    MAP_FORTREE_CITY_HOUSE5 = 8,
    MAP_FORTREE_CITY_DECORATION_SHOP = 9,
};

enum IndoorLilycoveMapIds {
    MAP_LILYCOVE_CITY_COVE_LILY_MOTEL_1F = 0,
    MAP_LILYCOVE_CITY_COVE_LILY_MOTEL_2F = 1,
    MAP_LILYCOVE_CITY_LILYCOVE_MUSEUM_1F = 2,
    MAP_LILYCOVE_CITY_LILYCOVE_MUSEUM_2F = 3,
    MAP_LILYCOVE_CITY_CONTEST_LOBBY = 4,
    MAP_LILYCOVE_CITY_CONTEST_HALL = 5,
    MAP_LILYCOVE_CITY_POKEMON_CENTER_1F = 6,
    MAP_LILYCOVE_CITY_POKEMON_CENTER_2F = 7,
    MAP_LILYCOVE_CITY_UNUSED_MART = 8,
    MAP_LILYCOVE_CITY_POKEMON_TRAINER_FAN_CLUB = 9,
    MAP_LILYCOVE_CITY_HARBOR = 10,
    MAP_LILYCOVE_CITY_MOVE_DELETERS_HOUSE = 11,
    MAP_LILYCOVE_CITY_HOUSE1 = 12,
    MAP_LILYCOVE_CITY_HOUSE2 = 13,
    MAP_LILYCOVE_CITY_HOUSE3 = 14,
    MAP_LILYCOVE_CITY_HOUSE4 = 15,
    MAP_LILYCOVE_CITY_DEPARTMENT_STORE_1F = 16,
    MAP_LILYCOVE_CITY_DEPARTMENT_STORE_2F = 17,
    MAP_LILYCOVE_CITY_DEPARTMENT_STORE_3F = 18,
    MAP_LILYCOVE_CITY_DEPARTMENT_STORE_4F = 19,
    MAP_LILYCOVE_CITY_DEPARTMENT_STORE_5F = 20,
    MAP_LILYCOVE_CITY_DEPARTMENT_STORE_ROOFTOP = 21,
    MAP_LILYCOVE_CITY_DEPARTMENT_STORE_ELEVATOR = 22,
};

enum IndoorMossdeepMapIds {
    MAP_MOSSDEEP_CITY_GYM = 0,
    MAP_MOSSDEEP_CITY_HOUSE1 = 1,
    MAP_MOSSDEEP_CITY_HOUSE2 = 2,
    MAP_MOSSDEEP_CITY_POKEMON_CENTER_1F = 3,
    MAP_MOSSDEEP_CITY_POKEMON_CENTER_2F = 4,
    MAP_MOSSDEEP_CITY_MART = 5,
    MAP_MOSSDEEP_CITY_HOUSE3 = 6,
    MAP_MOSSDEEP_CITY_STEVENS_HOUSE = 7,
    MAP_MOSSDEEP_CITY_HOUSE4 = 8,
    MAP_MOSSDEEP_CITY_SPACE_CENTER_1F = 9,
    MAP_MOSSDEEP_CITY_SPACE_CENTER_2F = 10,
    MAP_MOSSDEEP_CITY_GAME_CORNER_1F = 11,
    MAP_MOSSDEEP_CITY_GAME_CORNER_B1F = 12,
};

enum IndoorSootopolisMapIds {
    MAP_SOOTOPOLIS_CITY_GYM_1F = 0,
    MAP_SOOTOPOLIS_CITY_GYM_B1F = 1,
    MAP_SOOTOPOLIS_CITY_POKEMON_CENTER_1F = 2,
    MAP_SOOTOPOLIS_CITY_POKEMON_CENTER_2F = 3,
    MAP_SOOTOPOLIS_CITY_MART = 4,
    MAP_SOOTOPOLIS_CITY_HOUSE1 = 5,
    MAP_SOOTOPOLIS_CITY_HOUSE2 = 6,
    MAP_SOOTOPOLIS_CITY_HOUSE3 = 7,
    MAP_SOOTOPOLIS_CITY_HOUSE4 = 8,
    MAP_SOOTOPOLIS_CITY_HOUSE5 = 9,
    MAP_SOOTOPOLIS_CITY_HOUSE6 = 10,
    MAP_SOOTOPOLIS_CITY_HOUSE7 = 11,
    MAP_SOOTOPOLIS_CITY_LOTAD_AND_SEEDOT_HOUSE = 12,
    MAP_SOOTOPOLIS_CITY_MYSTERY_EVENTS_HOUSE_1F = 13,
    MAP_SOOTOPOLIS_CITY_MYSTERY_EVENTS_HOUSE_B1F = 14,
};

enum IndoorEverGrandeMapIds {
    MAP_EVER_GRANDE_CITY_SIDNEYS_ROOM = 0,
    MAP_EVER_GRANDE_CITY_PHOEBES_ROOM = 1,
    MAP_EVER_GRANDE_CITY_GLACIAS_ROOM = 2,
    MAP_EVER_GRANDE_CITY_DRAKES_ROOM = 3,
    MAP_EVER_GRANDE_CITY_CHAMPIONS_ROOM = 4,
    MAP_EVER_GRANDE_CITY_HALL1 = 5,
    MAP_EVER_GRANDE_CITY_HALL2 = 6,
    MAP_EVER_GRANDE_CITY_HALL3 = 7,
    MAP_EVER_GRANDE_CITY_HALL4 = 8,
    MAP_EVER_GRANDE_CITY_HALL5 = 9,
    MAP_EVER_GRANDE_CITY_POKEMON_LEAGUE_1F = 10,
    MAP_EVER_GRANDE_CITY_HALL_OF_FAME = 11,
    MAP_EVER_GRANDE_CITY_POKEMON_CENTER_1F = 12,
    MAP_EVER_GRANDE_CITY_POKEMON_CENTER_2F = 13,
    MAP_EVER_GRANDE_CITY_POKEMON_LEAGUE_2F = 14,
};

enum IndoorRoute104MapIds {
    MAP_ROUTE104_MR_BRINEYS_HOUSE = 0,
    MAP_ROUTE104_PRETTY_PETAL_FLOWER_SHOP = 1,
};

enum IndoorRoute111MapIds {
    MAP_ROUTE111_WINSTRATE_FAMILYS_HOUSE = 0,
    MAP_ROUTE111_OLD_LADYS_REST_STOP = 1,
};

enum IndoorRoute112MapIds {
    MAP_ROUTE112_CABLE_CAR_STATION = 0,
    MAP_MT_CHIMNEY_CABLE_CAR_STATION = 1,
};

enum IndoorRoute114MapIds {
    MAP_ROUTE114_FOSSIL_MANIACS_HOUSE = 0,
    MAP_ROUTE114_FOSSIL_MANIACS_TUNNEL = 1,
    MAP_ROUTE114_LANETTES_HOUSE = 2,
};

enum IndoorRoute116MapIds {
    MAP_ROUTE116_TUNNELERS_REST_HOUSE = 0,
};

enum IndoorRoute117MapIds {
    MAP_ROUTE117_POKEMON_DAY_CARE = 0,
};

enum IndoorRoute121MapIds {
    MAP_ROUTE121_SAFARI_ZONE_ENTRANCE = 0,
};

enum DungeonsMapIds {
    MAP_METEOR_FALLS_1F_1R = 0,
    MAP_METEOR_FALLS_1F_2R = 1,
    MAP_METEOR_FALLS_B1F_1R = 2,
    MAP_METEOR_FALLS_B1F_2R = 3,
    MAP_RUSTURF_TUNNEL = 4,
    MAP_UNDERWATER_SOOTOPOLIS_CITY = 5,
    MAP_DESERT_RUINS = 6,
    MAP_GRANITE_CAVE_1F = 7,
    MAP_GRANITE_CAVE_B1F = 8,
    MAP_GRANITE_CAVE_B2F = 9,
    MAP_GRANITE_CAVE_STEVENS_ROOM = 10,
    MAP_PETALBURG_WOODS = 11,
    MAP_MT_CHIMNEY = 12,
    MAP_JAGGED_PASS = 13,
    MAP_FIERY_PATH = 14,
    MAP_MT_PYRE_1F = 15,
    MAP_MT_PYRE_2F = 16,
    MAP_MT_PYRE_3F = 17,
    MAP_MT_PYRE_4F = 18,
    MAP_MT_PYRE_5F = 19,
    MAP_MT_PYRE_6F = 20,
    MAP_MT_PYRE_EXTERIOR = 21,
    MAP_MT_PYRE_SUMMIT = 22,
    MAP_AQUA_HIDEOUT_1F = 23,
    MAP_AQUA_HIDEOUT_B1F = 24,
    MAP_AQUA_HIDEOUT_B2F = 25,
    MAP_UNDERWATER_SEAFLOOR_CAVERN = 26,
    MAP_SEAFLOOR_CAVERN_ENTRANCE = 27,
    MAP_SEAFLOOR_CAVERN_ROOM1 = 28,
    MAP_SEAFLOOR_CAVERN_ROOM2 = 29,
    MAP_SEAFLOOR_CAVERN_ROOM3 = 30,
    MAP_SEAFLOOR_CAVERN_ROOM4 = 31,
    MAP_SEAFLOOR_CAVERN_ROOM5 = 32,
    MAP_SEAFLOOR_CAVERN_ROOM6 = 33,
    MAP_SEAFLOOR_CAVERN_ROOM7 = 34,
    MAP_SEAFLOOR_CAVERN_ROOM8 = 35,
    MAP_SEAFLOOR_CAVERN_ROOM9 = 36,
    MAP_CAVE_OF_ORIGIN_ENTRANCE = 37,
    MAP_CAVE_OF_ORIGIN_1F = 38,
    MAP_CAVE_OF_ORIGIN_UNUSED_RUBY_SAPPHIRE_MAP1 = 39,
    MAP_CAVE_OF_ORIGIN_UNUSED_RUBY_SAPPHIRE_MAP2 = 40,
    MAP_CAVE_OF_ORIGIN_UNUSED_RUBY_SAPPHIRE_MAP3 = 41,
    MAP_CAVE_OF_ORIGIN_B1F = 42,
    MAP_VICTORY_ROAD_1F = 43,
    MAP_VICTORY_ROAD_B1F = 44,
    MAP_VICTORY_ROAD_B2F = 45,
    MAP_SHOAL_CAVE_LOW_TIDE_ENTRANCE_ROOM = 46,
    MAP_SHOAL_CAVE_LOW_TIDE_INNER_ROOM = 47,
    MAP_SHOAL_CAVE_LOW_TIDE_STAIRS_ROOM = 48,
    MAP_SHOAL_CAVE_LOW_TIDE_LOWER_ROOM = 49,
    MAP_SHOAL_CAVE_HIGH_TIDE_ENTRANCE_ROOM = 50,
    MAP_SHOAL_CAVE_HIGH_TIDE_INNER_ROOM = 51,
    MAP_NEW_MAUVILLE_ENTRANCE = 52,
    MAP_NEW_MAUVILLE_INSIDE = 53,
    MAP_ABANDONED_SHIP_DECK = 54,
    MAP_ABANDONED_SHIP_CORRIDORS_1F = 55,
    MAP_ABANDONED_SHIP_ROOMS_1F = 56,
    MAP_ABANDONED_SHIP_CORRIDORS_B1F = 57,
    MAP_ABANDONED_SHIP_ROOMS_B1F = 58,
    MAP_ABANDONED_SHIP_ROOMS2_B1F = 59,
    MAP_ABANDONED_SHIP_UNDERWATER1 = 60,
    MAP_ABANDONED_SHIP_ROOM_B1F = 61,
    MAP_ABANDONED_SHIP_ROOMS2_1F = 62,
    MAP_ABANDONED_SHIP_CAPTAINS_OFFICE = 63,
    MAP_ABANDONED_SHIP_UNDERWATER2 = 64,
    MAP_ABANDONED_SHIP_HIDDEN_FLOOR_CORRIDORS = 65,
    MAP_ABANDONED_SHIP_HIDDEN_FLOOR_ROOMS = 66,
    MAP_ISLAND_CAVE = 67,
    MAP_ANCIENT_TOMB = 68,
    MAP_UNDERWATER_ROUTE134 = 69,
    MAP_UNDERWATER_SEALED_CHAMBER = 70,
    MAP_SEALED_CHAMBER_OUTER_ROOM = 71,
    MAP_SEALED_CHAMBER_INNER_ROOM = 72,
    MAP_SCORCHED_SLAB = 73,
    MAP_AQUA_HIDEOUT_UNUSED_RUBY_MAP1 = 74,
    MAP_AQUA_HIDEOUT_UNUSED_RUBY_MAP2 = 75,
    MAP_AQUA_HIDEOUT_UNUSED_RUBY_MAP3 = 76,
    MAP_SKY_PILLAR_ENTRANCE = 77,
    MAP_SKY_PILLAR_OUTSIDE = 78,
    MAP_SKY_PILLAR_1F = 79,
    MAP_SKY_PILLAR_2F = 80,
    MAP_SKY_PILLAR_3F = 81,
    MAP_SKY_PILLAR_4F = 82,
    MAP_SHOAL_CAVE_LOW_TIDE_ICE_ROOM = 83,
    MAP_SKY_PILLAR_5F = 84,
    MAP_SKY_PILLAR_TOP = 85,
    MAP_MAGMA_HIDEOUT_1F = 86,
    MAP_MAGMA_HIDEOUT_2F_1R = 87,
    MAP_MAGMA_HIDEOUT_2F_2R = 88,
    MAP_MAGMA_HIDEOUT_3F_1R = 89,
    MAP_MAGMA_HIDEOUT_3F_2R = 90,
    MAP_MAGMA_HIDEOUT_4F = 91,
    MAP_MAGMA_HIDEOUT_3F_3R = 92,
    MAP_MAGMA_HIDEOUT_2F_3R = 93,
    MAP_MIRAGE_TOWER_1F = 94,
    MAP_MIRAGE_TOWER_2F = 95,
    MAP_MIRAGE_TOWER_3F = 96,
    MAP_MIRAGE_TOWER_4F = 97,
    MAP_DESERT_UNDERPASS = 98,
    MAP_ARTISAN_CAVE_B1F = 99,
    MAP_ARTISAN_CAVE_1F = 100,
    MAP_UNDERWATER_MARINE_CAVE = 101,
    MAP_MARINE_CAVE_ENTRANCE = 102,
    MAP_MARINE_CAVE_END = 103,
    MAP_TERRA_CAVE_ENTRANCE = 104,
    MAP_TERRA_CAVE_END = 105,
    MAP_ALTERING_CAVE = 106,
    MAP_METEOR_FALLS_STEVENS_CAVE = 107,
};

enum IndoorDynamicMapIds {
    MAP_SECRET_BASE_RED_CAVE1 = 0,
    MAP_SECRET_BASE_BROWN_CAVE1 = 1,
    MAP_SECRET_BASE_BLUE_CAVE1 = 2,
    MAP_SECRET_BASE_YELLOW_CAVE1 = 3,
    MAP_SECRET_BASE_TREE1 = 4,
    MAP_SECRET_BASE_SHRUB1 = 5,
    MAP_SECRET_BASE_RED_CAVE2 = 6,
    MAP_SECRET_BASE_BROWN_CAVE2 = 7,
    MAP_SECRET_BASE_BLUE_CAVE2 = 8,
    MAP_SECRET_BASE_YELLOW_CAVE2 = 9,
    MAP_SECRET_BASE_TREE2 = 10,
    MAP_SECRET_BASE_SHRUB2 = 11,
    MAP_SECRET_BASE_RED_CAVE3 = 12,
    MAP_SECRET_BASE_BROWN_CAVE3 = 13,
    MAP_SECRET_BASE_BLUE_CAVE3 = 14,
    MAP_SECRET_BASE_YELLOW_CAVE3 = 15,
    MAP_SECRET_BASE_TREE3 = 16,
    MAP_SECRET_BASE_SHRUB3 = 17,
    MAP_SECRET_BASE_RED_CAVE4 = 18,
    MAP_SECRET_BASE_BROWN_CAVE4 = 19,
    MAP_SECRET_BASE_BLUE_CAVE4 = 20,
    MAP_SECRET_BASE_YELLOW_CAVE4 = 21,
    MAP_SECRET_BASE_TREE4 = 22,
    MAP_SECRET_BASE_SHRUB4 = 23,
    MAP_BATTLE_COLOSSEUM_2P = 24,
    MAP_TRADE_CENTER = 25,
    MAP_RECORD_CORNER = 26,
    MAP_BATTLE_COLOSSEUM_4P = 27,
    MAP_CONTEST_HALL = 28,
    MAP_UNUSED_CONTEST_HALL1 = 29,
    MAP_UNUSED_CONTEST_HALL2 = 30,
    MAP_UNUSED_CONTEST_HALL3 = 31,
    MAP_UNUSED_CONTEST_HALL4 = 32,
    MAP_UNUSED_CONTEST_HALL5 = 33,
    MAP_UNUSED_CONTEST_HALL6 = 34,
    MAP_CONTEST_HALL_BEAUTY = 35,
    MAP_CONTEST_HALL_TOUGH = 36,
    MAP_CONTEST_HALL_COOL = 37,
    MAP_CONTEST_HALL_SMART = 38,
    MAP_CONTEST_HALL_CUTE = 39,
    MAP_INSIDE_OF_TRUCK = 40,
    MAP_SS_TIDAL_CORRIDOR = 41,
    MAP_SS_TIDAL_LOWER_DECK = 42,
    MAP_SS_TIDAL_ROOMS = 43,
    MAP_BATTLE_PYRAMID_SQUARE01 = 44,
    MAP_BATTLE_PYRAMID_SQUARE02 = 45,
    MAP_BATTLE_PYRAMID_SQUARE03 = 46,
    MAP_BATTLE_PYRAMID_SQUARE04 = 47,
    MAP_BATTLE_PYRAMID_SQUARE05 = 48,
    MAP_BATTLE_PYRAMID_SQUARE06 = 49,
    MAP_BATTLE_PYRAMID_SQUARE07 = 50,
    MAP_BATTLE_PYRAMID_SQUARE08 = 51,
    MAP_BATTLE_PYRAMID_SQUARE09 = 52,
    MAP_BATTLE_PYRAMID_SQUARE10 = 53,
    MAP_BATTLE_PYRAMID_SQUARE11 = 54,
    MAP_BATTLE_PYRAMID_SQUARE12 = 55,
    MAP_BATTLE_PYRAMID_SQUARE13 = 56,
    MAP_BATTLE_PYRAMID_SQUARE14 = 57,
    MAP_BATTLE_PYRAMID_SQUARE15 = 58,
    MAP_BATTLE_PYRAMID_SQUARE16 = 59,
    MAP_UNION_ROOM = 60,
};

enum SpecialAreaMapIds {
    MAP_SAFARI_ZONE_NORTHWEST = 0,
    MAP_SAFARI_ZONE_NORTH = 1,
    MAP_SAFARI_ZONE_SOUTHWEST = 2,
    MAP_SAFARI_ZONE_SOUTH = 3,
    MAP_BATTLE_FRONTIER_OUTSIDE_WEST = 4,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY = 5,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_ELEVATOR = 6,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_CORRIDOR = 7,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_BATTLE_ROOM = 8,
    MAP_SOUTHERN_ISLAND_EXTERIOR = 9,
    MAP_SOUTHERN_ISLAND_INTERIOR = 10,
    MAP_SAFARI_ZONE_REST_HOUSE = 11,
    MAP_SAFARI_ZONE_NORTHEAST = 12,
    MAP_SAFARI_ZONE_SOUTHEAST = 13,
    MAP_BATTLE_FRONTIER_OUTSIDE_EAST = 14,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_PARTNER_ROOM = 15,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_CORRIDOR = 16,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_BATTLE_ROOM = 17,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY = 18,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_CORRIDOR = 19,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_PRE_BATTLE_ROOM = 20,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_BATTLE_ROOM = 21,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY = 22,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_CORRIDOR = 23,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_BATTLE_ROOM = 24,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY = 25,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR = 26,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP = 27,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY = 28,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_CORRIDOR = 29,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_BATTLE_ROOM = 30,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY = 31,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_PRE_BATTLE_ROOM = 32,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_BATTLE_ROOM = 33,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY = 34,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_CORRIDOR = 35,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_THREE_PATH_ROOM = 36,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_NORMAL = 37,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_FINAL = 38,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS = 39,
    MAP_BATTLE_FRONTIER_RANKING_HALL = 40,
    MAP_BATTLE_FRONTIER_LOUNGE1 = 41,
    MAP_BATTLE_FRONTIER_EXCHANGE_SERVICE_CORNER = 42,
    MAP_BATTLE_FRONTIER_LOUNGE2 = 43,
    MAP_BATTLE_FRONTIER_LOUNGE3 = 44,
    MAP_BATTLE_FRONTIER_LOUNGE4 = 45,
    MAP_BATTLE_FRONTIER_SCOTTS_HOUSE = 46,
    MAP_BATTLE_FRONTIER_LOUNGE5 = 47,
    MAP_BATTLE_FRONTIER_LOUNGE6 = 48,
    MAP_BATTLE_FRONTIER_LOUNGE7 = 49,
    MAP_BATTLE_FRONTIER_RECEPTION_GATE = 50,
    MAP_BATTLE_FRONTIER_LOUNGE8 = 51,
    MAP_BATTLE_FRONTIER_LOUNGE9 = 52,
    MAP_BATTLE_FRONTIER_POKEMON_CENTER_1F = 53,
    MAP_BATTLE_FRONTIER_POKEMON_CENTER_2F = 54,
    MAP_BATTLE_FRONTIER_MART = 55,
    MAP_FARAWAY_ISLAND_ENTRANCE = 56,
    MAP_FARAWAY_ISLAND_INTERIOR = 57,
    MAP_BIRTH_ISLAND_EXTERIOR = 58,
    MAP_BIRTH_ISLAND_HARBOR = 59,
    MAP_TRAINER_HILL_ENTRANCE = 60,
    MAP_TRAINER_HILL_1F = 61,
    MAP_TRAINER_HILL_2F = 62,
    MAP_TRAINER_HILL_3F = 63,
    MAP_TRAINER_HILL_4F = 64,
    MAP_TRAINER_HILL_ROOF = 65,
    MAP_NAVEL_ROCK_EXTERIOR = 66,
    MAP_NAVEL_ROCK_HARBOR = 67,
    MAP_NAVEL_ROCK_ENTRANCE = 68,
    MAP_NAVEL_ROCK_B1F = 69,
    MAP_NAVEL_ROCK_FORK = 70,
    MAP_NAVEL_ROCK_UP1 = 71,
    MAP_NAVEL_ROCK_UP2 = 72,
    MAP_NAVEL_ROCK_UP3 = 73,
    MAP_NAVEL_ROCK_UP4 = 74,
    MAP_NAVEL_ROCK_TOP = 75,
    MAP_NAVEL_ROCK_DOWN01 = 76,
    MAP_NAVEL_ROCK_DOWN02 = 77,
    MAP_NAVEL_ROCK_DOWN03 = 78,
    MAP_NAVEL_ROCK_DOWN04 = 79,
    MAP_NAVEL_ROCK_DOWN05 = 80,
    MAP_NAVEL_ROCK_DOWN06 = 81,
    MAP_NAVEL_ROCK_DOWN07 = 82,
    MAP_NAVEL_ROCK_DOWN08 = 83,
    MAP_NAVEL_ROCK_DOWN09 = 84,
    MAP_NAVEL_ROCK_DOWN10 = 85,
    MAP_NAVEL_ROCK_DOWN11 = 86,
    MAP_NAVEL_ROCK_BOTTOM = 87,
    MAP_TRAINER_HILL_ELEVATOR = 88,
};

enum IndoorRoute104PrototypeMapIds {
    MAP_ROUTE104_PROTOTYPE = 0,
    MAP_ROUTE104_PROTOTYPE_PRETTY_PETAL_FLOWER_SHOP = 1,
};

enum IndoorRoute109MapIds {
    MAP_ROUTE109_SEASHORE_HOUSE = 0,
};

enum IndoorRoute110MapIds {
    MAP_ROUTE110_TRICK_HOUSE_ENTRANCE = 0,
    MAP_ROUTE110_TRICK_HOUSE_END = 1,
    MAP_ROUTE110_TRICK_HOUSE_CORRIDOR = 2,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE1 = 3,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE2 = 4,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE3 = 5,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE4 = 6,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE5 = 7,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE6 = 8,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE7 = 9,
    MAP_ROUTE110_TRICK_HOUSE_PUZZLE8 = 10,
    MAP_ROUTE110_SEASIDE_CYCLING_ROAD_SOUTH_ENTRANCE = 11,
    MAP_ROUTE110_SEASIDE_CYCLING_ROAD_NORTH_ENTRANCE = 12,
};

enum IndoorRoute113MapIds {
    MAP_ROUTE113_GLASS_WORKSHOP = 0,
};

enum IndoorRoute123MapIds {
    MAP_ROUTE123_BERRY_MASTERS_HOUSE = 0,
};

enum IndoorRoute119MapIds {
    MAP_ROUTE119_WEATHER_INSTITUTE_1F = 0,
    MAP_ROUTE119_WEATHER_INSTITUTE_2F = 1,
    MAP_ROUTE119_HOUSE = 2,
};

enum IndoorRoute124MapIds {
    MAP_ROUTE124_DIVING_TREASURE_HUNTERS_HOUSE = 0,
};

extern const Map ContestHallCute;
extern const Map Route123_BerryMastersHouse;
extern const Map BattleFrontier_BattlePikeCorridor;
extern const Map MauvilleCity_Gym;
extern const Map BattleFrontier_BattleArenaBattleRoom;
extern const Map Route114_FossilManiacsHouse;
extern const Map BattleFrontier_ExchangeServiceCorner;
extern const Map MarineCave_Entrance;
extern const Map FortreeCity_Gym;
extern const Map NavelRock_Up3;
extern const Map NavelRock_Up4;
extern const Map LilycoveCity_LilycoveMuseum_2F;
extern const Map BattleFrontier_Lounge8;
extern const Map LilycoveCity_MoveDeletersHouse;
extern const Map Route110_SeasideCyclingRoadSouthEntrance;
extern const Map BattleFrontier_BattleTowerMultiCorridor;
extern const Map SeafloorCavern_Room9;
extern const Map Underwater_Route124;
extern const Map SouthernIsland_Exterior;
extern const Map VerdanturfTown;
extern const Map OldaleTown_PokemonCenter_1F;
extern const Map RustboroCity_DevonCorp_3F;
extern const Map LilycoveCity_DepartmentStore_5F;
extern const Map DewfordTown_Gym;
extern const Map BattleFrontier_Lounge6;
extern const Map TrainerHill_3F;
extern const Map BattleFrontier_Lounge1;
extern const Map SeafloorCavern_Room7;
extern const Map SootopolisCity_MysteryEventsHouse_B1F;
extern const Map NavelRock_Up2;
extern const Map MossdeepCity;
extern const Map BattleFrontier_BattlePikeRoomWildMons;
extern const Map FallarborTown;
extern const Map LilycoveCity_LilycoveMuseum_1F;
extern const Map MauvilleCity_House2;
extern const Map BattleFrontier_RankingHall;
extern const Map SeafloorCavern_Room6;
extern const Map BattleFrontier_OutsideWest;
extern const Map OldaleTown_PokemonCenter_2F;
extern const Map SootopolisCity_Mart;
extern const Map SlateportCity_BattleTentLobby;
extern const Map MauvilleCity_BikeShop;
extern const Map BattleFrontier_Lounge7;
extern const Map RustboroCity_House1;
extern const Map SeafloorCavern_Room1;
extern const Map SeafloorCavern_Room8;
extern const Map Underwater_Route125;
extern const Map Route111_WinstrateFamilysHouse;
extern const Map BattleFrontier_Lounge9;
extern const Map LavaridgeTown_Gym_B1F;
extern const Map CaveOfOrigin_B1F;
extern const Map MtPyre_3F;
extern const Map Route102;
extern const Map UnusedContestHall6;
extern const Map BirthIsland_Harbor;
extern const Map SecretBase_Tree1;
extern const Map UnusedContestHall1;
extern const Map Route105;
extern const Map ContestHallSmart;
extern const Map VerdanturfTown_Mart;
extern const Map FortreeCity_PokemonCenter_2F;
extern const Map NavelRock_Down05;
extern const Map Route133;
extern const Map SecretBase_YellowCave1;
extern const Map SlateportCity_PokemonCenter_2F;
extern const Map SootopolisCity_PokemonCenter_1F;
extern const Map MagmaHideout_2F_2R;
extern const Map Route134;
extern const Map BattleFrontier_BattleTowerCorridor;
extern const Map NavelRock_Down02;
extern const Map FortreeCity_House5;
extern const Map MagmaHideout_3F_2R;
extern const Map SlateportCity_House;
extern const Map SlateportCity_OceanicMuseum_1F;
extern const Map FortreeCity_House2;
extern const Map CaveOfOrigin_UnusedRubySapphireMap1;
extern const Map TrainerHill_Entrance;
extern const Map Route112_CableCarStation;
extern const Map CaveOfOrigin_Entrance;
extern const Map BattleFrontier_BattleFactoryPreBattleRoom;
extern const Map SecretBase_RedCave4;
extern const Map SSTidalCorridor;
extern const Map VerdanturfTown_BattleTentLobby;
extern const Map BattlePyramidSquare16;
extern const Map SSTidalLowerDeck;
extern const Map VerdanturfTown_BattleTentCorridor;
extern const Map Route109_SeashoreHouse;
extern const Map BattlePyramidSquare11;
extern const Map RustboroCity_Flat2_2F;
extern const Map SecretBase_RedCave3;
extern const Map BattleFrontier_BattleDomeCorridor;
extern const Map ShoalCave_HighTideEntranceRoom;
extern const Map SkyPillar_4F;
extern const Map SafariZone_Northeast;
extern const Map LavaridgeTown_Gym_1F;
extern const Map NavelRock_Down03;
extern const Map BattleFrontier_BattlePikeRoomNormal;
extern const Map SealedChamber_OuterRoom;
extern const Map ContestHallTough;
extern const Map Route132;
extern const Map TrainerHill_Elevator;
extern const Map NavelRock_Down04;
extern const Map SootopolisCity;
extern const Map MagmaHideout_2F_1R;
extern const Map SootopolisCity_PokemonCenter_2F;
extern const Map Route104;
extern const Map LilycoveCity_Harbor;
extern const Map FortreeCity_PokemonCenter_1F;
extern const Map SafariZone_Southwest;
extern const Map Route103;
extern const Map SkyPillar_Entrance;
extern const Map SlateportCity_PokemonCenter_1F;
extern const Map LilycoveCity_ContestLobby;
extern const Map MossdeepCity_GameCorner_B1F;
extern const Map SecretBase_RedCave2;
extern const Map SlateportCity_OceanicMuseum_2F;
extern const Map EverGrandeCity_PhoebesRoom;
extern const Map BattlePyramidSquare10;
extern const Map RustboroCity;
extern const Map ArtisanCave_1F;
extern const Map NavelRock_Entrance;
extern const Map MagmaHideout_3F_1R;
extern const Map FallarborTown_BattleTentCorridor;
extern const Map RustboroCity_Flat2_1F;
extern const Map FortreeCity_House3;
extern const Map MtPyre_Summit;
extern const Map PetalburgCity_Gym;
extern const Map BattleColosseum_4P;
extern const Map SafariZone_South;
extern const Map FortreeCity_House4;
extern const Map Route117_PokemonDayCare;
extern const Map LavaridgeTown;
extern const Map TradeCenter;
extern const Map AbandonedShip_Corridors_1F;
extern const Map SlateportCity_BattleTentBattleRoom;
extern const Map BattlePyramidSquare04;
extern const Map BattlePyramidSquare03;
extern const Map AbandonedShip_Underwater2;
extern const Map AquaHideout_UnusedRubyMap1;
extern const Map MauvilleCity_Mart;
extern const Map FortreeCity;
extern const Map MtPyre_4F;
extern const Map BattleFrontier_ScottsHouse;
extern const Map FallarborTown_CozmosHouse;
extern const Map MagmaHideout_4F;
extern const Map RustboroCity_PokemonCenter_1F;
extern const Map Route121;
extern const Map NavelRock_Down10;
extern const Map Route126;
extern const Map SlateportCity_SternsShipyard_2F;
extern const Map MirageTower_1F;
extern const Map Route119;
extern const Map LilycoveCity_UnusedMart;
extern const Map MossdeepCity_StevensHouse;
extern const Map SootopolisCity_House4;
extern const Map SootopolisCity_Gym_1F;
extern const Map Route110;
extern const Map BattleFrontier_OutsideEast;
extern const Map Route117;
extern const Map SootopolisCity_House3;
extern const Map Route128;
extern const Map ContestHall;
extern const Map SafariZone_RestHouse;
extern const Map LittlerootTown;
extern const Map BattleFrontier_BattleTowerMultiBattleRoom;
extern const Map PetalburgCity;
extern const Map FortreeCity_DecorationShop;
extern const Map ShoalCave_HighTideInnerRoom;
extern const Map SootopolisCity_Gym_B1F;
extern const Map Route104_PrettyPetalFlowerShop;
extern const Map RustboroCity_PokemonCenter_2F;
extern const Map SkyPillar_3F;
extern const Map LilycoveCity_House1;
extern const Map BattlePyramidSquare02;
extern const Map SkyPillar_Top;
extern const Map OldaleTown_House2;
extern const Map BattlePyramidSquare05;
extern const Map BattleFrontier_BattleArenaLobby;
extern const Map EverGrandeCity;
extern const Map SootopolisCity_House2;
extern const Map Route116;
extern const Map SlateportCity_SternsShipyard_1F;
extern const Map MirageTower_2F;
extern const Map Route129;
extern const Map SeafloorCavern_Entrance;
extern const Map Route111;
extern const Map SootopolisCity_House5;
extern const Map SecretBase_BrownCave1;
extern const Map PetalburgCity_Mart;
extern const Map AbandonedShip_Corridors_B1F;
extern const Map Route127;
extern const Map NavelRock_Down11;
extern const Map IslandCave;
extern const Map Route118;
extern const Map Underwater_SeafloorCavern;
extern const Map Route124_DivingTreasureHuntersHouse;
extern const Map Route120;
extern const Map NewMauville_Inside;
extern const Map SafariZone_Southeast;
extern const Map PetalburgCity_WallysHouse;
extern const Map LavaridgeTown_PokemonCenter_2F;
extern const Map EverGrandeCity_ChampionsRoom;
extern const Map EverGrandeCity_Hall4;
extern const Map PacifidlogTown_House2;
extern const Map Route110_TrickHousePuzzle5;
extern const Map PacifidlogTown_PokemonCenter_1F;
extern const Map Route121_SafariZoneEntrance;
extern const Map SafariZone_Northwest;
extern const Map Route110_TrickHousePuzzle2;
extern const Map PacifidlogTown_House5;
extern const Map EverGrandeCity_Hall3;
extern const Map BattleFrontier_BattlePyramidFloor;
extern const Map Underwater_MarineCave;
extern const Map LilycoveCity_DepartmentStore_2F;
extern const Map SealedChamber_InnerRoom;
extern const Map MossdeepCity_Gym;
extern const Map SecretBase_BlueCave2;
extern const Map AbandonedShip_CaptainsOffice;
extern const Map TrainerHill_4F;
extern const Map DesertUnderpass;
extern const Map LavaridgeTown_HerbShop;
extern const Map SecretBase_Shrub1;
extern const Map Underwater_SootopolisCity;
extern const Map MarineCave_End;
extern const Map FallarborTown_PokemonCenter_2F;
extern const Map BattleFrontier_BattleTowerMultiPartnerRoom;
extern const Map MossdeepCity_House2;
extern const Map Route110_TrickHousePuzzle3;
extern const Map Underwater_SealedChamber;
extern const Map FallarborTown_Mart;
extern const Map EverGrandeCity_Hall2;
extern const Map PacifidlogTown_House4;
extern const Map MossdeepCity_GameCorner_1F;
extern const Map DewfordTown_House1;
extern const Map LavaridgeTown_PokemonCenter_1F;
extern const Map DewfordTown_Hall;
extern const Map PacifidlogTown_House3;
extern const Map EverGrandeCity_Hall5;
extern const Map Route110_TrickHousePuzzle4;
extern const Map PacifidlogTown_PokemonCenter_2F;
extern const Map Route110_TrickHouseCorridor;
extern const Map BattleFrontier_BattleArenaCorridor;
extern const Map Route111_OldLadysRestStop;
extern const Map MossdeepCity_House3;
extern const Map PetalburgWoods;
extern const Map BattleFrontier_BattlePikeRoomFinal;
extern const Map SkyPillar_Outside;
extern const Map PetalburgCity_House2;
extern const Map LilycoveCity_DepartmentStore_1F;
extern const Map Route114_LanettesHouse;
extern const Map MossdeepCity_House4;
extern const Map SlateportCity_BattleTentCorridor;
extern const Map BattleFrontier_BattleDomePreBattleRoom;
extern const Map SecretBase_BlueCave3;
extern const Map FallarborTown_PokemonCenter_1F;
extern const Map SecretBase_BlueCave4;
extern const Map RustboroCity_Mart;
extern const Map NavelRock_Bottom;
extern const Map AbandonedShip_Rooms2_B1F;
extern const Map Underwater_Route127;
extern const Map LilycoveCity_ContestHall;
extern const Map DesertRuins;
extern const Map AlteringCave;
extern const Map NavelRock_Top;
extern const Map InsideOfTruck;
extern const Map BattleFrontier_BattlePikeThreePathRoom;
extern const Map MeteorFalls_B1F_2R;
extern const Map CaveOfOrigin_1F;
extern const Map RustboroCity_PokemonSchool;
extern const Map ContestHallCool;
extern const Map TrainerHill_Roof;
extern const Map BattleFrontier_Lounge2;
extern const Map Route110_TrickHouseEnd;
extern const Map SeafloorCavern_Room4;
extern const Map Underwater_Route129;
extern const Map AbandonedShip_Rooms_B1F;
extern const Map SeafloorCavern_Room3;
extern const Map MossdeepCity_SpaceCenter_1F;
extern const Map BattleFrontier_Lounge5;
extern const Map RustboroCity_House3;
extern const Map MauvilleCity_PokemonCenter_1F;
extern const Map BattleFrontier_BattlePalaceLobby;
extern const Map BattleFrontier_BattleFactoryLobby;
extern const Map AncientTomb;
extern const Map GraniteCave_StevensRoom;
extern const Map EverGrandeCity_SidneysRoom;
extern const Map NavelRock_B1F;
extern const Map BattleFrontier_BattlePalaceBattleRoom;
extern const Map BattleFrontier_BattleTowerBattleRoom;
extern const Map BattleFrontier_Lounge4;
extern const Map RustboroCity_House2;
extern const Map SeafloorCavern_Room2;
extern const Map AbandonedShip_Rooms_1F;
extern const Map FallarborTown_BattleTentBattleRoom;
extern const Map RecordCorner;
extern const Map JaggedPass;
extern const Map ShoalCave_LowTideInnerRoom;
extern const Map SeafloorCavern_Room5;
extern const Map Underwater_Route128;
extern const Map BattleFrontier_Lounge3;
extern const Map MeteorFalls_B1F_1R;
extern const Map GraniteCave_1F;
extern const Map MossdeepCity_SpaceCenter_2F;
extern const Map MtChimney_CableCarStation;
extern const Map Underwater_Route126;
extern const Map NavelRock_Up1;
extern const Map LilycoveCity_DepartmentStore_3F;
extern const Map LittlerootTown_ProfessorBirchsLab;
extern const Map MauvilleCity_PokemonCenter_2F;
extern const Map VerdanturfTown_House;
extern const Map VerdanturfTown_WandasHouse;
extern const Map LilycoveCity_PokemonTrainerFanClub;
extern const Map ScorchedSlab;
extern const Map AbandonedShip_Room_B1F;
extern const Map LavaridgeTown_House;
extern const Map MauvilleCity_House1;
extern const Map FortreeCity_House1;
extern const Map Route104_MrBrineysHouse;
extern const Map VerdanturfTown_BattleTentBattleRoom;
extern const Map CaveOfOrigin_UnusedRubySapphireMap2;
extern const Map Route119_WeatherInstitute_2F;
extern const Map SlateportCity;
extern const Map ShoalCave_LowTideEntranceRoom;
extern const Map BattleFrontier_BattlePikeLobby;
extern const Map BattlePyramidSquare12;
extern const Map SkyPillar_2F;
extern const Map UnionRoom;
extern const Map BattleFrontier_Mart;
extern const Map SouthernIsland_Interior;
extern const Map ShoalCave_LowTideIceRoom;
extern const Map MtPyre_6F;
extern const Map BattlePyramidSquare15;
extern const Map NavelRock_Harbor;
extern const Map EverGrandeCity_GlaciasRoom;
extern const Map Route110_SeasideCyclingRoadNorthEntrance;
extern const Map LilycoveCity_PokemonCenter_2F;
extern const Map MirageTower_3F;
extern const Map SecretBase_Tree2;
extern const Map BattleColosseum_2P;
extern const Map VerdanturfTown_PokemonCenter_1F;
extern const Map Route106;
extern const Map Route119_House;
extern const Map UnusedContestHall2;
extern const Map AbandonedShip_Deck;
extern const Map NavelRock_Down08;
extern const Map UnusedContestHall5;
extern const Map Route101;
extern const Map Route108;
extern const Map NavelRock_Down01;
extern const Map Route130;
extern const Map NavelRock_Down06;
extern const Map SecretBase_YellowCave2;
extern const Map FallarborTown_BattleTentLobby;
extern const Map BattlePyramidSquare14;
extern const Map LilycoveCity_DepartmentStoreElevator;
extern const Map EverGrandeCity_DrakesRoom;
extern const Map Route104_PrototypePrettyPetalFlowerShop;
extern const Map Route110_TrickHouseEntrance;
extern const Map SSTidalRooms;
extern const Map SecretBase_RedCave1;
extern const Map LilycoveCity_DepartmentStoreRooftop;
extern const Map BattlePyramidSquare13;
extern const Map Route119_WeatherInstitute_1F;
extern const Map NavelRock_Exterior;
extern const Map MtPyre_5F;
extern const Map CaveOfOrigin_UnusedRubySapphireMap3;
extern const Map SlateportCity_NameRatersHouse;
extern const Map SkyPillar_1F;
extern const Map FallarborTown_MoveRelearnersHouse;
extern const Map TerraCave_End;
extern const Map SecretBase_YellowCave3;
extern const Map SafariZone_North;
extern const Map NavelRock_Down07;
extern const Map Route131;
extern const Map Route109;
extern const Map LilycoveCity_PokemonCenter_1F;
extern const Map VerdanturfTown_PokemonCenter_2F;
extern const Map SecretBase_YellowCave4;
extern const Map EverGrandeCity_HallOfFame;
extern const Map NavelRock_Down09;
extern const Map BirthIsland_Exterior;
extern const Map SecretBase_Tree4;
extern const Map UnusedContestHall4;
extern const Map SecretBase_Tree3;
extern const Map LilycoveCity;
extern const Map UnusedContestHall3;
extern const Map Route107;
extern const Map RustboroCity_CuttersHouse;
extern const Map BattleFrontier_PokemonCenter_1F;
extern const Map Route125;
extern const Map SkyPillar_5F;
extern const Map FieryPath;
extern const Map MagmaHideout_1F;
extern const Map Route122;
extern const Map MtPyre_1F;
extern const Map BattleFrontier_BattlePalaceCorridor;
extern const Map Route114;
extern const Map SootopolisCity_MysteryEventsHouse_1F;
extern const Map SecretBase_BrownCave4;
extern const Map Route113;
extern const Map BattleFrontier_BattlePyramidTop;
extern const Map SootopolisCity_House7;
extern const Map AquaHideout_1F;
extern const Map Route116_TunnelersRestHouse;
extern const Map Route114_FossilManiacsTunnel;
extern const Map SecretBase_BrownCave3;
extern const Map LilycoveCity_House3;
extern const Map AbandonedShip_Underwater1;
extern const Map AquaHideout_UnusedRubyMap2;
extern const Map LilycoveCity_CoveLilyMotel_2F;
extern const Map OldaleTown;
extern const Map BattlePyramidSquare07;
extern const Map EverGrandeCity_PokemonLeague_1F;
extern const Map LilycoveCity_House4;
extern const Map EverGrandeCity_PokemonCenter_2F;
extern const Map RusturfTunnel;
extern const Map MirageTower_4F;
extern const Map SlateportCity_PokemonFanClub;
extern const Map BattlePyramidSquare09;
extern const Map LavaridgeTown_Mart;
extern const Map RustboroCity_Flat1_2F;
extern const Map PetalburgCity_PokemonCenter_2F;
extern const Map SootopolisCity_House6;
extern const Map Route112;
extern const Map MtPyre_2F;
extern const Map FarawayIsland_Interior;
extern const Map SecretBase_BrownCave2;
extern const Map RustboroCity_Gym;
extern const Map Route115;
extern const Map BattleFrontier_PokemonCenter_2F;
extern const Map SootopolisCity_House1;
extern const Map BattleFrontier_BattleDomeBattleRoom;
extern const Map MossdeepCity_Mart;
extern const Map Route123;
extern const Map BattleFrontier_ReceptionGate;
extern const Map MtPyre_Exterior;
extern const Map Route124;
extern const Map NavelRock_Fork;
extern const Map MagmaHideout_2F_3R;
extern const Map BattlePyramidSquare08;
extern const Map MagmaHideout_3F_3R;
extern const Map EverGrandeCity_PokemonLeague_2F;
extern const Map BattleFrontier_BattleTowerLobby;
extern const Map LilycoveCity_CoveLilyMotel_1F;
extern const Map AbandonedShip_Rooms2_1F;
extern const Map OldaleTown_Mart;
extern const Map NewMauville_Entrance;
extern const Map OldaleTown_House1;
extern const Map RustboroCity_Flat1_1F;
extern const Map BattlePyramidSquare06;
extern const Map FortreeCity_Mart;
extern const Map PetalburgCity_PokemonCenter_1F;
extern const Map EverGrandeCity_PokemonCenter_1F;
extern const Map RustboroCity_Flat2_3F;
extern const Map BattlePyramidSquare01;
extern const Map AquaHideout_UnusedRubyMap3;
extern const Map LilycoveCity_House2;
extern const Map SecretBase_BlueCave1;
extern const Map AquaHideout_B2F;
extern const Map MauvilleCity_GameCorner;
extern const Map VerdanturfTown_FriendshipRatersHouse;
extern const Map LittlerootTown_BrendansHouse_1F;
extern const Map MossdeepCity_House1;
extern const Map Route113_GlassWorkshop;
extern const Map PacifidlogTown;
extern const Map SecretBase_Shrub2;
extern const Map VictoryRoad_B1F;
extern const Map SlateportCity_Harbor;
extern const Map DewfordTown;
extern const Map MeteorFalls_1F_1R;
extern const Map BattleFrontier_BattleDomeLobby;
extern const Map Route110_TrickHousePuzzle8;
extern const Map SlateportCity_Mart;
extern const Map MossdeepCity_PokemonCenter_1F;
extern const Map Route110_TrickHousePuzzle1;
extern const Map GraniteCave_B2F;
extern const Map TrainerHill_1F;
extern const Map DewfordTown_PokemonCenter_1F;
extern const Map LittlerootTown_MaysHouse_1F;
extern const Map PacifidlogTown_House1;
extern const Map Route110_TrickHousePuzzle6;
extern const Map ShoalCave_LowTideLowerRoom;
extern const Map RustboroCity_DevonCorp_1F;
extern const Map SecretBase_Shrub3;
extern const Map FarawayIsland_Entrance;
extern const Map PetalburgCity_House1;
extern const Map BattleFrontier_BattleFactoryBattleRoom;
extern const Map TerraCave_Entrance;
extern const Map MeteorFalls_StevensCave;
extern const Map Route104_Prototype;
extern const Map AquaHideout_B1F;
extern const Map SecretBase_Shrub4;
extern const Map AbandonedShip_HiddenFloorRooms;
extern const Map MtChimney;
extern const Map VictoryRoad_1F;
extern const Map VictoryRoad_B2F;
extern const Map MeteorFalls_1F_2R;
extern const Map MauvilleCity;
extern const Map LittlerootTown_BrendansHouse_2F;
extern const Map BattleFrontier_BattlePyramidLobby;
extern const Map BattleFrontier_BattleTowerElevator;
extern const Map ContestHallBeauty;
extern const Map ShoalCave_LowTideStairsRoom;
extern const Map Route110_TrickHousePuzzle7;
extern const Map AbandonedShip_HiddenFloorCorridors;
extern const Map SootopolisCity_LotadAndSeedotHouse;
extern const Map MossdeepCity_PokemonCenter_2F;
extern const Map EverGrandeCity_Hall1;
extern const Map DewfordTown_House2;
extern const Map Underwater_Route134;
extern const Map LilycoveCity_DepartmentStore_4F;
extern const Map RustboroCity_DevonCorp_2F;
extern const Map GraniteCave_B1F;
extern const Map LittlerootTown_MaysHouse_2F;
extern const Map DewfordTown_PokemonCenter_2F;
extern const Map TrainerHill_2F;
extern const Map Underwater_Route105;
extern const Map ArtisanCave_B1F;
