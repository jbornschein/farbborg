#include "config.h"
#include "util.h"
#include "api.h"
#include "testAnim.h"
#include "fileParser.h"
#include "colorMatrix.h"
#include "colorSnake.h"
#include <math.h>

#define SNAKE_LEN 100

color HtoRGB(int h31bit);
void fadeTest();
void plasmaTest();
void plasmaSnake();
//void setVoxelHSV(int x, int y, int z, farbe hsv);
void setVoxelH(int x, int y, int z, double h);
static unsigned isqrt(unsigned long val);
//void testSin() ;
double offset;
unsigned int ioffset;

// Playlist
void *display_loop(void * unused)  {
	offset = 0.0;
	ioffset = 0;
	//testSin();
	printf("%d", isqrt(32786));
	while (1) {
		//gameOfLife(1, 1000);
		//gameOfLife(0, 1000);
	
		//colorSnake();
		//colorMatrix();
     	//pyramide();
		//fadeTest();
		plasmaTest();
		//plasmaSnake();
		//test1();
		//brightnesTest();
		//playPlaylist("playlist.apl");
        //playAnimFile("sample.anim");
		//funkyBeat();
		//testShift();
		//movingArrows(); 
		//upgoingRandom();
		//planeBall();
		//wobbeln();
		//symetricRoutes();
		//spirale(); 
		//snake(); 
		//movingCubes();
		//symetricRandom();
		//testAnim();
		//fnordLicht();
	}
}

void gameOfLife(unsigned char anim, unsigned int delay) {
	unsigned char gen, erg, seven = 0, maxGen = 0;
	signed char x, y, z, neighC, r1 = 3, r2 = 4, r3 = 3, r4 = 2;
	signed char i, j, k;
	char neighs[MAX_Z][MAX_Y][MAX_X];
	unsigned int count[MAX_Z][MAX_Y][MAX_X];
	unsigned int *h = (unsigned int *) count;
	for(i = 0; i < MAX_X*MAX_Y*MAX_Z; i++)
		*h++ = 0;
	
	clearScreen(black);

	switch (anim) {
	case 0:	maxGen = 8;
		setVoxel((voxel) {1, 2, 2}, red);
		setVoxel((voxel) {2, 2, 2}, red);
		setVoxel((voxel) {3, 2, 2}, red);
		break;
	case 1:	maxGen = 30; // other rules
		r3 = 2;
		r4 = 1;
		setVoxel((voxel) {0, 2, 2}, red);		
		setVoxel((voxel) {1, 2, 2}, red);
		//setVoxel((voxel) {2, 2, 2}, red);
		setVoxel((voxel) {3, 2, 2}, red);
		setVoxel((voxel) {4, 2, 2}, red);
		break;
	}
	for (gen = 0; gen < maxGen; gen++) {
		fade(16, 32);
		swapAndWait(delay);	
		for (x = 0; x < MAX_X; x++) {	
			for (y = 0; y < MAX_Y; y++) {
				for (z = 0; z < MAX_Z; z++) {
					neighC = 0;
					for (i = -1; i < 2; i++) {
						for (j = -1; j < 2; j++) {
							for (k = -1; k < 2; k++) {
								if (i != 0 || j != 0 || k != 0) {
									if ((x+i >= seven && x+i < MAX_X) && 
										(y+j >= seven && y+j < MAX_Y) && 
										(z+k >= seven && z+k < MAX_Z)) {
										erg = isVoxelSet((voxel) {x+i, y+j, z+k});
										if (erg == 2) // outside image ?
											erg = 0;
									} else {
										erg = 0;
									}
									if (erg) {
										neighC++;	
									}
									//printf("- %d %d %d  neigh=%d erg=%d\n", x+i, y+j, z+k, neighC, erg);
								}
							}
						}
					}
					neighs[x][y][z] = neighC;
				}
			}
		}
		for (x = 0; x < MAX_X; x++) {	
			for (y = 0; y < MAX_Y; y++) {
				for (z = 0; z < MAX_Z; z++) {
					//if (neighs[x][y][z])			
						//printf("%d %d %d neigh=%d %d\n", x, y, z, neighs[x][y][z], isVoxelSet((voxel){x, y, z}));
					if (isVoxelSet((voxel) {x, y, z})) { // Feld gesetzt
						if (neighs[x][y][z] > r3 || neighs[x][y][z] < r4) {
							//count[x][y][z] = 0;
							setVoxel((voxel) {x, y, z}, black);
						} 
					} else {
						if (neighs[x][y][z] >= r1 && neighs[x][y][z] <= r2) {
							setVoxel((voxel){x, y, z}, HtoRGB(count[x][y][z]*8192));
							count[x][y][z]++;
						}
					}
				}
			}
		}
		//printf("============ Gerneration %d \n", gen);
	}			
}

#define PI 3.14159265
#define SQUARE(x) (x)*(x)
#define fMAX_X (double) MAX_X
#define fMAX_Y (double) MAX_Y
#define fMAX_Z (double) MAX_Z

void plasmaSnake()
{
	double scale = 4.0;
	
	voxel pixels[SNAKE_LEN]; 
	voxel *head = &pixels[1];
	voxel *tail = &pixels[0];
	voxel old_head;
	double snakeColor;
	voxel apples[10];
	unsigned char apple_num = 0;
	//	char addR = 2, addG = -1, addB = 1;
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
		offset += 0.05;
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
					dir = (direction) (i+1); 
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
			snakeColor = 0.5;
			snakeColor += 0.5 * sin(((double)head->x * (PI / (fMAX_X * scale))) + ((double)head->y * (PI / (fMAX_Y * scale))) + ((double)head->z * (PI / (fMAX_Z * scale))) + offset);
			setVoxelH(head->x, head->y, head->z, snakeColor);
			if (easyRandom() < 80) {
				dir = (direction) ((easyRandom() % 6) + 1);
			}
			if ((apple_num<10) && (easyRandom()<10)) {
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
				snakeColor = 0.5;
				snakeColor += 0.5 * sin(((double)apples[j].x * (PI / (fMAX_X * scale))) + ((double)apples[j].y * (PI / (fMAX_Y * scale))) + ((double)apples[j].z * (PI / (fMAX_Z * scale))) + offset);
				setVoxelH(apples[j].x, apples[j].y, apples[j].z, snakeColor);
			} else {
				setVoxel(apples[j], black);
			}
		}
		/*if (snakeColor.r < 5 || snakeColor.r > 250) 
			addR = -addR;
		if (snakeColor.g < 5 || snakeColor.g > 250) 
			addG = -addG;
		if (snakeColor.b < 5 || snakeColor.b > 250) 
			addB = -addB;
			
		snakeColor.r += addR;
		snakeColor.g += addG;
		snakeColor.b += addB;
		*/
		swapAndWait(60);
	}
}

/* fast implemntation of sine
float sine(float x)
{
    const float B = 4/pi;
    const float C = -4/(pi*pi);

    float y = B * x + C * x * abs(x);

    #ifdef EXTRA_PRECISION
    //  const float Q = 0.775;
        const float P = 0.225;

        y = P * (y * abs(y) - y) + y;   // Q * y + P * y * abs(y)
    #endif
}
*/

typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

const static s16 sin_table[66] =
{
    0,  804, 1608, 2410, 3212, 4011, 4808, 5602,
 6393, 7179, 7962, 8739, 9512,10278,11039,11793,
12539,13279,14010,14732,15446,16151,16846,17530,
18204,18868,19519,20159,20787,21403,22005,22594,
23170,23731,24279,24811,25329,25832,26319,26790,
27245,27683,28105,28510,28898,29268,29621,29956,
30273,30571,30852,31113,31356,31580,31785,31971,
32137,32285,32412,32521,32609,32678,32728,32757,
32767,32757
};

s32 Sine(s32 phase)
{
	s16 s0;
	u16 tmp_phase, tmp_phase_hi;

	tmp_phase = phase & 0x7fff;

	if (tmp_phase & 0x4000) 
		tmp_phase = 0x8000 - tmp_phase;

	tmp_phase_hi = tmp_phase >> 8; // 0...64

	s0 = sin_table[tmp_phase_hi];

	s0 += ((s16)((((s32)(sin_table[tmp_phase_hi+1] - s0))*(tmp_phase&0xff))>>8));

	if (phase & 0x8000) {
		s0 = -s0;
	}
	
	return s0;
}

s32 Cosi(u32 phase)
{
	return Sine(phase + 0x4000);
}

/* by Jim Ulery  http://www.azillionmonkeys.com/qed/ulerysqroot.pdf  */
static unsigned isqrt(unsigned long val) {
	unsigned long temp, g=0, b = 0x8000, bshft = 15;
	do {
		if (val >= (temp = (((g << 1) + b)<<bshft--))) {
		   g += b;
		   val -= temp;
		}
	} while (b >>= 1);
	return g;
}

/*
void testSin() {
	float f, s, s2;
	for (f = 0.0f; f < 3*PI; f += 0.023f) {
		s = sin(f);
		s2 = (float) Sine((s32)((f/PI)*32768.0)) / 32768.0;
		printf("%f %f %f\n", s, s2, s-s2);
	}
}
*/

//  braunt√∂ne    194 124 71    255 203 143


#define SQRT0x8000 181
#define SQRT25x3   0x45483
void plasmaTest()
{
	s32 color, oldOffset, scale;
	s32 sqx, sqy, sqz;
	s32 x, y, z;
	scale = 3*0x5000;
	oldOffset = ioffset;
	
	while (1)
	{
		ioffset += 250; //700;
		
		for(x = 0; x < MAX_X; x++)
		{
			for(y = 0; y < MAX_Y; y++)
			{
				for(z = 0; z < MAX_Z; z++)
				{
					//reset color;
					color = 0;
					
					//diagonal scrolling sine 
					color += 0x2000 * Sine((-x * (0x8000*0x8000 / (MAX_X * scale))) + 
							               (y * (0x8000*0x8000 / (MAX_Y * scale))) + 
										   (-z * (0x8000*0x8000 / (MAX_Z * scale))) + 
										   ioffset
										   ) / 0x8000;
					
					//polar sine
					sqx  = x - 2;
					sqx *= sqx*0x8000;
					sqy  = y - 2;
					sqy *= sqy*0x8000;
					sqz  = z - 2;
					sqz *= sqz*0x8000;
					color += 0x8000/5 * Sine(
						isqrt(sqx + sqy + sqz)*SQRT0x8000/8 + ioffset + 
					    (x*y*z*0x8000*20)/(scale*(1+x+y+z))
				    ) / 0x8000;  //end of sine
					//printf("%06d ", isqrt(sqx + sqy + sqz));
					//sum of x-sine and y-sine and z-sine
					color +=  (
							  (0x3200 * Sine((x * (0x8000*0x8000 / (MAX_X * scale))) + ioffset) / 0x8000) 
							+ (0x4900 * Sine((-y * (0x8000*0x8000 / (MAX_Y * scale))) + ioffset) / 0x8000)
							+ (0x4400 * Sine((-z * (0x8000*0x8000 / (MAX_Z * scale))) + ioffset) / 0x8000)
							) / (0x13000);
					
					//divide by number of added sines, to re-scale to colorspace	
					//color /= 1.0;
					
					//colorspace offset
					color += 0x2500 + (ioffset/32);
					
					setVoxelH(x, y, z, color/32768.);
				} 
				//printf("\n");
			}
			//printf("\n");
		}
		//printf("\n");
		
		swapAndWait(30);
		
		if((ioffset-oldOffset) >= 4*0x8000)
			break;
	}
}


color HtoRGB(int h31bit)
{
	if (h31bit < 0)
		h31bit += 49152;
	if (h31bit > 49152)
		h31bit -= 49152;
	      color rgb;
 
 		unsigned char sextant;
		int           q;
		sextant  = h31bit / 8192;   // 60¬∞
		
		h31bit     = h31bit % 8192;
		q          = 8192 - h31bit;	
		//h31bit     = 8192 - (8192 - h31bit);
		//printf("%d %d %d\n", h31bit, sextant, help);
		switch(sextant)
		{
		    case 0:
				rgb.r = 255;
				rgb.g = h31bit / 32; 
				rgb.b = 0;
				break;
		    case 1:
				rgb.r = q / 32;
				rgb.g = 255;
				rgb.b = 0;
				break;
		    case 2:
				rgb.r = 0;
				rgb.g = 255;
				rgb.b = h31bit / 32;
				break;
		    case 3:
				rgb.r = 0;
				rgb.g = q / 32;
				rgb.b = 255;
				break;
		    case 4:
				rgb.r = h31bit / 32;
				rgb.g = 0;
				rgb.b = 255;
				break;
		    default:
				rgb.r = 255;
				rgb.g = 0;
				rgb.b = q / 32;
				break;
   };
    return(rgb);
};

void setVoxelH(int x, int y, int z, double h)
{
	h -= floor(h);
	setVoxel((voxel) {x, y, z}, HtoRGB((int) (h*49152)));
}

void fadeTest() {
	clearScreen(black);
	clearImage(red);
	fade(10, 100);
	clearImage(blue);
	fade(10, 100);
	clearImage(green);
	fade(10, 100);
}


#define NPOINTS 9
#define NLINES 12
void pyramide() {
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

#define FAKTOR (255/5) 

void test1() {
	unsigned char x, y, z, c;
	for (z = 0; z < MAX_Z; z++) {
		for (y = 0; y < MAX_Y; y++) {
			for (x = 0; x < MAX_X; x++) {
				for (c = 0; c < COLOR_BYTES; c++) {
					imag[z][y][x][c] = 255;
					swapAndWait(200);
				}
			}
		}
	}
}

void brightnesTest() {
// Written by Michael Psarros, just test all the possible brightness values
  unsigned char x, y, z; // the position
  unsigned char offset, rund; // 255 bright values but only 125 leds, so
							  // we need 2 runds for showing all the values
  unsigned char color, tmp;   // actual colors

  for (color = R; color <= B; color++) { // iterate over all three colors
    offset = 1; // first run: brightness 1-126
    for (rund = 0; rund <= 1; rund++) {
      for (x = 0; x < MAX_X; x++) {
	    for (y = 0; y < MAX_Y; y++) {
		  for (z = 0; z < MAX_Z; z++) {
            for (tmp = 0; tmp <= 2; tmp++) { // set all the color values
			  if (tmp == color)
                imag[x][y][z][tmp] = offset + z;
 			  else
				imag[x][y][z][tmp] = 0;
			} //tmp
			swapAndWait(200);
		  } //z
		  offset += MAX_Z; // update the offset after every column
		} //y
      } //x
      offset = 130; // in the second run get values 130-255
	} //rund
  } //color
}

void testShift() {
	unsigned char z;
	clearImage(black);
	swapAndWait(1000);
	setVoxel((voxel) {0, 0, 0}, white);
	for (z = 0; z < 5; z++) {
		shift(up);
		swapAndWait(1000);
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
			swapAndWait(250);
		else 
			fade(20, 40);	
	}
	clearImage(black);
	fade(10, 50);
}

void fnordLicht() {
	unsigned char *im, i, j;
	unsigned short k;

	for (j = 0; j < 2; j++) {
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = 255 - i;
				*im++ = i;
				*im++ = 0;
			}
			swapAndWait(20);
		}
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++;
				*im++ = 255 - i;
				*im++ = i;
			}
			swapAndWait(20);
		}
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = i;
				*im++;
				*im++ = 255 - i;
			}
			swapAndWait(20);
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
	smallCubeInc,
	fade10
} beatPattern;

typedef struct {
	beatPattern pP;
	beatColor bC;
	unsigned char time; // 128 is a full note   32 is 1/4   16 is 1/8
} beat;

void funkyBeat() {
	beat beats[31] = {// erster Takt
					  {chess,      White,   8}, 
				  	  {chessInv,   Green,  12},
					  {all,        Black,   4},
					  {all,        Red,     6},
					  {all,        Black,  10},	
					  {chess,      C1,     12},
					  {all,        Black,  16},
					  {chessInv,   C2,      8},
					  {chess,      Black,   8},
					  {chessInv,   Red,     8},
					  {all,        Black,   8},
					  {smallCube,  Red,     12},
					  {chessInv,   Blue,    4},  
					  {all,        Green,   6},
					  {all,        Black,   2},
					  {chess,      Yellow,  8}, 
					  // zeiter Takt
					  {all,        Black,   8},
					  {smallCube,  White,  10},
					  {chess,      Black,   6},
					  {chessInv,   Blue,    2},
					  {all,        Green,   8},
					  {smallCube,  Black,   8},
					  {chess,      Red,     8},
					  {all,        Black,   8},
					  {smallCube,  Yellow,  8},
					  {all,        Black,   8},
					  {chess,      Yellow, 16},
					  {smallCube,  White,   4},
					  {chessInv,   Red,     6},
					  {all,        Black,   2},
					  {chess,      C1,      8}
					  
					};
	unsigned char i, j, b, x, y, z;
	color curColor;
	unsigned char *im;
	
	for (j = 0; j < 10; j++) {
		for (b = 0; b < 31; b++) {
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
					curColor = (color) {255, 255,   0};
					break;
				case C1:
					curColor = (color) {180, 120, 220};
					break;
				case C2:
					curColor = (color) {100, 120, 220};
					break;
				case C3:
					curColor = (color) {180,  60, 240};
					break;
			}
			switch (beats[b].pP) {
				case all:
					clearImage(curColor);
					break;
				case chess:
					im = (unsigned char *) imag;
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
					im = (unsigned char *) imag;
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
					for (z = 1; z < 2; z++) {
						for (y = 2; y < 3; y++) {
							for (x = 1; x < 2; x++) {
								setVoxel((voxel) {x, y, z}, curColor);
							}
						}
					}
					break;
			}
			swapAndWait(beats[b].time*16);
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
		swapAndWait(20);
	}
	clearScreen(black);
}


void symetricRoutes() {
	color curColor = {100, 100, 100};
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
	unsigned char x, y, i, j;
	signed char addR = 2, addG = -1, addB = 4; 
	for (j = 0; j <200; j++) {
		for (i = 0; i < 6; i++) {
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
		swapAndWait(30);
		shift(up);
		addR = (easyRandom() % 16) - 8;
		addG = (easyRandom() % 16) - 8;
		addB = (easyRandom() % 16) - 8;
	}

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
	swapAndWait(5000);
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
	setVoxel((voxel) {2, 0, 2}, green);
	fade(5, 5);
}

void movingCubes() {
	// Startpoint of the cube
	voxel cube1 = {0, 0, 0}, cube2 = {3, 3, 3}, cube3 = {0, 3, 3};	
	direction way[] = {up, right, up, right, up, right, 
					   forward, forward, forward,
					   down, left, back, down, back, back,
					   down, left, left, up,  right, down, left, (direction) 0}; 
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
					dir = (direction) (i+1); 
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
				dir = (direction) ((easyRandom() % 6) + 1);
			}
			if ((apple_num<10) && (easyRandom()<10)) {
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

/*  4 2 2 2 2 2  
 *  3 2 1 1 1 2
 *  2 2 1 0 1 2
 *  1 2 1 1 1 2
 *  0 2 2 2 2 2 
 *    0 1 2 3 4 
 */
void movingArrows() {
     unsigned char x, y, cnt, cnt2 = 0;
	 color colors[4] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {0, 255, 255}};
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
								setVoxel((voxel) {x, y, 4-z}, colors[j%5]);
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
			fade(10, 9);
		}
		swapAndWait(300);
		if (j < 20)
			clearImage(black);	
	}
	clearImage(black);	
	
}


