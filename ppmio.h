typedef struct{
	int w;
	int h;
} dimension;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} color;

typedef struct {
	unsigned char* map_r;
	unsigned char* map_g;
	unsigned char* map_b;
	dimension dim;
	unsigned char max;
} pixmap;

pixmap* createPixMap(int, int, int);
pixmap* openPixMap(char*);

color getPix(pixmap*, int, int);

void setPix(pixmap* , color, int, int);
void writePixMap(char* , pixmap*);
void freeMap(pixmap*);
void invertPixMap(pixmap*);
void mirrorHorPixMap(pixmap*);
void mirrorVerPixMap(pixmap*);
void setMaxPix(pixmap*,unsigned char);
void setMinPix(pixmap*,unsigned char);
void rotatePixMap(pixmap**);

