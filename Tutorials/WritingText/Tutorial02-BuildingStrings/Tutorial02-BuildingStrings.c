/* -----------------------------------------------
   Games may want to join different pieces of
   information into a string to print messages.
   This second step shows how to join together
   strings and numbers.
/  -------------------------------------------- */


// include Vircon libraries
#include "video.h"
#include "string.h"


// -----------------------------------------------
// STRING BUILDING FUNCTIONS
// -----------------------------------------------
// Since C string functions are too low level for
// regular use, we will write support functions
// to make it easier to join numbers into a string
// -----------------------------------------------


// This function takes a string and appends to it an integer number.
// Be careful! It assumes your array to be long enough.
void AddIntToString( int* String, int Number )
{
    // convert number to string
    int[ 20 ] NumberString;
    itoa( Number, NumberString, 10 );
    
    // concatenate the converted number to the original string
    strcat( String, NumberString );
}

// -----------------------------------------------

// This function takes a string and appends to it a float number.
// Be careful! It assumes your array to be long enough.
void AddFloatToString( int* String, float Number )
{
    // convert number to string
    int[ 20 ] NumberString;
    ftoa( Number, NumberString );
    
    // concatenate the converted number to the original string
    strcat( String, NumberString );
}


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{   
    // clear the screen in some visible color
    clear_screen( color_blue );
    
    // let's suppose we have some player state variables
    int Gold = 250;
    float LuckPercentage = 71.8;
    int[ 20 ] PlayerName;
    strcpy( PlayerName, "John" );
    
    // use our functions to join all our data
    int[ 30 ] Statement;
    strcpy( Statement, PlayerName );
    strcat( Statement, " has " );
    AddIntToString( Statement, Gold );
    strcat( Statement, " Gold and " );
    AddFloatToString( Statement, LuckPercentage );
    strcat( Statement, "% luck." );
    
    // now print the result statement on screen
    print_at( 20,20, Statement );
}
