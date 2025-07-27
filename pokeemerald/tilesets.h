#pragma once
#include <stdbool.h>
#include <types.h>

//typedef void (*TilesetCB)(void);

typedef struct Tileset
{
    bool isCompressed;
    bool isSecondary;
    const u32 tiles;
    const u32 dataSize;
    const u32 palettes_offset; //the offset to the first of 16 palettes. Each palette is 32 bytes, so second palette is palettes+32 and so on
    u16 *palettes;
    const u32 metatiles_offset;
    u16 *metatiles;
    const u32 metatileAttributes;
//  TilesetCB callback;
} Tileset;

extern Tileset gTileset_General;
extern Tileset gTileset_Petalburg;
extern Tileset gTileset_Rustboro;
extern Tileset gTileset_Dewford;
extern Tileset gTileset_Slateport;
extern Tileset gTileset_Mauville;
extern Tileset gTileset_Lavaridge;
extern Tileset gTileset_Fallarbor;
extern Tileset gTileset_Fortree;
extern Tileset gTileset_Lilycove;
extern Tileset gTileset_Mossdeep;
extern Tileset gTileset_EverGrande;
extern Tileset gTileset_Pacifidlog;
extern Tileset gTileset_Sootopolis;
extern Tileset gTileset_BattleFrontierOutsideWest;
extern Tileset gTileset_BattleFrontierOutsideEast;
extern Tileset gTileset_Building;
extern Tileset gTileset_Shop;
extern Tileset gTileset_PokemonCenter;
extern Tileset gTileset_Cave;
extern Tileset gTileset_PokemonSchool;
extern Tileset gTileset_PokemonFanClub;
extern Tileset gTileset_Unused1;
extern Tileset gTileset_MeteorFalls;
extern Tileset gTileset_OceanicMuseum;
extern Tileset gTileset_CableClub;
extern Tileset gTileset_SeashoreHouse;
extern Tileset gTileset_PrettyPetalFlowerShop;
extern Tileset gTileset_PokemonDayCare;
extern Tileset gTileset_Facility;
extern Tileset gTileset_BikeShop;
extern Tileset gTileset_RusturfTunnel;
extern Tileset gTileset_SecretBaseBrownCave;
extern Tileset gTileset_SecretBaseTree;
extern Tileset gTileset_SecretBaseShrub;
extern Tileset gTileset_SecretBaseBlueCave;
extern Tileset gTileset_SecretBaseYellowCave;
extern Tileset gTileset_SecretBaseRedCave;
extern Tileset gTileset_InsideOfTruck;
extern Tileset gTileset_Unused2;
extern Tileset gTileset_Contest;
extern Tileset gTileset_LilycoveMuseum;
extern Tileset gTileset_BrendansMaysHouse;
extern Tileset gTileset_Lab;
extern Tileset gTileset_Underwater;
extern Tileset gTileset_PetalburgGym;
extern Tileset gTileset_SootopolisGym;
extern Tileset gTileset_GenericBuilding;
extern Tileset gTileset_MauvilleGameCorner;
extern Tileset gTileset_RustboroGym;
extern Tileset gTileset_DewfordGym;
extern Tileset gTileset_MauvilleGym;
extern Tileset gTileset_LavaridgeGym;
extern Tileset gTileset_TrickHousePuzzle;
extern Tileset gTileset_FortreeGym;
extern Tileset gTileset_MossdeepGym;
extern Tileset gTileset_InsideShip;
extern Tileset gTileset_SecretBase;
extern Tileset gTileset_EliteFour;
extern Tileset gTileset_BattleFrontier;
extern Tileset gTileset_BattlePalace;
extern Tileset gTileset_BattleDome;
extern Tileset gTileset_BattleFactory;
extern Tileset gTileset_BattlePike;
extern Tileset gTileset_BattleArena;
extern Tileset gTileset_BattlePyramid;
extern Tileset gTileset_MirageTower;
extern Tileset gTileset_MossdeepGameCorner;
extern Tileset gTileset_IslandHarbor;
extern Tileset gTileset_TrainerHill;
extern Tileset gTileset_NavelRock;
extern Tileset gTileset_BattleFrontierRankingHall;
extern Tileset gTileset_BattleTent;
extern Tileset gTileset_MysteryEventsHouse;
extern Tileset gTileset_UnionRoom;