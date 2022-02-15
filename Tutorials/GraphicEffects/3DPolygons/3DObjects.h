// definitions of our minimal 3D engine
#define MaxPolygonVertices   4    // we only use triangles but an extra vertex is needed to interpolate
#define MaxObjectVertices   12    // just enough for our icosahedron
#define MaxObjectPolygons   20    // just enough for our icosahedron

// this represents point projections on screen
// and therefore uses ints (not floats)
struct Pixel2D
{
    int x, y;
};

// a general-purpose 3D vector;
// in our space, the axis are:
// - X grows towards right
// - Y grows upwards
// - Z grows away from the camera
struct Vector3D
{
    float x, y, z;
};

// vertices need to store static
// and transformed 3D positions,
// as well as their 2D projection
struct Vertex
{
    Vector3D PositionInObject;      // coordinates relative to object center (constant in a rigid body)
    Vector3D WorldPosition;         // world coordinates after object translation and rotation
    Pixel2D  ScreenProjection;      // screen coordinates of its projected pixel
};

// we are assuming all our polygons to be
// convex and have its vertices ordered
// consecutively in a clockwise manner when
// they face towards the screen
struct Polygon
{
    Vertex*[ MaxPolygonVertices ] Vertices;  // pointers to the vertices that define the polygon
    int NumberOfVertices;                    // the number of them actually used in this polygon
    Vector3D OuterNormal;                    // normal vector pointing outwards of the object
    int NeutralColor;                        // its constant true color, light independent
    int LightedColor;                        // color seen on camera, affected by lighting
};

// An object is made out of poligons, which
// in turn are debined as a set of vertices.
// Since many polygons will have common edges
// the vertices are defined at object level
// to avoid calculating vertices twice
struct Object
{
    Vertex[ MaxObjectVertices ] Vertices;   // all vertices present in the object
    Polygon[ MaxObjectPolygons ] Polygons;  // the faces that define object surface
    int NumberOfVertices;                   // number of vertices actually used in this object
    int NumberOfPolygons;                   // number of polygons actually used in this object
    Vector3D WorldPosition;                 // world coordinates of the object's reference point
    float AngleAroundX;                     // measured in radians
    float AngleAroundY;                     // measured in radians
    float AngleAroundZ;                     // measured in radians
};
