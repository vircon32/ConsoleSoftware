// *****************************************************************************
    // start include guard
    #ifndef PHYSICS_H
    #define PHYSICS_H
    
    // forward struct declarations
    struct Player;
    struct LevelMap;
// *****************************************************************************


// ---------------------------------------------------------
//   TILE BASED ROOM-PLAYER COLLISION DETECTION
// ---------------------------------------------------------


// global level boundary check
void EnforceLevelBoundaries( Player* P, LevelMap* L );

// detecting of exposed tile edges
// (prevents colliding against hidden edges)
bool TileLeftIsExposed( LevelMap* L, int TileX, int TileY );
bool TileRightIsExposed( LevelMap* L, int TileX, int TileY );
bool TileTopIsExposed( LevelMap* L, int TileX, int TileY );
bool TileBottomIsExposed( LevelMap* L, int TileX, int TileY );

// partial player-tile collision functions
void CollidePlayerWithTileLeft( Player* P, int TileX, int TileY );
void CollidePlayerWithTileRight( Player* P, int TileX, int TileY );
void CollidePlayerWithTileTop( Player* P, int TileX, int TileY );
void CollidePlayerWithTileBottom( Player* P, int TileX, int TileY );

// full player-tile collision functions
void CollidePlayerWithTile( Player* P, LevelMap* L, int TileX, int TileY );
void CollidePlayerWithLevel( Player* P, LevelMap* L );

// collisions between tilemap and other objects (detection only)
bool point_overlaps_tilemap( Vector2D* point, tilemap* tm );
bool box_overlaps_tilemap( Box* B, tilemap* tm );
int* get_tile_at_point( Vector2D* point, tilemap* tm );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
