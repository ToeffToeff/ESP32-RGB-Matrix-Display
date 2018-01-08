/*
Author:	Vitali Gottselig
https://github.com/

Forked by ToeffToeff
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Adafruit_GFX.h"
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef short	int16_t;
typedef unsigned short uint16_t;

//G1	R1 |
//GND	B1 |
//G2	R2 |
//GND	B2  |
//B		A   |
//D		C  |
//LAT	CLK|
//GND	OE |
//OE connected with LAT

#pragma region Pintests
//int tespin1 = 00; OK
//int tespin3 = 01; NOT OK!!!! on (TX0)
//int tespin2 = 02; OK
//int tespin1 = 03; OK (RX0)
//int tespin3 = 04; OK
//int tespin3 = 05; OK
//int tespin3 = 6; ???
//int tespin3 = 7; freeze
//int tespin2 = 8; freeze
//int tespin1 = 12; OK	(JTAG Pin on ESP23 WROVER Kit) 
//int tespin1 = 13; OK	(JTAG Pin on ESP23 WROVER Kit)
//int tespin2 = 14; OK	(JTAG Pin on ESP23 WROVER Kit)
//int tespin3 = 15; OK	(JTAG Pin on ESP23 WROVER Kit)
//int tespin3 = 16; OK	(not OK on ESP32 WROVER Kit) 
//int tespin3 = 17; OK	(not OK on ESP32 WROVER Kit)  
//int tespin3 = 18; OK
//int tespin2 = 21; OK
//int tespin3 = 19; OK
//int tespin2 = 22; OK
//int tespin1 = 23; OK
//int tespin3 = 25; OK
//int tespin2 = 26; OK
//int tespin3 = 27; OK
//int tespin3 = 32; not OK on ESP32 WROVER Kit (unknown on standard ESP32)
//int tespin3 = 32; not OK on ESP32 WROVER Kit (unknown on standard ESP32)

#pragma endregion

struct color
{
public:
	uint8 r;
	uint8 g;
	uint8 b;
};

#define COLUMNS 64
#define ROWS 32
#define layers 256
#define colorDepthPerChannel 4  //(2^colorDepthPerChannel)^3  Colors. 4096@4 bit.  262144 @ 6 bit. 
#define frameBufferEnabled

//0b0000RRRRGGGGBBBB
#define rmask 0b0000000000001111
#define gmask 0b0000000011110000
#define bmask 0b0000111100000000

class Colors
{
public:
	static const int16_t BLACK = 0;
	static const int16_t RED = rmask;
	static const int16_t GREEN = gmask;
	static const int16_t BLUE = bmask;
	static const int16_t WHITE = rmask | gmask | bmask;
	static const int16_t CYAN = gmask | bmask;
	static const int16_t YELLOW = rmask | gmask;
	static const int16_t MAGENTA = rmask | bmask;
};



class ESP32RGBmatrixPanel : public Adafruit_GFX 
{
public:
	/*variable pin setup*/
	ESP32RGBmatrixPanel(uint8 oe, uint8 clk, uint8 lat, uint8 r1, uint8 g1, uint8 b1, uint8 r2, uint8 g2, uint8 b2, uint8 a, uint8 b, uint8 c, uint8 d);
	
	/*
	uint8 OE = 23;
	uint8 CLK = 22;
	uint8 LAT = 03;
	uint8 CH_A = 21;
	uint8 CH_B = 19;
	uint8 CH_C = 18;
	uint8 CH_D = 5;
	uint8 R1 = 17;
	uint8 G1 = 16;
	uint8 BL1 = 4;
	uint8 R2 = 0;
	uint8 G2 = 2;
	uint8 BL2 = 15;
	*/

	ESP32RGBmatrixPanel();
	void drawPixel(int16_t x, int16_t y, uint16_t c);
	void drawPixel(int16_t x, int16_t y, uint8 r, uint8 g, uint8 b);
	

	/* reset all Pixels to black */
	void black();

	/*  call every 1-3 us! */
	void update();

	/*  from 0 = off to 15 = max */
	void setBrightness(byte brightness);

	/*  returns Color for call Adafruit_GFS methods */
	static int16_t AdafruitColor(uint8 r, uint8 g, uint8 b);

	void drawBitmap(String* bytes);

	void frameComplete();

	uint8_t debugVal[10];

private:
	volatile byte loopNr = 0;
	volatile byte loopNrOn = 0;
	void initGPIO();
	void drawRow();
	void on();
	void off();
	void swapWriteFrames();
	void swapReadFrames();
	bool readyToSwapFrames = false;
	uint8 OE = 23;
	uint8 CLK = 22;
	uint8 LAT = 03;

	uint8 CH_A = 21;
	uint8 CH_B = 19;
	uint8 CH_C = 18;
	uint8 CH_D = 5;

	uint8 R1 = 17;
	uint8 G1 = 16;
	uint8 BL1 = 4;

	uint8 R2 = 0;
	uint8 G2 = 2;
	uint8 BL2 = 15;

	struct color pixels1[ROWS][COLUMNS];
	struct color pixels2[ROWS][COLUMNS];
	struct color pixels3[ROWS][COLUMNS];
	struct color (*writeFrame)[ROWS][COLUMNS] = &pixels1;
	struct color (*readFrame)[ROWS][COLUMNS] = &pixels2;
	struct color (*bufferFrame)[ROWS][COLUMNS] = &pixels3;

	uint32_t gpio;

	uint8 layerStep = 256 / (1 << colorDepthPerChannel);
	uint8 layer = layerStep - 1;
	uint8 row = 0;
	uint8 column = 0;
};

