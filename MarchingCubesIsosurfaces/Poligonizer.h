typedef struct {
	double x, y, z;
} XYZ;

typedef struct {
	XYZ p[3];
} TRIANGLE;

typedef struct {
	XYZ p[8];
	double val[8];
} GRIDCELL;

class Poligonizer
{
public:
	Poligonizer();
	~Poligonizer();
	int Polygonize(GRIDCELL grid, double isolevel, TRIANGLE *triangles);
private:
	static const int edgeTable[256];
	static const int triTable[256][16];
	XYZ VertexInterp(double isolevel, XYZ p1, XYZ p2, double valp1, double valp2);
};

