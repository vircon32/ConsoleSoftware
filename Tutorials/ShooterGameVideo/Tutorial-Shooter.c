// include Vircon libraries
#include "audio.h"
#include "video.h"
#include "input.h"
#include "time.h"
#include "math.h"
#include "misc.h"


// -----------------------------------------------
// DEFINITIONS FOR TEXTURES
// -----------------------------------------------


// give names to each texture in our cartridge
#define TextureBackground   0
#define TextureGameObjects  1

// names for our texture regions
// within the background texture
#define RegionBackground  0

// names for our texture regions
// within the game objects texture
#define RegionPlayerShip      0
#define RegionPlayerShot      1
#define RegionEnemyBee        2
#define FirstRegionExplosion  10


// -----------------------------------------------
// DEFINITIONS FOR SOUNDS
// -----------------------------------------------


// give names to each sound in our cartridge
#define MusicGameplay   0
#define SoundShoot      1
#define SoundExplosion  2
#define SoundDeath      3

// we will reserve the last sound channel
// exclusively to play the music
#define ChannelMusic  15


// -----------------------------------------------
// DEFINITIONS FOR GAMEPLAY
// -----------------------------------------------


// movement speeds of the different objects
#define PlayerSpeed  4
#define EnemySpeed   3
#define ShotSpeed    15

// maximum simultaneous enemies
#define MaximumEnemies  5

// basic state for each game object
struct GameObject
{
    bool Active;
    int X, Y;           // position of hitbox center on screen
    int Width, Height;  // dimensions of hitbox, in pixels
};


// -----------------------------------------------
// AUXILIARY FUNCTIONS
// -----------------------------------------------


// returns true if the 2 objects hitboxes overlap
bool ObjectsOverlap( GameObject* Object1, GameObject* Object2 )
{
    // detect non-overlap along X axis
    if( (Object1->X - Object1->Width/2) >= (Object2->X + Object2->Width/2) )
      return false;
    
    if( (Object1->X + Object1->Width/2) <= (Object2->X - Object2->Width/2) )
      return false;
    
    // detect non-overlap along Y axis
    if( (Object1->Y - Object1->Height/2) >= (Object2->Y + Object2->Height/2) )
      return false;
    
    if( (Object1->Y + Object1->Height/2) <= (Object2->Y - Object2->Height/2) )
      return false;
    
    // there was overlap on both axes
    return true;
}


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    // first we will define regions on
    // our background texture
    select_texture( TextureBackground );
    
    // define our fixed background region to 
    // be drawn always from top-left at (0,0)
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    // now define regions for our other texture
    select_texture( TextureGameObjects );
    
    // define our single regions in this texture
    select_region( RegionPlayerShip );
    define_region( 81,79,  149,130,  114,108 );
    
    select_region( RegionPlayerShot );
    define_region( 81,135,  111,147,  96,141 );
    
    select_region( RegionEnemyBee );
    define_region( 3,79,  76,152,  40,119 );
    
    // explosions are a 6-frame animation that
    // we will define as a 6x1 matrix of regions
    // within the texture, with consecutive IDs
    // and with hotspot at their center
    define_region_matrix( FirstRegionExplosion,  3,3,  74,74,  38,38,  6,1,  1 );
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    
    // configure music to loop
    select_sound( MusicGameplay );
    set_sound_loop( true );
    
    // lower the volume on all sound channels
    // (if more than 1 sound is playing at the
    // same time it can be too loud and make
    // the audio output saturate)
    for( int i = 0; i < sound_channels; i++ )
    {
        select_channel( i );
        set_channel_volume( 0.3 );
    }
    
    // -----------------------------------------------
    // PART 3: GAME INITIALIZATIONS
    
    // we will control the ship with gamepad 1
    select_gamepad( 0 );
    
    // make random numbers different each time
    srand( get_time() );
    
    // set the initial state of our ship
    GameObject Player;
    Player.Active = true;
    Player.Width = 55;
    Player.Height = 37;
    Player.X = screen_width / 6;
    Player.Y = screen_height / 2;
    
    // a single player shot, initially inactive
    // (can only shoot 1 at a time)
    GameObject Shot;
    Shot.Active = false;
    Shot.Width = 27;
    Shot.Height = 13;
    
    // a maximum of 10 active enemies,
    // all inactive until initialized
    // (Note the non-standard array declaration)
    GameObject[ MaximumEnemies ] Enemies;
    
    for( int i = 0; i < MaximumEnemies; i++ )
    {
        Enemies[ i ].Active = true;
        Enemies[ i ].Width = 57;
        Enemies[ i ].Height = 43;
        
        // set a random position avoiding left half
        // of the screen (to prevent instant death)
        Enemies[ i ].X = screen_width/2 + rand() % screen_width;
        Enemies[ i ].Y = 30 + rand() % (screen_height - 60);
    }
    
    // a single explosion (only 1 at a time);
    // it will need an extra counter to keep
    // track of its animation frames
    GameObject Explosion;
    Explosion.Active = false;
    int ExplosionFrames = 0;
    
    // begin playing music
    play_sound_in_channel( MusicGameplay, ChannelMusic );
    
    // -----------------------------------------------
    // PART 4: MAIN LOOP
    
    // our game never ends: keep repeating
    // our game logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 1: Read player inputs
        
        // read the direction pressed on the D-Pad
        int DirectionX, DirectionY;
        gamepad_direction( &DirectionX, &DirectionY );        
        
        // check if button A (shot) was just pressed
        bool ShotPressed = (gamepad_button_a() == 1);
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 2: Apply game logic for this frame
        
        // apply player controls when alive
        if( Player.Active )
        {
            // move the ship in the direction pressed
            Player.X += PlayerSpeed * DirectionX;
            Player.Y += PlayerSpeed * DirectionY;
            
            // create a new shot if player pressed the
            // shot button, and there was not already one
            if( ShotPressed && !Shot.Active )
            {
                Shot.Active = true;
                Shot.X = Player.X + Player.Width/2;
                Shot.Y = Player.Y;
                
                // play the shoot sound
                play_sound( SoundShoot );
            }
        }
        
        // prevent player from leaving the screen
        Player.X = max( Player.X, Player.Width/2 );
        Player.Y = max( Player.Y, Player.Height/2 );
        
        Player.X = min( Player.X, screen_width - Player.Width/2 );
        Player.Y = min( Player.Y, screen_height - Player.Height/2 );
        
        // move active enemies to the left
        for( int i = 0; i < MaximumEnemies; i++ )
        {
            Enemies[ i ].X -= EnemySpeed;
            
            // remove enemies that leave the screen
            if( Enemies[ i ].X < -Enemies[ i ].Width/2 )
            {
                // re-spawn that same enemy off-screen
                // from the right, at a random height
                Enemies[ i ].X = screen_width + 60;
                Enemies[ i ].Y = 30 + rand() % (screen_height - 60);
            }
        }
        
        // move shot, if active, to the right
        if( Shot.Active )
        {
            Shot.X += ShotSpeed;
            
            // remove the shot when leaving the screen
            if( Shot.X > (screen_width + Shot.Width/2) )
              Shot.Active = false;
        }
        
        // check enemy collisions
        for( int i = 0; i < MaximumEnemies; i++ )
        {
            // detect collisions with the player
            if( Player.Active && ObjectsOverlap( &Player, &Enemies[ i ] ) )
            {
                // kill player and show an explosion
                Player.Active = false;
                Explosion.Active = true;
                Explosion.X = Player.X;
                Explosion.Y = Player.Y;
                ExplosionFrames = 1;
                
                // play the death sound
                play_sound( SoundDeath );
            }
            
            // detect collisions with the shot
            if( Shot.Active && ObjectsOverlap( &Shot, &Enemies[ i ] ) )
            {
                // kill shot, and show an explosion
                Shot.Active = false;
                Explosion.Active = true;
                Explosion.X = Enemies[ i ].X;
                Explosion.Y = Enemies[ i ].Y;
                ExplosionFrames = 1;
                
                // play the explosion sound
                play_sound( SoundExplosion );
                
                // re-spawn that same enemy off-screen
                // from the right, at a random height
                Enemies[ i ].X = screen_width + 60;
                Enemies[ i ].Y = 30 + rand() % (screen_height - 60);
            }
        }
        
        // process explosion lifecycle
        if( Explosion.Active )
        {
            ExplosionFrames++;
            
            if( ExplosionFrames >= (6*3) )
              Explosion.Active = false;
        }
        
        // if player's explosion ended, the game ends
        // (no restart, we simply stop the program)
        if( !Player.Active && !Explosion.Active )
        {
            // stop music and darken the screen
            // to let player know the game has
            // not just frozen, but he died
            stop_channel( ChannelMusic );
            clear_screen( make_color_rgba(0,0,0,128) );
            return;
        }
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 3: Draw the scene on screen
        
        // draw our fixed background
        select_texture( TextureBackground );
        select_region( RegionBackground );
        draw_region_at( 0, 0 );
        
        // draw all active game objects
        select_texture( TextureGameObjects );
        
        if( Player.Active )
        {
            select_region( RegionPlayerShip );
            draw_region_at( Player.X, Player.Y );
        }
        
        for( int i = 0; i < MaximumEnemies; i++ )
        {
            if( !Enemies[ i ].Active )
              continue;
            
            select_region( RegionEnemyBee );
            draw_region_at( Enemies[ i ].X, Enemies[ i ].Y );
        }
        
        if( Shot.Active )
        {
            select_region( RegionPlayerShot );
            draw_region_at( Shot.X, Shot.Y );
        }
        
        if( Explosion.Active )
        {
            int AnimationRegion = FirstRegionExplosion + ExplosionFrames/3;
            select_region( AnimationRegion );
            draw_region_at( Explosion.X, Explosion.Y );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 4: Wait for next frame
        end_frame();
    }
}
