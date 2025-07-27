#include <stdbool.h>
#include <types.h>
#include "tilesets.h"

Tileset gTileset_General =
{
    .isCompressed = true,
    .isSecondary = false,
    .tiles = 14503952,
    .dataSize = 8912,
    .palettes_offset = 3550864,
    .metatiles_offset = 3760368,
    .metatileAttributes = 3768560,
    //.callback = InitTilesetAnim_General,
};

Tileset gTileset_Petalburg =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3382792,
    .dataSize = 2300,
    .palettes_offset = 3385092,
    .metatiles_offset = 3769584,
    .metatileAttributes = 3771888,
    //.callback = InitTilesetAnim_Petalburg,
};

Tileset gTileset_Rustboro =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3385604,
    .dataSize = 5564,
    .palettes_offset = 3391168,
    .metatiles_offset = 3772176,
    .metatileAttributes = 3777776,
    //.callback = InitTilesetAnim_Rustboro,
};

Tileset gTileset_Dewford =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3391680,
    .dataSize = 5248,
    .palettes_offset = 3385092,
    .metatiles_offset = 3778476,
    .metatileAttributes = 3784540,
    //.callback = InitTilesetAnim_Dewford,
};

Tileset gTileset_Slateport =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3397440,
    .dataSize = 8136,
    .palettes_offset = 3385092,
    .metatiles_offset = 3785298,
    .metatileAttributes = 3791794,
    //.callback = InitTilesetAnim_Slateport,
};

Tileset gTileset_Mauville =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3406088,
    .dataSize = 7716,
    .palettes_offset = 3413804,
    .metatiles_offset = 3792606,
    .metatileAttributes = 3800766,
    //.callback = InitTilesetAnim_Mauville,
};

Tileset gTileset_Lavaridge =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3414316,
    .dataSize = 5372,
    .palettes_offset = 3419688,
    .metatiles_offset = 3801786,
    .metatileAttributes = 3808842,
    //.callback = InitTilesetAnim_Lavaridge,
};

Tileset gTileset_Fallarbor =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3420200,
    .dataSize = 6008,
    .palettes_offset = 3426208,
    .metatiles_offset = 3809724,
    .metatileAttributes = 3815596,
    //.callback = InitTilesetAnim_Fallarbor,
};

Tileset gTileset_Fortree =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3426720,
    .dataSize = 5760,
    .palettes_offset = 3432480,
    .metatiles_offset = 3816330,
    .metatileAttributes = 3820810,
    //.callback = InitTilesetAnim_Fortree,
};

Tileset gTileset_Lilycove =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3432992,
    .dataSize = 5024,
    .palettes_offset = 3385092,
    .metatiles_offset = 3821370,
    .metatileAttributes = 3826986,
    //.callback = InitTilesetAnim_Lilycove,
};

Tileset gTileset_Mossdeep =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3438528,
    .dataSize = 5712,
    .palettes_offset = 3385092,
    .metatiles_offset = 3827688,
    .metatileAttributes = 3834952,
    //.callback = InitTilesetAnim_Mossdeep,
};

Tileset gTileset_EverGrande =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3444752,
    .dataSize = 3096,
    .palettes_offset = 3385092,
    .metatiles_offset = 3835860,
    .metatileAttributes = 3838548,
    //.callback = InitTilesetAnim_EverGrande,
};

Tileset gTileset_Pacifidlog =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3448360,
    .dataSize = 6028,
    .palettes_offset = 3385092,
    .metatiles_offset = 3838884,
    .metatileAttributes = 3842132,
    //.callback = InitTilesetAnim_Pacifidlog,
};

Tileset gTileset_Sootopolis =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3454900,
    .dataSize = 3840,
    .palettes_offset = 3385092,
    .metatiles_offset = 3842538,
    .metatileAttributes = 3846602,
    //.callback = InitTilesetAnim_Sootopolis,
};

Tileset gTileset_BattleFrontierOutsideWest =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3459252,
    .dataSize = 6960,
    .palettes_offset = 3385092,
    .metatiles_offset = 3847110,
    .metatileAttributes = 3855270,
    //.callback = InitTilesetAnim_BattleFrontierOutsideWest,
};

Tileset gTileset_BattleFrontierOutsideEast =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3466724,
    .dataSize = 5024,
    .palettes_offset = 3385092,
    .metatiles_offset = 3856290,
    .metatileAttributes = 3864450,
    //.callback = InitTilesetAnim_BattleFrontierOutsideEast,
};

Tileset gTileset_Building =
{
    .isCompressed = true,
    .isSecondary = false,
    .tiles = 3472260,
    .dataSize = 3384,
    .palettes_offset = 3475644,
    .metatiles_offset = 3865470,
    .metatileAttributes = 3865598,
    //.callback = InitTilesetAnim_Building,
};

Tileset gTileset_Shop =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3476156,
    .dataSize = 5664,
    .palettes_offset = 3481820,
    .metatiles_offset = 3865614,
    .metatileAttributes = 3870238,
    //.callback = NULL,
};

Tileset gTileset_PokemonCenter =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3482332,
    .dataSize = 4760,
    .palettes_offset = 3487092,
    .metatiles_offset = 3870816,
    .metatileAttributes = 3874528,
    //.callback = NULL,
};

Tileset gTileset_Cave =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3487604,
    .dataSize = 6892,
    .palettes_offset = 3385572,
    .metatiles_offset = 3874992,
    .metatileAttributes = 3881616,
    //.callback = InitTilesetAnim_Cave,
};

Tileset gTileset_PokemonSchool =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3495008,
    .dataSize = 1864,
    .palettes_offset = 3496872,
    .metatiles_offset = 3882444,
    .metatileAttributes = 3883372,
    //.callback = NULL,
};

Tileset gTileset_PokemonFanClub =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3497384,
    .dataSize = 2348,
    .palettes_offset = 3499732,
    .metatiles_offset = 3883488,
    .metatileAttributes = 3885152,
    //.callback = NULL,
};

Tileset gTileset_Unused1 =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3500244,
    .dataSize = 76,
    .palettes_offset = 3500320,
    .metatiles_offset = 3885360,
    .metatileAttributes = 179,
    //.callback = NULL,
};

Tileset gTileset_MeteorFalls =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3500832,
    .dataSize = 7456,
    .palettes_offset = 3385572,
    .metatiles_offset = 3885396,
    .metatileAttributes = 3887940,
    //.callback = NULL,
};

Tileset gTileset_OceanicMuseum =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3508800,
    .dataSize = 3980,
    .palettes_offset = 3385572,
    .metatiles_offset = 3888258,
    .metatileAttributes = 3890210,
    //.callback = NULL,
};

Tileset gTileset_CableClub =
{
    .isCompressed = false,
    .isSecondary = true,
    .tiles = 3513292,
    .dataSize = 3840,
    .palettes_offset = 3533516,
    .metatiles_offset = 3890454,
    .metatileAttributes = 3894550,
    //.callback = NULL,
};

Tileset gTileset_SeashoreHouse =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3534028,
    .dataSize = 2268,
    .palettes_offset = 3481820,
    .metatiles_offset = 3895062,
    .metatileAttributes = 3895958,
    //.callback = NULL,
};

Tileset gTileset_PrettyPetalFlowerShop =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3536808,
    .dataSize = 3064,
    .palettes_offset = 3481820,
    .metatiles_offset = 3896070,
    .metatileAttributes = 3897222,
    //.callback = NULL,
};

Tileset gTileset_PokemonDayCare =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3540384,
    .dataSize = 2620,
    .palettes_offset = 3481820,
    .metatiles_offset = 3897366,
    .metatileAttributes = 3898454,
    //.callback = NULL,
};

Tileset gTileset_Facility =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3543516,
    .dataSize = 7348,
    .palettes_offset = 3550864,
    .metatiles_offset = 3898590,
    .metatileAttributes = 3906766,
    //.callback = NULL,
};

Tileset gTileset_BikeShop =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3551376,
    .dataSize = 4912,
    .palettes_offset = 3556288,
    .metatiles_offset = 3907788,
    .metatileAttributes = 3911756,
    //.callback = InitTilesetAnim_BikeShop,
};

Tileset gTileset_RusturfTunnel =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3556800,
    .dataSize = 2632,
    .palettes_offset = 3385572,
    .metatiles_offset = 3912252,
    .metatileAttributes = 3913580,
    //.callback = NULL,
};

Tileset gTileset_SecretBaseBrownCave =
{
    .isCompressed = false,
    .isSecondary = true,
    .tiles = 3570592,
    .dataSize = 2624,
    .palettes_offset = 3561352,
    .metatiles_offset = 3913746,
    .metatileAttributes = 3918930,
    //.callback = NULL,
};

Tileset gTileset_SecretBaseTree =
{
    .isCompressed = false,
    .isSecondary = true,
    .tiles = 3573248,
    .dataSize = 2624,
    .palettes_offset = 3563180,
    .metatiles_offset = 3913746,
    .metatileAttributes = 3918930,
    //.callback = NULL,
};

Tileset gTileset_SecretBaseShrub =
{
    .isCompressed = false,
    .isSecondary = true,
    .tiles = 3575904,
    .dataSize = 2624,
    .palettes_offset = 3563180,
    .metatiles_offset = 3913746,
    .metatileAttributes = 3918930,
    //.callback = NULL,
};

Tileset gTileset_SecretBaseBlueCave =
{
    .isCompressed = false,
    .isSecondary = true,
    .tiles = 3578560,
    .dataSize = 2624,
    .palettes_offset = 3563180,
    .metatiles_offset = 3913746,
    .metatileAttributes = 3918930,
    //.callback = NULL,
};

Tileset gTileset_SecretBaseYellowCave =
{
    .isCompressed = false,
    .isSecondary = true,
    .tiles = 3581216,
    .dataSize = 2624,
    .palettes_offset = 3561352,
    .metatiles_offset = 3913746,
    .metatileAttributes = 3918930,
    //.callback = NULL,
};

Tileset gTileset_SecretBaseRedCave =
{
    .isCompressed = false,
    .isSecondary = true,
    .tiles = 3583872,
    .dataSize = 2624,
    .palettes_offset = 3563180,
    .metatiles_offset = 3913746,
    .metatileAttributes = 3918930,
    //.callback = NULL,
};

Tileset gTileset_InsideOfTruck =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3586528,
    .dataSize = 604,
    .palettes_offset = 3587132,
    .metatiles_offset = 3919578,
    .metatileAttributes = 3920186,
    //.callback = NULL,
};

Tileset gTileset_Unused2 =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3622024,
    .dataSize = 1320,
    .palettes_offset = 3391168,
    .metatiles_offset = 3948594,
    .metatileAttributes = 3949522,
    //.callback = NULL,
};

Tileset gTileset_Contest =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3587644,
    .dataSize = 2900,
    .palettes_offset = 3481820,
    .metatiles_offset = 3920262,
    .metatileAttributes = 3923990,
    //.callback = NULL,
};

Tileset gTileset_LilycoveMuseum =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3591056,
    .dataSize = 4892,
    .palettes_offset = 3385572,
    .metatiles_offset = 3924456,
    .metatileAttributes = 3926744,
    //.callback = NULL,
};

Tileset gTileset_BrendansMaysHouse =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3596460,
    .dataSize = 4300,
    .palettes_offset = 3600760,
    .metatiles_offset = 3927030,
    .metatileAttributes = 3930166,
    //.callback = NULL,
};

Tileset gTileset_Lab =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3601272,
    .dataSize = 4196,
    .palettes_offset = 3605468,
    .metatiles_offset = 3930558,
    .metatileAttributes = 3933086,
    //.callback = NULL,
};

Tileset gTileset_Underwater =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3605980,
    .dataSize = 4540,
    .palettes_offset = 3385092,
    .metatiles_offset = 3933402,
    .metatileAttributes = 3937178,
    //.callback = InitTilesetAnim_Underwater,
};

Tileset gTileset_PetalburgGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3630252,
    .dataSize = 1700,
    .palettes_offset = 3631952,
    .metatiles_offset = 3955092,
    .metatileAttributes = 3958676,
    //.callback = NULL,
};

Tileset gTileset_SootopolisGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3635504,
    .dataSize = 4500,
    .palettes_offset = 3624756,
    .metatiles_offset = 3962670,
    .metatileAttributes = 3964622,
    //.callback = InitTilesetAnim_SootopolisGym,
};

Tileset gTileset_GenericBuilding =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3611032,
    .dataSize = 6700,
    .palettes_offset = 3617732,
    .metatiles_offset = 3937650,
    .metatileAttributes = 3945842,
    //.callback = NULL,
};

Tileset gTileset_MauvilleGameCorner =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3618244,
    .dataSize = 3268,
    .palettes_offset = 3621512,
    .metatiles_offset = 3946866,
    .metatileAttributes = 3948402,
    //.callback = NULL,
};

Tileset gTileset_RustboroGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3623856,
    .dataSize = 900,
    .palettes_offset = 3624756,
    .metatiles_offset = 3949638,
    .metatileAttributes = 3950534,
    //.callback = NULL,
};

Tileset gTileset_DewfordGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3625268,
    .dataSize = 716,
    .palettes_offset = 3624756,
    .metatiles_offset = 3950646,
    .metatileAttributes = 3951750,
    //.callback = NULL,
};

Tileset gTileset_MauvilleGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3626496,
    .dataSize = 1924,
    .palettes_offset = 3624756,
    .metatiles_offset = 3951888,
    .metatileAttributes = 3953760,
    //.callback = InitTilesetAnim_MauvilleGym,
};

Tileset gTileset_LavaridgeGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3628932,
    .dataSize = 808,
    .palettes_offset = 3629740,
    .metatiles_offset = 3953994,
    .metatileAttributes = 3954970,
    //.callback = NULL,
};

Tileset gTileset_TrickHousePuzzle =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3640516,
    .dataSize = 3508,
    .palettes_offset = 3624756,
    .metatiles_offset = 3964866,
    .metatileAttributes = 3967890,
    //.callback = NULL,
};

Tileset gTileset_FortreeGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3632464,
    .dataSize = 884,
    .palettes_offset = 3624756,
    .metatiles_offset = 3959124,
    .metatileAttributes = 3960404,
    //.callback = NULL,
};

Tileset gTileset_MossdeepGym =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3633860,
    .dataSize = 1132,
    .palettes_offset = 3624756,
    .metatiles_offset = 3960564,
    .metatileAttributes = 3962436,
    //.callback = NULL,
};

Tileset gTileset_InsideShip =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3644536,
    .dataSize = 3024,
    .palettes_offset = 3647560,
    .metatiles_offset = 3968268,
    .metatileAttributes = 3972300,
    //.callback = NULL,
};

Tileset gTileset_SecretBase =
{
    .isCompressed = false,
    .isSecondary = false,
    .tiles = 3648072,
    .dataSize = 16384,
    .palettes_offset = 3563180,
    .metatiles_offset = 3972804,
    .metatileAttributes = 179,
    //.callback = NULL,
};

Tileset *const gTilesetPointer_SecretBase = &gTileset_SecretBase;
Tileset *const gTilesetPointer_SecretBaseRedCave = &gTileset_SecretBaseRedCave;

Tileset gTileset_EliteFour =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3681352,
    .dataSize = 4220,
    .palettes_offset = 3624756,
    .metatiles_offset = 3972840,
    .metatileAttributes = 3978152,
    //.callback = InitTilesetAnim_EliteFour,
};

Tileset gTileset_BattleFrontier =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3686084,
    .dataSize = 3332,
    .palettes_offset = 3689416,
    .metatiles_offset = 3978816,
    .metatileAttributes = 3986960,
    //.callback = NULL,
};

Tileset gTileset_BattlePalace =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3689928,
    .dataSize = 1912,
    .palettes_offset = 3550864,
    .metatiles_offset = 3987978,
    .metatileAttributes = 3990762,
    //.callback = NULL,
};

Tileset gTileset_BattleDome =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3692352,
    .dataSize = 4688,
    .palettes_offset = 3697040,
    .metatiles_offset = 3991110,
    .metatileAttributes = 3997942,
    //.callback = InitTilesetAnim_BattleDome,
};

Tileset gTileset_BattleFactory =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3697552,
    .dataSize = 4588,
    .palettes_offset = 3702140,
    .metatiles_offset = 3998796,
    .metatileAttributes = 4004460,
    //.callback = NULL,
};

Tileset gTileset_BattlePike =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3702652,
    .dataSize = 4820,
    .palettes_offset = 3707472,
    .metatiles_offset = 4005168,
    .metatileAttributes = 4010720,
    //.callback = NULL,
};

Tileset gTileset_BattleArena =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3707984,
    .dataSize = 2876,
    .palettes_offset = 3710860,
    .metatiles_offset = 4011414,
    .metatileAttributes = 4014246,
    //.callback = NULL,
};

Tileset gTileset_BattlePyramid =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3711372,
    .dataSize = 4624,
    .palettes_offset = 3715996,
    .metatiles_offset = 4014600,
    .metatileAttributes = 4018136,
    //.callback = InitTilesetAnim_BattlePyramid,
};

Tileset gTileset_MirageTower =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3716508,
    .dataSize = 6728,
    .palettes_offset = 3385572,
    .metatiles_offset = 4018578,
    .metatileAttributes = 3881616,
    //.callback = NULL,
};

Tileset gTileset_MossdeepGameCorner =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3723748,
    .dataSize = 1056,
    .palettes_offset = 3624756,
    .metatiles_offset = 4026030,
    .metatileAttributes = 4026878,
    //.callback = NULL,
};

Tileset gTileset_IslandHarbor =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3725316,
    .dataSize = 6608,
    .palettes_offset = 3550864,
    .metatiles_offset = 4026984,
    .metatileAttributes = 4034504,
    //.callback = NULL,
};

Tileset gTileset_TrainerHill =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3732436,
    .dataSize = 4336,
    .palettes_offset = 3624756,
    .metatiles_offset = 4035444,
    .metatileAttributes = 4043220,
    //.callback = NULL,
};

Tileset gTileset_NavelRock =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3737284,
    .dataSize = 7060,
    .palettes_offset = 3385572,
    .metatiles_offset = 4044192,
    .metatileAttributes = 4051408,
    //.callback = NULL,
};

Tileset gTileset_BattleFrontierRankingHall =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3744856,
    .dataSize = 2132,
    .palettes_offset = 3746988,
    .metatiles_offset = 4052310,
    .metatileAttributes = 4053814,
    //.callback = NULL,
};

Tileset gTileset_BattleTent =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3747500,
    .dataSize = 2660,
    .palettes_offset = 3475644,
    .metatiles_offset = 4054002,
    .metatileAttributes = 4058306,
    //.callback = NULL,
};

Tileset gTileset_MysteryEventsHouse =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3750672,
    .dataSize = 6504,
    .palettes_offset = 3617732,
    .metatiles_offset = 4058844,
    .metatileAttributes = 4059916,
    //.callback = NULL,
};

Tileset gTileset_UnionRoom =
{
    .isCompressed = true,
    .isSecondary = true,
    .tiles = 3758200,
    .dataSize = 2168,
    .palettes_offset = 3757688,
    .metatiles_offset = 4060050,
    .metatileAttributes = 4060834,
    //.callback = NULL,
};
