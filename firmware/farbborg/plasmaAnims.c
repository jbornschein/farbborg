#include "plasmaAnims.h"
#include "config.h"
#include "api.h"
#include <math.h>

float offset;
unsigned int ioffset;

#define PI 3.14159265
#define SQUARE(x) (x)*(x)
#define fMAX_X (float)MAX_X
#define fMAX_Y (float)MAX_Y
#define fMAX_Z (float) MAX_Z

#define SNAKE_LEN 100

#define max(x, y) ((x > y)?x:y)

void plasmaSnake()
{
	float scale = 4.0;
	
	voxel pixels[SNAKE_LEN]; 
	voxel *head = &pixels[1];
	voxel *tail = &pixels[0];
	voxel old_head;
	float snakeColor;
	voxel apples[10];
	unsigned char apple_num = 0;
	//char addR = 2, addG = -1, addB = 1;
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
		offset += 0.09;
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
			snakeColor = 0.5;
			snakeColor += 0.5 * sinf(((float)head->x * (PI / (fMAX_X * scale))) + ((float)head->y * (PI / (fMAX_Y * scale))) + ((float)head->z * (PI / (fMAX_Z * scale))) + offset);
			setVoxelH_f(head->x, head->y, head->z, snakeColor);
			if (easyRandom() < 80) {
				dir = 1 + (direction) (easyRandom() % 6);
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
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x & 1) { // let the apples blink
				snakeColor = 0.5;
				snakeColor += 0.5 * sinf(((float)apples[j].x * (PI / (fMAX_X * scale))) + ((float)apples[j].y * (PI / (fMAX_Y * scale))) + ((float)apples[j].z * (PI / (fMAX_Z * scale))) + offset);
				setVoxelH_f(apples[j].x, apples[j].y, apples[j].z, snakeColor);
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
		swapAndWait(90);
	}
}


#define SQRT0x8000 181
#define SQRT25x3   0x45483
void plasmaTest()
{
	int32_t color, scale;
	int32_t sqx, sqy, sqz;
	int32_t x, y, z;
	scale = 3*0x5100;
	ioffset = 0;
	
	while (1)
	{
		ioffset += 290; //700;
		
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
					color +=  (
							  (0x3200 * Sine(( x * (0x8000*0x8000 / (MAX_X * scale))) + ioffset) / 0x8000) 
							+ (0x5300 * Sine((-y * (0x8000*0x8000 / (MAX_Y * scale))) + ioffset) / 0x8000)
							+ (0x4400 * Sine((-z * (0x8000*0x8000 / (MAX_Z * scale))) + ioffset) / 0x8000)
							) / (0x13000);
					
					//colorspace offset
					color += 0x2500 + (ioffset/32);
					
					setVoxelH(x, y, z, color);
				} 
				//printf("\n");
			}
			//printf("\n");
		}
		//printf("\n");
		
		fade(10,2);
		
		if ((ioffset) >= 4*0x8000)
			break;
	}
	clearImage(black);
	fade(15, 100);
}

int plasmaDiag(int x, int y, int z, int scale, int ioff)
{
	return Sine((-x * (0x8000*0x8000 / (MAX_X * scale))) + (y * (0x8000*0x8000 / (MAX_Y * scale))) + (-z * (0x8000*0x8000 / (MAX_Z * scale))) + ioff);
}

void plasmaBall()
{
	int32_t col, scale;
	int32_t x, y, z;
	int sqx, sqy, distCalc;
	unsigned int dist;
	color colRGB;
	voxel pos;
	scale = 10*0x5000;
	dist = 0;
	ioffset = 0;
	
	while (1)
	{
		clearImage(black);
		
		ioffset += 48; //700;
		dist = (ioffset / 128) % 80;
		#define M_FILTER 15
		for(x = 0; x < MAX_X; x++)
		{
			sqx = (x-2)*(x-2)*256;
	
			for(y = 0; y < MAX_Y; y++)
			{
				sqy = (y-2)*(y-2)*256;
				
				for(z = 0; z < MAX_Z; z++)
				{
					//calculate distance to center and check against desired distance
					distCalc = isqrt(sqx + sqy + (z-2)*(z-2)*256);
					
					//reset color;
					col = 0;
					
					//diagonal scrolling sine 
					col += plasmaDiag(x, y, z, scale, ioffset);
				
					//colorspace offset
					col += 0x8000 + (ioffset/32);

					int dingsVal = abs(dist - distCalc);
					dingsVal *= 255 / M_FILTER;
					if (dingsVal > 255)
						dingsVal = 255;
					
					colRGB = HtoRGB(col);
					colRGB.r = max((int)colRGB.r - dingsVal, 0);
					colRGB.g = max((int)colRGB.g - dingsVal, 0);
					colRGB.b = max((int)colRGB.b - dingsVal, 0);
					
					pos.x = x; pos.y = y; pos.z = z;
					setVoxel(pos, colRGB);						
				
				} 
				//printf("\n");
			}
			//printf("\n");
		}
		//printf("\n");
		swapAndWait(10);
		
		if((ioffset) >= 8*0x8000)
			break;
	}
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
	unsigned char z, angle, count = 0, i = 0, angleAdd, angleAdd2;
	int32_t color, scale;
	scale = 3*0x9000;
	
	for (angleAdd2 = 0; angleAdd2 < 17; count++) {
		angleAdd = angleAdd2 < 9 ? angleAdd2 : 17 - angleAdd2; 
		//printf("%d %d %d  %d %d \n", curColor.r, curColor.g, curColor.b, index, value);
		for (angle = 0; angle < 8; angle++) {
			ioffset += 200; //700;
			for (z = 0; z < 5; z++) {
				ioffset += 8;
				color  = ioffset;
				color += 0x3000 * Sine((4-z * (0x8000*0x8000 / (MAX_Y * scale))) + 
										ioffset*4
									   ) / 0x8000;
	
				drawLineZAngle((angle+(angleAdd*z/4)) & 0x07, z, HtoRGB(color*49152/32768));		
			}
			swapAndWait(50);
			clearImage(black);
			
			if (count > 4) { 
				angleAdd2++;
				count = 0;
			}
		}
		i++;
	}
	fade(15, 100);
}
