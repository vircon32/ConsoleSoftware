// This helper function creates a regular icosahedron
// placed at the origin with 2 opposing vertices at
// the top and bottom. Size is the distance between them
void MakeIcosahedron( Object* O, float Size )
{
    // fill the object's general properties
    O->NumberOfPolygons = 20;
    O->NumberOfVertices = 12;
    O->WorldPosition.x = 0;
    O->WorldPosition.y = 0;
    O->WorldPosition.z = 0;
    O->AngleAroundX = 0;
    O->AngleAroundY = 0;
    O->AngleAroundZ = 0;
    
    // - - - - - - - - - - - - - - - - - - - - - -
    // Step 1: Position object vertices
    
    // top vertex
    O->Vertices[ 0 ].PositionInObject.x = 0;
    O->Vertices[ 0 ].PositionInObject.z = 0;
    O->Vertices[ 0 ].PositionInObject.y = Size/2;
    
    // upper set of 5 vertices
    for( int i = 0; i < 5; i++ )
    {
        float Angle = i * (2*pi / 5);
        O->Vertices[ 1+i ].PositionInObject.x = Size/2 * 0.866 * cos( Angle );
        O->Vertices[ 1+i ].PositionInObject.z = Size/2 * 0.866 * sin( Angle );
        O->Vertices[ 1+i ].PositionInObject.y = Size/4;
    }
    
    // lower set of 5 vertices
    for( int i = 0; i < 5; i++ )
    {
        float Angle = (i+0.5) * (2*pi / 5);
        O->Vertices[ 6+i ].PositionInObject.x = Size/2 * 0.866 * cos( Angle );
        O->Vertices[ 6+i ].PositionInObject.z = Size/2 * 0.866 * sin( Angle );
        O->Vertices[ 6+i ].PositionInObject.y = -Size/4;
    }
    
    // bottom vertex
    O->Vertices[ 11 ].PositionInObject.x = 0;
    O->Vertices[ 11 ].PositionInObject.z = 0;
    O->Vertices[ 11 ].PositionInObject.y = -Size/2;
    
    // - - - - - - - - - - - - - - - - - - - - - - -
    // Step 2: Build triangles from those vertices
    
    // top 5 triangles
    for( int i = 0; i < 5; i++ )
    {
        O->Polygons[ i ].Vertices[ 0 ] = &O->Vertices[ 0 ];
        O->Polygons[ i ].Vertices[ 1 ] = &O->Vertices[ 1+i ];
        O->Polygons[ i ].Vertices[ 2 ] = &O->Vertices[ 1+ (i+1)%5 ];
        O->Polygons[ i ].NumberOfVertices = 3;
        O->Polygons[ i ].NeutralColor = color_cyan;
    }
    
    // 5 center triangles pointing down
    for( int i = 0; i < 5; i++ )
    {
        O->Polygons[ 5+i ].Vertices[ 0 ] = &O->Vertices[ 1+i ];
        O->Polygons[ 5+i ].Vertices[ 1 ] = &O->Vertices[ 6+i ];
        O->Polygons[ 5+i ].Vertices[ 2 ] = &O->Vertices[ 1 + (i+1)%5 ];
        
        O->Polygons[ 5+i ].NumberOfVertices = 3;
        O->Polygons[ 5+i ].NeutralColor = color_green;
    }
    
    // 5 center triangles pointing up
    for( int i = 0; i < 5; i++ )
    {
        O->Polygons[ 10+i ].Vertices[ 0 ] = &O->Vertices[ 6+i ];
        O->Polygons[ 10+i ].Vertices[ 1 ] = &O->Vertices[ 6 + (i+1)%5 ];
        O->Polygons[ 10+i ].Vertices[ 2 ] = &O->Vertices[ 1+(i+1)%5 ];
        
        O->Polygons[ 10+i ].NumberOfVertices = 3;
        O->Polygons[ 10+i ].NeutralColor = color_green;
    }
    
    // bottom 5 triangles
    for( int i = 0; i < 5; i++ )
    {
        O->Polygons[ 15+i ].Vertices[ 0 ] = &O->Vertices[ 11 ];
        O->Polygons[ 15+i ].Vertices[ 1 ] = &O->Vertices[ 6+ (i+1)%5 ];
        O->Polygons[ 15+i ].Vertices[ 2 ] = &O->Vertices[ 6+i ];
        O->Polygons[ 15+i ].NumberOfVertices = 3;
        O->Polygons[ 15+i ].NeutralColor = color_red;
    }
}
