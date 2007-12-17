#include "spike_hw.h"
#include "config.h"
#include "util.h"
#include "api.h"

color black = {  0,   0,   0};
color white = {255, 255, 255};
color red   = {255,   0,   0};
color green = {  0, 255,   0};
color blue  = {  0,   0, 255};

uint32_t imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

uint32_t pixmap_readback[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

// a voxel is compareabel to a pixel in 2D, with the differents, that it has 
// a volume
void setVoxel(voxel pos, color c) {
	uint32_t *im;
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) {
		im = &imag[pos.z][pos.y][pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
	}
}

void setSymetricVoxel(voxel pos, color c) {
	uint32_t *im;
	if (pos.x < (MAX_X+1)/2 && pos.y < (MAX_Y+1)/2 && pos.z < (MAX_Z+1)/2) {
		im = &imag[pos.z+2][pos.y+2][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][pos.y+2][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[pos.z+2][2-pos.y][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][2-pos.y][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[pos.z+2][pos.y+2][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][pos.y+2][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[pos.z+2][2-pos.y][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][2-pos.y][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
	}
}

unsigned char isVoxelSet(voxel pos) {
	// check if voxel exists
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) {
		if (imag[pos.z][pos.y][pos.x][R] ||
			imag[pos.z][pos.y][pos.x][G] ||
			imag[pos.z][pos.y][pos.x][B]) {
			return 1; // set color != black
		}
		return 0;     // is black 
	} 
	return 2;         // is outside image
}

voxel getNextVoxel(voxel pos, direction d) {
	switch (d) {
		case up:	  pos.z++; break;
		case down:	  pos.z--; break;
		case back:	  pos.y++; break;
		case forward: pos.y--; break;
		case right:	  pos.x++; break;
		case left:	  pos.x--; break;
		default: break; 
	}
	return pos;
}

direction direction_r(direction dir) {
	switch (dir) {
		case right:
			return back;
		case down:
			return left;
		case left:
			return up;
		case up:
			return forward;	
		case back:
			return down;
		case forward:
			return right;
		default:
			return dir;
	}
}

// too big for a real avr
/*void fade(unsigned int msProStep, unsigned int steps) {
	int32_t s;
	int32_t addColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	int32_t *aC = addColor;
	int32_t helpColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	int32_t *help = helpColor;
	int32_t z, i;
	uint32_t *pixr = pixmap_readback;
	uint32_t *pix;
	uint32_t *im  = (uint32_t *) imag;
	unsigned char val;
 	
	for (z = 0; z < MAX_Z; z++) {
		for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
			*help = *pixr * 1024;
			*aC   = ((int32_t) (*im * 1024) - *help)/steps;
			aC++;
			pixr++;
			help++;
			im++;
		}
	}
	
 	for (s = 0; s < steps; s++) {
		aC   = addColor;
		help = helpColor;
		im   = imag;
		pixr = pixmap_readback;
		for (z = 0; z < MAX_Z; z++) {
			pix = (uint32_t *) &PIXMAP[128*z];
			for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
				*help += *aC++; 
				val  = (*help++ + 512) / 1024;
				*pix++  = val;
				*pixr++ = val;
			}
		}
		myWait(msProStep);
	}
	swapAndWait(msProStep);
}*/

void fade(unsigned int msProStep, unsigned int steps) {
	uint32_t s;
	int32_t addColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	int32_t *aC = addColor;
	int32_t i, z, j, helpColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	int32_t *help = helpColor;
	uint32_t *pix, val; 
	uint32_t  *im  = imag, *pixr = pixmap_readback;
	char buffer[80];
	
	
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X*COLOR_BYTES; i++) {
		*help = *pixr * 1024;
		*aC   = (int32_t)((int32_t) (*im * 1024) - *help)/(int32_tamke)steps;
		//sprintf(buffer, "%d %d %d %d\n", *aC, *help, *pixr, *im);
		//uart_putstr(buffer);
		
		pixr++;
		help++;
		aC++;
		im++;
	}

 	for (s = 0; s < steps; s++) {
		aC   = addColor;
		help = helpColor;
		pixr = pixr = pixmap_readback;
		im   = imag;
		for (z = 0; z < MAX_Z; z++) {
			pix = &PIXMAP[128*z];
			for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
				*help += *aC++; 
				val  = (*help++ + 512) / 1024;
				*pix++  = val;
				*pixr++ = val;
			}
		}
		myWait(msProStep);
	}
	swapAndWait(msProStep);
}

void swapAndWait(unsigned int ms) {
	uint32_t *pix, *im = imag;
	uint32_t *pixr = pixmap_readback;
	uint32_t i, z, help;
	for (z = 0; z < MAX_Z; z++) {
		pix = (uint32_t *) &PIXMAP[128*z];
		for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
			help = *im++;
			*pix++  = help;
			*pixr++ = help;	
		}
	}
	myWait(ms);
}

void clearScreen(color c) {
	uint32_t *im = imag, *pixr = pixmap_readback;
	uint32_t  *pix;
	unsigned char i, z;
	for (z = 0; z < MAX_Z; z++) {
		pix = &PIXMAP[128*z];
		for (i = 0; i < MAX_Y*MAX_X; i++) {
			*pix++ = c.r;
			*pixr++ = c.r; 
			*im++  = c.r;
			*pix++ = c.g;
			*pixr++ = c.g;
			*im++  = c.g;
			*pix++ = c.b;
			*pixr++ = c.b;
			*im++  = c.b;
		}
	}
}

void clearImage(color c) {
	uint32_t *im = imag;
	unsigned short i;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
		*im++  = c.r;
		*im++  = c.g;
		*im++  = c.b;
	}
}

#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char easyRandom() {
	static unsigned int muh = 0xAA;
	unsigned char x;
	for (x = 0; x < 8; x++) {
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ 
		      BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;
}

/*
unsigned char get_voxel(voxel p);
unsigned char get_next_voxel(voxel p, direction dir);

voxel next_voxel(voxel pix, direction dir);
*/

void shift(direction dir) {
	unsigned char i, z;
	uint32_t *im  = imag;
	
	switch (dir) {
		case up:
			for (z = 4; z < MAX_Z; z--) {
				im = (uint32_t *) &imag[z][0][0][0];
				for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
					im[MAX_Y*MAX_X*COLOR_BYTES] = *im++; 
				}
			}
			im = (uint32_t *) imag;
			for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
				*im++ = 0; 
			}
			break;
		case down:
			break;
		case forward:
			break;
		case back:
			break;
		case right:
			break;
		case left:
			break;
		default:
			break;
	}
}


/** Draws a thredimentional line with the bressenham 3d algrorthmus form the point
  * px1, py1, xz1 to the point px2, py2, pz2 with the brightness value.
  */
void drawLine3D(char px1, char py1, char pz1, 
 			    char px2, char py2, char pz2, color value) {
    int8_t i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
	int8_t curx = px1, cury = py1, curz = pz1;
    dx = (px2 - px1);
    dy = (py2 - py1);
    dz = (pz2 - pz1);
    x_inc = (dx < 0) ? -1 : 1;	// sign
    l = dx >= 0 ? dx : -dx;    	// abs
    y_inc = (dy < 0) ? -1 : 1;  
    m = dy >= 0 ? dy : -dy;
    z_inc = (dz < 0) ? -1 : 1;
    n = dz >= 0 ? dz : -dz;
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;

    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            setVoxel((voxel) {curx, cury, curz}, value);
            if (err_1 > 0) {
                cury += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                curz += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            curx += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            setVoxel((voxel) {curx, cury, curz}, value);
            if (err_1 > 0) {
                curx += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                curz += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            cury += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            setVoxel((voxel) {curx, cury, curz}, value);
            if (err_1 > 0) {
                cury += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                curx += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            curz += z_inc;
        }
    }
   	setVoxel((voxel) {curx, cury, curz}, value);
}

// 16 = sin(90¡) = 1
char sinTab[] = {0, 6, 12, 19, 24, 30, 36, 41, 45, 49, 53, 56, 59, 61, 63, 64, 64};
		 
char Sin(unsigned char a) {
	a %= 64;
	if (a < 17) {
		return  sinTab[a];
	} else if (a < 33) {
		return  sinTab[32-a];
	} else if (a < 49) {
		return -sinTab[a-32];
	} else {
		return -sinTab[64-a];
	}
}	


voxel mulMatrixPoint(char *mat, voxel *p) {
	return (voxel) {
		(mat[0]*(char)p->x)/64 + (mat[1]*(char)p->y)/64 + (mat[2]*(char)p->z)/64 + mat[3],
		(mat[4]*(char)p->x)/64 + (mat[5]*(char)p->y)/64 + (mat[6]*(char)p->z)/64 + mat[7],
		(mat[8]*(char)p->x)/64 + (mat[9]*(char)p->y)/64 + (mat[10]*(char)p->z)/64 + mat[11]
	};
}

/*
Matrix Format

  0  1  2  3
  4  5  6  7  
  8  9 10 11
(12 13 14 15) not exist  because normally 0 0 0 1
              but works intern with homogen coordiantes
*/
void rotate(char a, char b, char c, voxel* points, 
			voxel* resPoints, int numPoint, voxel rotP) {
	char mat[12];
	unsigned char i;
					
	// Initialiesierung der Rotationsmatrix				
	mat[0] = (Cos(b)*Cos(c))/64;
	mat[1] = (-Cos(b)*Sin(c))/64;
	mat[2] = -Sin(b);
		
	mat[4] = (((-Sin(a)*Sin(b))/64)*Cos(c))/64 + (Cos(a)*Sin(c))/64;
	mat[5] = (((Sin(a)*Sin(b))/64)*Sin(c))/64 + (Cos(a)*Cos(c))/64;
	mat[6] = (-Sin(a)*Cos(b))/64;
		
	mat[8]  = (((Cos(a)*Sin(b))/64)*Cos(c))/64 + (Sin(a)*Sin(c))/64;
	mat[9]  = (((-Cos(a)*Sin(b))/64)*Sin(c))/64 + (Sin(a)*Cos(c))/64;
	mat[10] = (Cos(a)*Cos(b))/64;
					
	if (rotP.x == 0 && rotP.y == 0 && rotP.z == 0) {
		mat[3]  = 0;
		mat[7]  = 0;
		mat[11] = 0;
	} else {
		mat[3]  = rotP.x - ((mat[0]*(char)rotP.x)/64 + (mat[1]*(char)rotP.y)/64 + (mat[2]*(char)rotP.z)/64);
		mat[7]  = rotP.y - ((mat[4]*(char)rotP.x)/64 + (mat[5]*(char)rotP.y)/64 + (mat[6]*(char)rotP.z)/64);
		mat[11] = rotP.z - ((mat[8]*(char)rotP.x)/64 + (mat[9]*(char)rotP.y)/64 + (mat[10]*(char)rotP.z)/64);
	}
	/*
	for (i = 0; i < 3; i++) {
		printf("%d\t%d\t%d\t%d\n", mat[(i*4)], mat[(i*4)+1], mat[(i*4)+2], mat[(i*4)+3]);
	}*/
	
	for (i = 0; i < numPoint; i++) {
		resPoints[i] = mulMatrixPoint(mat, &points[i]);
	}	
}

void scale(char sx, char sy, char sz, voxel* points, 
			voxel* resPoints, int numPoint, voxel scaleP) {
	char mat[12] = {sx,  0,  0,  scaleP.x - (sx*scaleP.x)/64,
					 0, sy,  0,  scaleP.y - (sy*scaleP.y)/64,
					 0,  0, sz,  scaleP.z - (sz*scaleP.z)/64};
	unsigned char i;
 	for (i = 0; i < numPoint; i++) {
		resPoints[i] = mulMatrixPoint(mat, &points[i]);
	}			
}					

