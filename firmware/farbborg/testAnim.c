#include "config.h"
#include "util.h"
#include "api.h"
#include "testAnim.h"
#include "spike_hw.h"

// Playlist
void *display_loop(void * unused)  {
	while (1) {
		uart_putstr("funkyBeat()\n");
		funkyBeat();
		uart_putstr("symetricRoutes()\n");
		symetricRoutes();	
	
	    //uart_putstr("shiftTest()\n");
		//shiftTest();
		uart_putstr("fadeTest()\n");
		fadeTest();
	    uart_putstr("symetricRoutes()\n");
		symetricRoutes();
		//uart_putstr("cubes()\n");
		//cubes();
		//uart_putstr("brightnesTest()\n");
		//brightnesTest();
		uart_putstr("movingArrows()\n");
		movingArrows();
		uart_putstr("upgoingRandom()\n");
		upgoingRandom();
		uart_putstr("planeBall()\n");
		planeBall();
		uart_putstr("wobbeln()\n");
		wobbeln();
		uart_putstr("spirale()\n");
		spirale();
		uart_putstr("snake()\n");
		snake();
		uart_putstr("movingCubes()\n");
		movingCubes();
		uart_putstr("symetricRandom()\n");
		symetricRandom();
		uart_putstr("testAnim()\n");
		testAnim();
		uart_putstr("fnordLicht()\n");
		fnordLicht();
	}
	return 0;
}

void shiftTest() {
	int y, x;
	clearScreen(black);
	for (y = 0; y < MAX_Y; y++) {
		for (x = 0; x < MAX_X; x++) {
			imag[0][y][x][R] = 255;
			imag[0][y][x][G] = 255;
			imag[0][y][x][B] = 255;
		}
	}
	swapAndWait(2000);
	shift(up);
	swapAndWait(10000);	
}

void fadeTest() {
	clearScreen(black);
	clearImage(red);
	fade(5, 255);
	clearImage(blue);
	fade(5, 255);
	clearImage(green);
	fade(5, 255);
}


void brightnesTest() {
  unsigned char x, y, z, offset, rund, color, tmp;
  
  for (color = R; color <= B; color++) {
    offset = 1;
    for (rund = 0; rund <= 1; rund++) {
      for (x = 0; x < MAX_X; x++) {
	    for (y = 0; y < MAX_Y; y++) {
		  for (z = 0; z < MAX_Z; z++) {
            for (tmp = 0; tmp <= 2; tmp++) {
			  if (tmp == color)
                imag[x][y][z][tmp] = offset + z;
 			  else
				imag[x][y][z][tmp] = 0;
			  swapAndWait(20);
            } //tmp
		  } //z
		} //y
		offset += MAX_Z*MAX_Y;
      } //x
      offset = 130;
	} //rund
  } //color
}

#define FAKTOR (255/5) 

void test1() {
	unsigned char x, y, z, c;
	for (z = 0; z < MAX_Z; z++) {
		for (y = 0; y < MAX_Y; y++) {
			for (x = 0; x < MAX_X; x++) {
				for (c = 0; c < COLOR_BYTES; c++) {
					imag[z][y][x][c] = 255;
					swapAndWait(100);
				}
			}
		}
	}
}

typedef enum {
	Black,
	White,
	Red,
	Blue,
	Green,
	Yellow,
	C1,
	C2,
	C3
} beatColor;

typedef enum {
	chess,
	chessInv,
	all,
	smallCube,
	fade1,
	blockUp,
	blockDown,
	blockRight,
	blockDiagonal
} beatPattern;

typedef struct {
	beatPattern pP;
	beatColor bC;
	unsigned char time; // 128 is a full note   32 is 1/4   16 is 1/8
} beat;

void funkyBeat() {
	beat beats[31] = {// erster Takt
					  {chess,      White,   8}, 
				  	  {chessInv,   C2,     12},
					  {all,        Black,   4},
					  {blockDiagonal,        Red,     6},
					  {all,        Black,  10},	
					  {chess,      C1,     12},
					  {fade1,      Yellow, 16},
					  {chessInv,   C2,      8},
					  {chess,      Black,   8},
					  {blockDiagonal,   Red,     8},
					  {blockDown,       White,   8},
					  {smallCube,  Red,    12},
					  {chessInv,   Blue,    4},  
					  {all,        Green,   6},
					  {all,        Black,   2},
					  {chess,      Yellow,  8}, 
					  // zeiter Takt
					  {all,        Black,   8},
					  {smallCube,  White,  10},
					  {chess,      Black,   6},
					  {chessInv,   C3,      2},
					  {blockDiagonal,        Green,   8},
					  {smallCube,  Black,   8},
					  {chess,      Red,     8},
					  {all,        Black,   8},
					  {smallCube,  Yellow,  8},
					  {all,        Black,   8},
					  {fade1,       C2,     16},
					  {smallCube,  White,   4},
					  {chessInv,   Red,     6},
					  {blockUp,    Green,   2},
					  {chess,      C1,      8}
					};
	unsigned char i, j, b, x, y, z;
	color curColor;
	uint32_t *im;
	
	for (j = 0; j < 10; j++) {
		for (b = 0; b < 31; b++) {
			clearImage(black);
			switch (beats[b].bC) {
				case Black:
					curColor = black;
					break;
				case White:
					curColor = white;
					break;
				case Red:
					curColor = red;
					break;
				case Green:
					curColor = green;
					break;
				case Blue:
					curColor = blue;
					break;
				case Yellow:
					curColor = (color) {255, 255, 102};
					break;
				case C1:
					curColor = (color) {102, 102, 255};
					break;
				case C2:
					curColor = (color) {102, 102, 255};
					break;
				case C3:
					curColor = (color) {113, 120, 204};
					break;
			}
			switch (beats[b].pP) {
				case fade1:
				case all:
					clearImage(curColor);
					break;
					
				case chess:
					im = imag;
					for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
						if (i & 1) {
							*im++ = curColor.r;
							*im++ = curColor.g;
							*im++ = curColor.b;
						} else {
							im += 3;
						}
					}
					break;
					
				case chessInv:
					im = imag;
					for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
						if (!(i & 1)) {
							*im++ = curColor.r;
							*im++ = curColor.g;
							*im++ = curColor.b;
						} else {
							im += 3;
						}
					}
					break;
					
				case smallCube:
					for (z = 1; z < 4; z++) {
						for (y = 1; y < 4; y++) {
							for (x = 1; x < 4; x++) {
								setVoxel((voxel) {x, y, z}, curColor);
							}
						}
					}
					break;
					
				case blockUp:
					for (z = 2; z < 5; z++) {
						for (y = 0; y < 5; y++) {
							for (x = 0; x < 5; x++) {
								setVoxel((voxel) {x, y, z}, curColor);
							}
						}
					}
					break;	
				
				case blockDown:
					for (z = 0; z < 3; z++) {
						for (y = 0; y < 5; y++) {
							for (x = 0; x < 5; x++) {
								setVoxel((voxel) {x, y, z}, curColor);
							}
						}
					}
					break;
					
				case blockRight:
					for (z = 0; z < 5; z++) {
						for (y = 2; y < 5; y++) {
							for (x = 0; x < 5; x++) {
								setVoxel((voxel) {x, y, z}, curColor);
							}
						}
					}
					break;
					
				case blockDiagonal:
					for (z = 0; z < 5; z++) {
						for (y = 0; y < 5; y++) {
							for (x = 0; x < 5; x++) {
								if ((x + y + z) < 6)
									setVoxel((voxel) {x, y, z}, curColor);
							}
						}
					}
					break;
			}
			
			if (beats[b].pP == fade1)
				fade(16, beats[b].time);
			else 
				swapAndWait(beats[b].time*16);
		}
	}
}

#define NPOINTS 9
#define NLINES 12
void cubes() {
	// Drehender Quader
	voxel org[NPOINTS] = {{0x10, 0x10, 0x10}, // 0
							{0x60, 0x10, 0x10}, // 1
							{0x60, 0x60, 0x10}, // 2 
							{0x10, 0x60, 0x10}, // 3
							{0x10, 0x10, 0x60}, // 4
							{0x60, 0x10, 0x60}, // 5
							{0x60, 0x60, 0x60}, // 6
							{0x10, 0x60, 0x60}, // 7
							{0x00, 0x00, 0x00}  // 8
							};							
	unsigned char drawlist[NLINES*2] = {0, 1,
										0, 4,
										1, 2,
										1, 5,
										2, 3,
										2, 6,
										3, 0,
										3, 7,
										4, 5,
										5, 6,
										6, 7,
										7, 4
										};		
	
	voxel rot[NPOINTS], sca[NPOINTS], h1, h2; 
	unsigned int  a;
	unsigned char i;								
											
	for (a = 0; a < 1500; a++) {
		scale(a/35+40, a/35+40, a/35+40, org, sca, 8, (voxel) {0x30, 0x30, 0x30});
		rotate(a/3, a/5 , a/4, sca, rot, NPOINTS, (voxel) {0x38, 0x38, 0x38});
		
		for (i = 0; i < NLINES*2; i += 2) {
			h1 = rot[drawlist[i]];
			h2 = rot[drawlist[i+1]];
			drawLine3D(((char)h1.x+8)/16, ((char)h1.y+8)/16, ((char)h1.z+8)/16,
					   ((char)h2.x+8)/16, ((char)h2.y+8)/16, ((char)h2.z+8)/16, blue);
		}
		swapAndWait(6); 
		clearImage(black);
	}
	swapAndWait(20);
	// Drehende Phyramide
	org[2] = (voxel) {0x37, 0x54, 0x10};
	org[3] = (voxel) {0x37, 0x37, 0x48};
	drawlist[0]  = 0; drawlist[1]  = 1;
	drawlist[2]  = 1; drawlist[3]  = 2;
	drawlist[4]  = 2; drawlist[5]  = 0;
	drawlist[6]  = 0; drawlist[7]  = 3;
	drawlist[8]  = 1; drawlist[9]  = 3;
	drawlist[10] = 2; drawlist[11] = 3;	
	for (a = 0; a < 1500; a++) {
		scale(a/35+40, a/35+40, a/35+40, org, sca, 4, (voxel) {0x30, 0x30, 0x30});
		rotate(a/3, a/4 , a/5, sca, rot, 4, (voxel) {0x38, 0x38, 0x38});
		
		for (i = 0; i < 12; i += 2) {
			h1 = rot[drawlist[i]];
			h2 = rot[drawlist[i+1]];
			drawLine3D(((char)h1.x+8)/16, ((char)h1.y+8)/16, ((char)h1.z+8)/16,
					   ((char)h2.x+8)/16, ((char)h2.y+8)/16, ((char)h2.z+8)/16, blue);
		}
		swapAndWait(6); 
		clearImage(black);
	}
	wait(20);
	for (a = 0	; a < 4; a++) {
		org[a].x += 9;
		org[a].y += 9;
		org[a].z += 20;
	}	
	org[4] = (voxel) {0x40, 0x40, 0x40};
	scale(40, 40, 40, org, &org[5], 3, org[4]);
	drawlist[0]  = 0; drawlist[1]  = 4;
	drawlist[2]  = 1; drawlist[3]  = 4;
	drawlist[4]  = 2; drawlist[5]  = 4;
	drawlist[6]  = 3; drawlist[7]  = 4;
	drawlist[8]  = 0; drawlist[9]  = 5;
	drawlist[10] = 1; drawlist[11] = 6;	
	drawlist[12] = 2; drawlist[13] = 7;	
	drawlist[14] = 3; drawlist[15] = 8;	
	for (a = 0; a < 3000; a++) {
		rotate(a/3,  a/3 , a/4	,  org, rot, NPOINTS, org[4]);		
		rotate(a/20, a/20, a/20, &rot[5], &rot[5], 3, org[4]);
		for (i = 0; i < 16; i += 2) {
			h1 = rot[drawlist[i]];
			h2 = rot[drawlist[i+1]];
			drawLine3D(((char)h1.x+8)/16, ((char)h1.y+8)/16, ((char)h1.z+8)/16,
					   ((char)h2.x+8)/16, ((char)h2.y+8)/16, ((char)h2.z+8)/16, red);
		}
		swapAndWait(6); 
		clearImage(black);
	}
}

void testAnim() {
	unsigned char x, y, z, i;
	for (i = 0; i < 50; i++) {
		for (z = 0; z < MAX_Z; z++) {
			for (y = 0; y < MAX_Y; y++) {
				for (x = 0; x < MAX_X; x++) {
					switch (i % 6) {
						case 0:
							imag[z][y][x][R] = x * FAKTOR;
							imag[z][y][x][G] = y * FAKTOR;
							imag[z][y][x][B] = z * FAKTOR;
							break;
						case 1:
							imag[z][y][x][0] = x * FAKTOR;
							imag[z][y][x][1] = z * FAKTOR;
							imag[z][y][x][2] = y * FAKTOR;
							break;
						case 2:
							imag[z][y][x][0] = y * FAKTOR;
							imag[z][y][x][1] = x * FAKTOR;
							imag[z][y][x][2] = z * FAKTOR;
							break;
						case 3:
							imag[z][y][x][0] = y * FAKTOR;
							imag[z][y][x][1] = z * FAKTOR;
							imag[z][y][x][2] = x * FAKTOR;
							break;
						case 4:
							imag[z][y][x][0] = z * FAKTOR;
							imag[z][y][x][1] = x * FAKTOR;
							imag[z][y][x][2] = y * FAKTOR;
							break;
						case 5:
							imag[z][y][x][0] = z * FAKTOR;
							imag[z][y][x][1] = y * FAKTOR;
							imag[z][y][x][2] = x * FAKTOR;
							break;
					} 
				}
			}
		}
		if (i > 25) 
			swapAndWait(300);
		else 
			fade(20, 50);	
	}
	clearImage(black);
	fade(10, 50);
}

/*  4 2 2 2 2 2  
 *  3 2 1 1 1 2
 *  2 2 1 0 1 2
 *  1 2 1 1 1 2
 *  0 2 2 2 2 2 
 *    0 1 2 3 4 
 */
void movingArrows() {
     unsigned char x, y, cnt, cnt2 = 0;
	 color colors[4] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}};
     color curColor = {0,0,0};
	 for (cnt = 0; cnt < 100; cnt++) {
         shift(up);
         for (x = 0; x < MAX_X; x++) {
             for (y = 0; y < MAX_Y; y++) {
				switch (cnt % 3) {
					case 0:
						if (x == 2 && y == 2) {
							curColor = colors[cnt2++ % 4];
							setVoxel((voxel) {x, y, 0}, curColor);
						} 
						break;
					case 1:
						if ((x == 1 || x == 3) && (y == 1 || y == 3)) {
							setVoxel((voxel) {x, y, 0}, 
									 (color) {curColor.r/2, curColor.g/2, 
											  curColor.b/2});
						}
						break;
					case 2:
						if ((x == 0 || x == 4) && (y == 0 || y == 4)) {
							setVoxel((voxel) {x, y, 0}, 
									 (color) {curColor.r/4, curColor.g/4, 
											  curColor.b/4});
						}
						break;
				}
			 }	
         }
         fade(10, 10);
     }
     clearImage(black);
	 fade(10, 50);
}

void fnordLicht() {
	unsigned char i, j;
	unsigned short k;
	uint32_t * im;
	clearImage(red);
	fade(10, 40);
	for (j = 0; j < 2; j++) {
		for (i = 0; i < 255; i++) {
			im = imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = 255 - i;
				*im++ = i;
				*im++ = 0;
			}
			swapAndWait(15);
		}
		for (i = 0; i < 255; i++) {
			im = imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++;
				*im++ = 255 - i;
				*im++ = i;
			}
			swapAndWait(15);
		}
		for (i = 0; i < 255; i++) {
			im = imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = i;
				*im++;
				*im++ = 255 - i;
			}
			swapAndWait(15);
		}
	}
}

void symetricRandom() {
	unsigned char  i, j;
	voxel pos; 
	color col;
	for (j = 0; j < 254; j++) {
		clearScreen(black);
		for (i = 0; i < 10; i++) {
			pos.x = easyRandom() % 3;
			pos.y = easyRandom() % 3;
			pos.z = easyRandom() % 3;
			col.r = easyRandom();
			col.g = easyRandom();
			col.b = easyRandom();
			setSymetricVoxel(pos, col);
		}
		swapAndWait(15);
	}
	clearScreen(black);
}


void symetricRoutes() {
	color curColor = {160, 100, 170};
	voxel pos = {1, 1, 1}, help;
	unsigned char i;
	char addR = 3, addG = 1, addB = -2; 
    clearScreen(black);
	for (i = 0; i < 200; i++) {
		do {
			help = getNextVoxel(pos, (direction)((easyRandom()%6) + 1));
		} while (help.x > 2 || help.y > 2 || help.z > 2);
		pos = help;
		setSymetricVoxel(pos, curColor);
		swapAndWait(50);
		if ((i & 15) == 0)
			clearScreen(black);
		// prevent big color jumps
		if (curColor.r < 5 || curColor.r > 250) 
			addR = -addR;
		if (curColor.g < 5 || curColor.g > 250) 
			addG = -addG;
		if (curColor.b < 5 || curColor.b > 250) 
			addB = -addB;
		curColor.r += addR;
		curColor.g += addG;
		curColor.b += addB;
	}
}

void upgoingRandom() {
	color curColor = {100, 100, 100};
	unsigned char x, y, i;
	unsigned short j;
	signed char addR = 2, addG = -1, addB = 4; 
	for (j = 0; j < 600; j++) {
		for (i = 0; i < j%8 + 4; i++) {
			if (curColor.r < 8 || curColor.r > 248) 
				addR = -addR;
			if (curColor.g < 8 || curColor.g > 248) 
				addG = -addG;
			if (curColor.b < 8 || curColor.b > 248) 
				addB = -addB;
			curColor.r += addR;
			curColor.g += addG;
			curColor.b += addB;
			setVoxel((voxel) {easyRandom()%5, easyRandom()%5, 0}, curColor);
		}
		swapAndWait(18);
		shift(up);
		addR = (easyRandom() % 16) - 8;
		addG = (easyRandom() % 16) - 8;
		addB = (easyRandom() % 16) - 8;
	}
	clearImage(black);
	fade(10, 30);
	swapAndWait(1000);
}

static void drawCube(voxel pos, unsigned char color) {
	imag[pos.x][pos.y][pos.z][color]     = 255;
	imag[pos.x+1][pos.y][pos.z][color]   = 255;
	imag[pos.x][pos.y+1][pos.z][color]   = 255;
	imag[pos.x+1][pos.y+1][pos.z][color] = 255;
	imag[pos.x][pos.y][pos.z+1][color]     = 255;
	imag[pos.x+1][pos.y][pos.z+1][color]   = 255;
	imag[pos.x][pos.y+1][pos.z+1][color]   = 255;
	imag[pos.x+1][pos.y+1][pos.z+1][color] = 255;
}
 
/* ^
 * | 4   6 7 0 1 2
 * | 3   5   |>  3
 * y 2   4   +   4
 * | 1   3  <|   5
 * | 0   2 1 0 7 6
 * | 
 * |     0 1 2 3 4
 * +-------- x ------->
 */
static void drawLineZAngle(unsigned char angle, unsigned char z, color value) {
	// could be optimised in programcode
	unsigned char x1[8] = {2, 1, 0, 0, 0, 0, 0, 1};
	unsigned char y1[8] = {0, 0, 0, 1, 2, 3, 4, 4};
	unsigned char x2[8] = {2, 3, 4, 4, 4, 4, 4, 3};
	unsigned char y2[8] = {4, 4, 4, 3, 2, 1, 0, 0};
	angle &= 0x07;
	drawLine3D(x1[angle], y1[angle], z, x2[angle], y2[angle], z, value);	
}


void spirale() {
	unsigned char z, angle, count = 0, i = 0, index, value, angleAdd;
	signed short help; 
	color colors[5] = {{150,   0, 250},
					   { 50, 150, 180},
					   { 50, 220,  80},
					   {250, 150, 180},
					   {  0,   0,   0}
					  };
	color curColor;
	for (angleAdd = 0; angleAdd < 12; count++) {
		// Farbwerte interpolieren
		index = i / 16; // (12*5)/4 = 15
		value = i % 16;
		help  = colors[index+1].r; 
		help -= colors[index].r;
		help *= value;
		help /= 16;
		curColor.r = help + colors[index].r;
		help  = colors[index+1].g; 
		help -= colors[index].g;
		help *= value;
		help /= 16;
		curColor.g = help + colors[index].g;
		help  = colors[index+1].b; 
		help -= colors[index].b;
		help *= value;
		help /= 16;
		curColor.b = help + colors[index].b;
		//printf("%d %d %d  %d %d \n", curColor.r, curColor.g, curColor.b, index, value);
		for (angle = 0; angle < 8; angle++) {
			for (z = 0; z < 5; z++) {
				drawLineZAngle((angle+(angleAdd*z/4)) & 0x07, z, curColor);		
			}
			swapAndWait(30);
			clearScreen(black);
			
			if (count > 5) { 
				angleAdd++;
				count = 0;
			}
		}
		i++;
	}
	
	clearImage(black);
	fade(10, 30);
	swapAndWait(1000);
}

/*
static void drawPlane(uns) {
	
}
*/

void planeBall() {
	unsigned char x;
	clearScreen(black);
	// draw a red plane
	for (x = 0; x < 5; x++) {
		drawLine3D(x, 0, 1, 
		           x, 4, 1, red);
	}
	fade(5, 10);
	swapAndWait(1000);
	setVoxel((voxel) {2, 2, 5}, green);
	swapAndWait(100);
	setVoxel((voxel) {2, 2, 5}, black);
	setVoxel((voxel) {2, 2, 4}, green);
	swapAndWait(100);
	setVoxel((voxel) {2, 2, 4}, black);
	setVoxel((voxel) {2, 2, 3}, green);
	swapAndWait(100);
	setVoxel((voxel) {2, 2, 3}, black);
	setVoxel((voxel) {2, 2, 2}, green);
	swapAndWait(100);
	setVoxel((voxel) {2, 2, 2}, black);
	setVoxel((voxel) {2, 2, 3}, green);
	swapAndWait(100);
	setVoxel((voxel) {2, 2, 3}, black);
	setVoxel((voxel) {2, 2, 2}, green);
	swapAndWait(1000);
	clearImage(black);
	
	for (x = 0; x < 5; x++) {
		drawLine3D(x, 0, 0, 
		           x, 4, 2, red);
	}
	fade(5, 5);
	// now the ball has to roll to y = 0
	swapAndWait(400);
	setVoxel((voxel) {2, 2, 2}, black);
	setVoxel((voxel) {2, 1, 2}, green);
	swapAndWait(200);
	setVoxel((voxel) {2, 1, 2}, black);
	setVoxel((voxel) {2, 0, 1}, green);
		
	swapAndWait(1000);
	clearImage(black);
	
	for (x = 0; x < 5; x++) {
		drawLine3D(x, 0, 1, 
		           x, 4, 1, red);
	}
	setVoxel((voxel) {2, 0, 2}, green);
	fade(5, 5);
	swapAndWait(400);
	clearImage(black);
	for (x = 0; x < 5; x++) {
		drawLine3D(x, 0, 2, 
		           x, 4, 0, red);
	}
	setVoxel((voxel) {2, 0, 3}, green);
	fade(5, 5);
	swapAndWait(400);
	setVoxel((voxel) {2, 0, 3}, black);
	setVoxel((voxel) {2, 1, 2}, green);
	swapAndWait(200);
	setVoxel((voxel) {2, 1, 2}, black);
	setVoxel((voxel) {2, 2, 2}, green);
	swapAndWait(200);	
	setVoxel((voxel) {2, 2, 2}, black);
	setVoxel((voxel) {2, 3, 1}, green);
	swapAndWait(200);
	setVoxel((voxel) {2, 3, 1}, black);
	setVoxel((voxel) {2, 4, 1}, green);
	swapAndWait(200);
	setVoxel((voxel) {2, 2, 2}, black);
	setVoxel((voxel) {2, 3, 1}, green);
	swapAndWait(200);
	setVoxel((voxel) {2, 3, 1}, black);
	setVoxel((voxel) {2, 4, 1}, green);
	swapAndWait(200);
	
	swapAndWait(1000);
	clearImage(black);
	
	for (x = 0; x < 5; x++) {
		drawLine3D(x, 0, 1, 
		           x, 4, 1, red);
	}
	setVoxel((voxel) {2, 4, 2}, green);
	fade(5, 5);
}

void movingCubes() {
	// Startpoint of the cube
	voxel cube1 = {0, 0, 0}, cube2 = {3, 3, 3}, cube3 = {0, 3, 3};	
	direction way[] = {up, right, up, right, up, right, 
					   forward, forward, forward,
					   down, left, back, down, back, back,
					   down, left, left, up,  right, down, left, 0}; 
	unsigned char i, j; 
	for (j = 0; j < 5; j++) {
		i = 0;
		while(way[i]) {
			switch (way[i++]) {
				case up:      cube1.z++; 
							  cube2.x--;
							  cube3.z--;
							  if (cube1.z > MAX_Z-1) 
								cube1.z = 0;
							  if (cube2.x > MAX_X-1) 
								cube2.x = 0; 
							  if (cube3.z > MAX_Z-1) 
								cube3.z = 0;
							  break;
				case down:    cube1.z--; 
							  cube2.x++;
							  cube3.z++;
							  if (cube1.z > MAX_Z-1) 
								cube1.z = 0;
							  if (cube2.x > MAX_X-1) 
								cube2.x = 0; 
							  if (cube3.z > MAX_Z-1) 
								cube3.z = 0; 
							  break;
				case right:   cube1.x++; 
							  cube2.z--;
							  cube3.x++;
							  if (cube1.x > MAX_X-1) 
								cube1.z = 0;
							  if (cube2.z > MAX_Z-1) 
								cube2.x = 0; 
							  if (cube3.x > MAX_X-1) 
								cube3.x = 0;
							  break;
				case left:    cube1.x--; 
							  cube2.z++;
							  cube3.x--;
							  if (cube1.x > MAX_X-1) 
								cube1.x = 0;
							  if (cube2.z > MAX_Z-1) 
								cube2.z = 0; 
							  if (cube3.x > MAX_X-1) 
								cube3.x = 0;
							  break;
				case forward: cube1.y++; 
							  cube2.y--;
							  cube3.y--;
							  if (cube1.y > MAX_Y-1) 
								cube1.y = 0;
							  if (cube2.y > MAX_Y-1) 
								cube2.y = 0; 
							  if (cube3.y > MAX_Y-1) 
								cube3.y = 0;
							  break;
				case back:    cube1.y--; 
							  cube2.y++;
							  cube3.y++;
							  if (cube1.y > MAX_Y-1) 
								cube1.y = 0;
							  if (cube2.y > MAX_Y-1) 
								cube2.y = 0;
							  if (cube3.y > MAX_Y-1) 
								cube3.y = 0;
							  break;
				default: break;
			}
			
			drawCube(cube1, 0);
			drawCube(cube2, 1);
			drawCube(cube3, 2);
			swapAndWait(110);
			clearScreen(black);			
		}
	}
}

// a green Version of the Matrix of Borg 3D


#define SNAKE_LEN 100

void snake() {
	voxel pixels[SNAKE_LEN]; 
	voxel *head = &pixels[1];
	voxel *tail = &pixels[0];
	voxel old_head;
	color snakeColor = {100, 50, 220};
	voxel apples[10];
	unsigned char apple_num = 0;
	char addR = 2, addG = -1, addB = 1;
	pixels[0].x = 1; 
	pixels[0].y = 1;
	pixels[0].z = 0;
	pixels[1].x = 1; 
	pixels[1].y = 2;
	pixels[1].z = 0;
	
	direction dir = forward;

	clearScreen(black);

	unsigned char x = 0, dead = 0;
	while (1) {
		x++;
		old_head = *head;
		if (++head == pixels + SNAKE_LEN) 
			head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for (j = 0; j < apple_num; j++) {
			unsigned char i;
			for (i = 1; i < 7; i++) {
				if ((getNextVoxel(old_head, (direction)i).x == apples[j].x) && 
					(getNextVoxel(old_head, (direction)i).y == apples[j].y) &&
				    (getNextVoxel(old_head, (direction)i).z == apples[j].z)) {
					apple_found = 1;
					dir = (direction)i+1; 
					for(; j < apple_num-1; j++) {
						apples[j] = apples[j+1];
					}
					apple_num--;
					goto apple_se;
				}
			}
		}
		apple_se:
		if (!apple_found) {
			while (isVoxelSet(getNextVoxel(old_head, dir))) {
				if ((dead_cnt++) == 4) {
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}
		if (!dead) {
			*head = getNextVoxel(old_head, dir);
			setVoxel(*head, snakeColor);
			if (easyRandom() < 80) {
				dir = 1 + (direction) (easyRandom() % 6);
			}
			if ((apple_num < 10) && (easyRandom() < 10)) {
				voxel new_apple = (voxel) {easyRandom() % MAX_X,
										   easyRandom() % MAX_Y,
										   easyRandom() % MAX_Z};
				if (!isVoxelSet(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if (!apple_found) {
				setVoxel(*tail, black);
				if (++tail == pixels + SNAKE_LEN) 
					tail = pixels;
			}
		} else {
			while (tail != head) {
				setVoxel(*tail, black);
				if ((++tail) > pixels + SNAKE_LEN) 
					tail = pixels;
				wait(60);
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x & 1) { // let the apples blink
				setVoxel(apples[j], snakeColor);
			} else {
				setVoxel(apples[j], black);
			}
		}
		if (snakeColor.r < 5 || snakeColor.r > 250) 
			addR = -addR;
		if (snakeColor.g < 5 || snakeColor.g > 250) 
			addG = -addG;
		if (snakeColor.b < 5 || snakeColor.b > 250) 
			addB = -addB;
		snakeColor.r += addR;
		snakeColor.g += addG;
		snakeColor.b += addB;
		swapAndWait(60);
	}
}

void glowingBobbles() {
	
}

void wobbeln() {
	unsigned char i, j, z, y, x;
	color colors[5] = {{0,   0,  250},
				   { 250,  150,    0},
				   {0xef, 0xae, 0x30},
				   {   0  ,250,   18},
				   { 255,   0,     0}
				  };
	for (j = 0; j < 30; j++) {
		for (i = 1; i < 6; i++) {
			for (z = 0; z < i; z++) {
				for (y = 0; y < i; y++) {
					for (x = 0; x < i; x++) {
						switch (j % 8) {
							case 0: 
								setVoxel((voxel) {x, y, z}, colors[j%5]);
								break;
							case 1:
								setVoxel((voxel) {4-x, y, z}, blue);
								break;
							case 2:
								setVoxel((voxel) {x, 4-y, z}, red);
								break;
							case 3:
								setVoxel((voxel) {4-x, 4-y, z}, green);
								break;
							case 4: 
								setVoxel((voxel) {x, y, 4-z}, white);
								break;
							case 5:
								setVoxel((voxel) {4-x, y, 4-z}, red);
								break;
							case 6:
								setVoxel((voxel) {x, 4-y, 4-z}, blue);
								break;
							case 7:
								setVoxel((voxel) {4-x, 4-y, 4-z}, green);
								break;
							
						}
					}
				}
			}
			fade(5, 9);
		}
		swapAndWait(300);
		if (j < 20)
			clearImage(black);	
	}
	clearImage(black);
	fade(10, 30);
	swapAndWait(1000);
}
