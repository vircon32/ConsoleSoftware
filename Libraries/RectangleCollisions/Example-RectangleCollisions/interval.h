/* *****************************************************************************
*  Vircon32 library: "interval.h"                 File version: 2024/06/11     *
*  --------------------------------------------------------------------------- *
*  This header defines real number intervals and their general operations.     *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef INTERVAL_H
    #define INTERVAL_H
    
    // include standard Vircon headers
    #include "math.h"
// *****************************************************************************


// =============================================================================
//   DEFINITION OF A 1D REAL NUMBER INTERVAL
// =============================================================================


struct interval
{
    // careful! all functions assume that minimum <= maximum;
    // also note that both ends can use special float values:
    // - for an empty interval, both ends will be NaN
    // - for infinite intervals ends can be +/- infinity
    float minimum;
    float maximum;
};


// =============================================================================
//   HANDLING SPECIAL FLOAT FALUES
// =============================================================================


// this C language cannot work with nan or infinity
// values for floats, so we need to use this as a hack
union float_bits
{
    int bits;
    float value;
};

// these is the actual value used in some x86 machines
// for a "non-signaling NaN", but it may not be portable;
// however the way we treat it here should still work
#define bits_nan  0x7FC00000

// IEEE floats are actually required to support
// operating with infinities and NaN, but the Vircon32
// specification does not mention how these should
// be represented or treated; to ensure compatibility
// we will operate with the maximum positive and
// negative float values as if they were infinities
// (for our purposes this will work)
#define bits_plus_inf   0x7F7FFFFF   // actual +inf = 0x7F800000
#define bits_minus_inf  0xFF7FFFFF   // actual -inf = 0xFF800000

// ---------------------------------------------------------

float nan()
{
    float_bits converter;
    converter.bits = bits_nan;
    return converter.value;
}

// ---------------------------------------------------------

float plus_inf()
{
    float_bits converter;
    converter.bits = bits_plus_inf;
    return converter.value;
}

// ---------------------------------------------------------

float minus_inf()
{
    float_bits converter;
    converter.bits = bits_minus_inf;
    return converter.value;
}

// ---------------------------------------------------------

// careful! in most machines our value for NaN will not be
// equal to itself, so we need to check for NaN using this
bool is_nan( float f )
{
    float_bits converter;
    converter.value = f;
    return (converter.bits == bits_nan);
}


// =============================================================================
//   INTERVAL PROPERTIES
// =============================================================================


bool interval_is_empty( interval* i )
{
    return is_nan( i->minimum );
}

// ---------------------------------------------------------

float interval_length( interval* i )
{
    if( interval_is_empty( i ) )
      return -1;
    
    return i->maximum - i->minimum;
}

// ---------------------------------------------------------

float interval_center( interval* i )
{
    if( interval_is_empty( i ) )
      return nan();
    
    return (i->minimum + i->maximum) / 2;
}

// ---------------------------------------------------------

bool interval_contains_number( interval* i, float number )
{
    if( interval_is_empty( i ) )
      return false;
    
    return (number >= i->minimum) && (number <= i->maximum);    
}


// =============================================================================
//   MODIFYING INTERVALS
// =============================================================================


void interval_extend( interval* i, float distance )
{
    if( interval_is_empty( i ) )
      return;
    
    i->minimum -= distance;
    i->maximum += distance;
}

// ---------------------------------------------------------

void interval_displace( interval* i, float distance )
{
    if( interval_is_empty( i ) )
      return;
    
    i->minimum += distance;
    i->maximum += distance;
}


// =============================================================================
//   RELATIONS BETWEEN INTERVALS
// =============================================================================


void intervals_get_union( interval* i1, interval* i2, interval* result )
{
    // for empty intervals
    if( interval_is_empty( i1 ) )
    {
        *result = *i2;
        return;
    }
    
    if( interval_is_empty( i2 ) )
    {
        *result = *i1;
        return;
    }
    
    // if there is no intersection, return an empty interval
    // (since we can't represent disjoint intervals)
    if( (i1->minimum > i2->maximum) || (i1->maximum < i2->minimum) )
    {
        result->minimum = nan();
        result->maximum = nan();
        return;
    }
    
    // for regular cases
    result->minimum = fmin( i1->minimum, i2->minimum );
    result->maximum = fmax( i1->maximum, i2->maximum );
}

// ---------------------------------------------------------

void intervals_get_intersection( interval* i1, interval* i2, interval* result )
{
    // for empty intervals
    if( interval_is_empty( i1 ) || interval_is_empty( i2 ) )
    {
        result->minimum = nan();
        result->maximum = nan();
        return;
    }
    
    // if there is no intersection, return an empty interval
    if( (i1->minimum > i2->maximum) || (i1->maximum < i2->minimum) )
    {
        result->minimum = nan();
        result->maximum = nan();
        return;
    }
    
    result->minimum = fmax( i1->minimum, i2->minimum );
    result->maximum = fmin( i1->maximum, i2->maximum );
}

// ---------------------------------------------------------

bool intervals_overlap( interval* i1, interval* i2 )
{
    interval intersection;
    intervals_get_intersection( i1, i2, &intersection );
    return !interval_is_empty( &intersection );
}

// ---------------------------------------------------------

bool interval_contains_interval( interval* container, interval* contained )
{
    if( interval_is_empty( container ) )  return false;
    if( interval_is_empty( contained ) )  return false;
    
    return (container->minimum <= contained->minimum)
        && (container->maximum >= contained->maximum);
}


// =============================================================================
//   PRINTING ON SCREEN
// =============================================================================


// if library print_numbers was included, we can also
// extend it with the ability to handle interval values
#ifdef PRINT_NUMBERS_H

// auxiliary function to deal with possible infinity values
void add_interval_end_to_string( int* string, float interval_end )
{
    if( interval_end == plus_inf() )
      strcat( string, "+Inf" );
    
    else if( interval_end == minus_inf() )
      strcat( string, "-Inf" );
    
    else
      add_float_to_string( string, interval_end );
}

// ---------------------------------------------------------

// your array is assumed to be long enough, so be careful
void add_interval_to_string( int* string, interval* i )
{
    if( interval_is_empty( i ) )
    {
        strcat( string, "[]" );
        return;
    }
    
    // we need to deal with possible infinite values
    strcat( string, "[ " );
    add_interval_end_to_string( string, i->minimum );
    strcat( string, ", " );
    add_interval_end_to_string( string, i->maximum );
    strcat( string, " ]" );
}

// ---------------------------------------------------------

// this function will modify the selected region
void print_interval_at( int x, int y, interval* i )
{
    int[ 60 ] interval_string;
    add_interval_to_string( interval_string, i );
    print_at( x, y, interval_string );
}

#endif  // PRINT_NUMBERS_H


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
