// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREROAD
#define TEXTUREREGIONS_TEXTUREROAD

// define names for single regions
#define RegionClouds  0
#define RegionRoad  1
#define RegionBackground  2
#define RegionCarBehind  3
#define RegionCarSide  4
#define RegionCarSemiside  5
#define RegionRoadObject1  6
#define RegionRoadObject2  7
#define RegionRoadObject3  8
#define RegionRoadObject4  9
#define RegionMountainSmall  10
#define RegionMountain  11

// your initialization code must call this function
void DefineRegions_TextureRoad() 
{
    select_texture( TextureRoad );

    // define single regions
    select_region( RegionClouds );
    define_region( 1,1,  640,63,  1,63 );
    select_region( RegionRoad );
    define_region( 1,67,  512,578,  193,67 );
    select_region( RegionBackground );
    define_region( 516,67,  516,426,  516,67 );
    select_region( RegionCarBehind );
    define_region( 520,67,  633,174,  583,170 );
    select_region( RegionCarSide );
    define_region( 520,178,  653,282,  593,276 );
    select_region( RegionCarSemiside );
    define_region( 520,286,  637,394,  592,391 );
    select_region( RegionRoadObject1 );
    define_region( 520,398,  621,438,  570,411 );
    select_region( RegionRoadObject2 );
    define_region( 516,442,  602,492,  520,465 );
    select_region( RegionRoadObject3 );
    define_region( 516,496,  593,531,  554,512 );
    select_region( RegionRoadObject4 );
    define_region( 516,535,  590,548,  555,536 );
    select_region( RegionMountainSmall );
    define_region( 637,117,  728,172,  684,157 );
    select_region( RegionMountain );
    define_region( 644,1,  825,113,  738,88 );
}

// end include guard
#endif
