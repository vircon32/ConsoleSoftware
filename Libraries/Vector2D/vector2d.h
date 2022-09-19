/* *****************************************************************************
*  Vircon32 library: "vector2d.h"                 File version: 2022/09/19     *
*  --------------------------------------------------------------------------- *
*  This header defines 2D vectors and allows general operation with them.      *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef VECTOR2D_H
    #define VECTOR2D_H
    
    // include standard Vircon headers
    #include "math.h"
// *****************************************************************************


// =============================================================================
//   DEFINITION OF A 2D VECTOR
// =============================================================================


struct vector2d
{
    float x, y;
};


// =============================================================================
//   BASIC VECTOR PROPERTIES
// =============================================================================


bool v2_iszero( vector2d* v )
{
    return !v->x && !v->y;
}

// ---------------------------------------------------------

float v2_modulus( vector2d* v )
{
    float x = v->x;
    float y = v->y;
    return sqrt( x*x + y*y );
}

// ---------------------------------------------------------

float v2_modulus2( vector2d* v )
{
    float x = v->x;
    float y = v->y;
    return x*x + y*y;
}

// ---------------------------------------------------------

// careful! this will cause a hardware error
// for a null vector (its angle is undefined);
// angles are given in radians and grow anti-clockwise
float v2_angle( vector2d* v )
{
    return atan2( v->y, v->x );
}


// =============================================================================
//   ASSOCIATED VECTORS
// =============================================================================


// unit vector in the same direction; careful!
// this will cause a hardware error for a null vector
void v2_unit( vector2d* v, vector2d* result )
{
    float modulus = v2_modulus( v );
    result->x = v->x / modulus;
    result->y = v->y / modulus;
}

// ---------------------------------------------------------

// right normal vector (as seen on screen)
// with the same modulus as the original
void v2_right_normal( vector2d* v, vector2d* result )
{
    result->x =  v->y; 
    result->y = -v->x; 
}

// ---------------------------------------------------------

// left normal vector (as seen on screen)
// with the same modulus as the original
void v2_left_normal( vector2d* v, vector2d* result )
{
    result->x = -v->y; 
    result->y =  v->x; 
}


// =============================================================================
//   VECTOR ARITHMETIC
// =============================================================================


// the opposite vector, same as multiplying by -1
void v2_negative( vector2d* v, vector2d* result )
{
    result->x = -v->x;
    result->y = -v->y;
}

// ---------------------------------------------------------

void v2_add( vector2d* v1, vector2d* v2, vector2d* result )
{
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
}

// ---------------------------------------------------------

void v2_subtract( vector2d* v1, vector2d* v2, vector2d* result )
{
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
}

// ---------------------------------------------------------

void v2_multiply( vector2d* v, float number, vector2d* result )
{
    result->x = v->x * number;
    result->y = v->y * number;
}

// ---------------------------------------------------------

void v2_divide( vector2d* v, float number, vector2d* result )
{
    result->x = v->x / number;
    result->y = v->y / number;
}


// =============================================================================
//   VECTOR ARITHMETIC (COMPOUND ASSIGNMENTS)
// =============================================================================


// the opposite vector, same as multiplying by -1
void v2_self_negative( vector2d* self )
{
    self->x = -self->x;
    self->y = -self->y;
}

// ---------------------------------------------------------

void v2_self_add( vector2d* self, vector2d* other )
{
    self->x += other->x;
    self->y += other->y;
}

// ---------------------------------------------------------

void v2_self_subtract( vector2d* self, vector2d* other )
{
    self->x -= other->x;
    self->y -= other->y;
}

// ---------------------------------------------------------

void v2_self_multiply( vector2d* self, float number )
{
    self->x *= number;
    self->y *= number;
}

// ---------------------------------------------------------

void v2_self_divide( vector2d* self, float number )
{
    self->x /= number;
    self->y /= number;
}


// =============================================================================
//   VECTOR-VECTOR PRODUCTS
// =============================================================================


float v2_dot_product( vector2d* v1, vector2d* v2 )
{
    return (v1->x * v2->x) + (v1->y * v2->y);
}

// ---------------------------------------------------------

// result represents the Z axis component of a 3D vector
float v2_cross_product( vector2d* v1, vector2d* v2 )
{
    return (v1->x * v2->y) - (v1->y * v2->x);
}

// ---------------------------------------------------------

// careful! this will cause a hardware error if any
// of the vectors is null; result angle is given in
// radians and does not distinguish vector order
float v2_angle_between( vector2d* v1, vector2d* v2 )
{
    float angle_cosine = v2_dot_product( v1, v2 ) / (v2_modulus( v1 ) * v2_modulus( v2 ));
    return acos( angle_cosine );
}


// =============================================================================
//   PRINTING ON SCREEN
// =============================================================================


// if library print_numbers was included, we can also
// extend it with the ability to handle vector2d values
#ifdef PRINT_NUMBERS_H

// your array is assumed to be long enough, so be careful
void add_vector2d_to_string( int* string, vector2d* v )
{
    strcat( string, "( " );
    add_float_to_string( string, v->x );
    strcat( string, ", " );
    add_float_to_string( string, v->y );
    strcat( string, " )" );
}

// ---------------------------------------------------------

// this function will modify the selected region
void print_vector2d_at( int x, int y, vector2d* v )
{
    int[ 60 ] vector_string;
    add_vector2d_to_string( vector_string, v );
    print_at( x, y, vector_string );
}

#endif  // PRINT_NUMBERS_H


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
