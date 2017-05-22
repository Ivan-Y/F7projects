/**
 ******************************************************************************
 * File Name          : u_tty.c
 * Description        : This file contains
 ******************************************************************************
 *
 * CREATED: 18 apr 2017 Hobbist E
 *
 ******************************************************************************
 */

/* Includes ----------------------------------------------------------------------*/
#include "u_tty.h"

/* Type declarations -----------------------------------------------------------*/

/* Private variables ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Procedures ------------------------------------------------------------------*/

/*Create positive mask for number of last bits)*/
#define LMASK(numbits) ((1<<(numbits))-1)
/*Create positive mask for bits from low to high (both included)*/
#define PMASK(low,high) (((1<<(high-low+1))-1) << low)
/*Create negative mask for bits from low to high (both included)*/
#define NMASK(low,high) (~(PMASK(low,high)))
/*Check bits from low to high (both included)*/
#define BITS(x,low,high) (((x) >> (low)) & (LMASK((high)-(low)+1)))
/*Clear bits from low to high (both included)*/
#define CLITS(x,low,high) ((x) & (NMASK((low),(high))))
/*Set bits from low to high (both included)*/
#define SETS(x,low,high) ((x) | (PMASK((low),(high))))
/*Flip bits from low to high (both included)*/
#define FLIPS(x,low,high) ((x) ^ (PMASK((low),(high))))
#define BIT(x,m)  (((x)>>(m)) & 1)  /*Slower, but (x) value remain unchanged*/
#define BITF(x,m)  (((x)>>=(m)) & 1)  /*(x)>>=(m) faster then (x)>>(m), but change (x) value*/
/*Set bits of number (x) from (low) to (high), both included, to equal of the bits of (y) where
 * lowest bit of y will define value in the (low) position of (x)
 * Example: 1101101011111 (Binary) = BITSET(0b1111111111111, 5,10,0b11010);*/
#define BITWORD(x,low,high,y) ((CLITS((x),(low),(high))) | (((y)<<(low)) & (PMASK((low),(high)))))



void testSGR(int min, int max)
    {
    tx("\r\n");
    char msg[25];
    for (int attr = min; attr < max + 1; ++attr)
	{
	tx("\033[0m");/*Reset all attributes*/
	txCOLOR(attr, 9);
	snprintf(msg, 26, " TEST attribute # %d", attr);
	tx(msg);
	txUTF8(0x2591);
	txUTF8(0x2588);
	tx("\033[0m\r\n reset\r\n");/*Reset all attributes, newline*/
	}
     }

void txtest(void)
    {

     ts(TESTBORDER,START_LINE1);
    /*BEGIN TEST***********************/

    /*ts(0x25942,ASCART02rn)*/;/*artist*/
    testSGR(0, 9);

    /*txCPOS(5, 15);
     txANSI(0x26);
     txANSI(0x31);
     txANSI(0x5F);*/

    /*END TEST*************************/
    ts(TESTBORDER,END_LINE);
     }

/*String Output*/
void tx(char *txt)
    {
    HAL_UART_Transmit(&huart3, (uint8_t*) txt, strlen(txt),
    HAL_MAX_DELAY);
    /*TODO Repeat when error, wait if busy, ets ?*/
    }

/**
 * @brief Transmit to UART message with color and font attributes and reset attributes.
 * @param frontcolor: foreground terminal color (0-9)
 * @param backcolor: background terminal color (0-9)
 * @param bold: non-zero to make text bold
 * @param italic: non-zero to make text italic or underline , terminal dependent
 * @param negative: non-zero to exchange foreground <=> background colors
 * @param txt: text message
 * @param linefeedafter: number of empty line(s) to inserted after this text
 * @retval None
 *
 * COLOR:0-Black,1-Red,2-Green,3-Yellow,4-Blue,5-Magenta,6-Cyan,7-White,9-reset default
 * ATTRIBUTE:  bold(1/0), italic(1/0), negative(1/0)
 */
void ts(uint32_t style, char *tst)
    {
    tx(RATTR); /*RESET all attributes*/
    for (uint8_t i = 0; i < BITS((style), 16, 19); ++i)
	{
	tx("\r\n");
	}
    /* txCMOV(BITS(style, 16, 19), "N");*//*linefeedbefore*/
    txCOLOR(BITS((style), 12, 15), BITS((style), 8, 11));/*frontcolor,backcolor*/
    txATTR(BIT((style), 6), BIT((style), 5), BIT((style), 4));/*bold,italic,negative*/
   tx(tst);
    /* txCMOV(BITS(style, 0, 3), "N");*//*linefeedafter*/
    tx(RATTR); /*RESET all attributes*/
    for (uint8_t m = 0; m < BITS((style), 0, 3); ++m)
	{
	tx("\r\n");
	}
     }

/*Cursor Position, (r)ow, (c)olumn*/
void txCPOS(int row, int column)
    {
    char msg[8];
    snprintf(msg, 9, "\033[%d;%dH", row, column);
    tx(msg);
    }

/*Cursor movement*/
void txCMOV(int positions, char *direction)
    {
    switch (*direction)
	{
    case 0x55:/*"U - n positions up"*/
	direction = "A";
	break;
    case 0x44:/*"D - n positions down"*/
	direction = "B";
	break;
    case 0x46:/*"F - n positions forward"*/
	direction = "C";
	break;
    case 0x42:/*"B - n positions backward"*/
	direction = "D";
	break;
    case 0x4E:/*"N - to beginning of (current + n) next line down"*/
	direction = "E";
	break;
    case 0x50:/*"P - to beginning of (current - n) previous  line up"*/
	direction = "F";
	break;
    case 0x43:/*"C - to column number in current line"*/
	direction = "G";
	break;
    default:
	direction = "S";
	break;
	}

    if (*direction == 0x53) /*"S"*/
	return;
    char msg[8];
    snprintf(msg, 9, "\033[%d%s", positions, direction);
    tx(msg);
    }

/*CoLoR (f)oreground[0-7], (b)ackground[0-7]*/
/*0-Black,1-Red,2-Green,3-Yellow,4-Blue,5-Magenta,6-Cyan,7-White,9-reset default*/
void txCOLOR(int fgnd, int bgnd)
    {
    int fg, bg, efg, ebg;
    switch (fgnd)
	{
    case 0:
	fg = 7;
	efg = 237;
	break;
    case 1:
	fg = 1;
	efg = 160;
	break;
    case 2:
	fg = 2;
	efg = 40;
	break;
    case 3:
	fg = 3;
	efg = 184;
	break;
    case 4:
	fg = 4;
	efg = 26;
	break;
    case 5:
	fg = 5;
	efg = 164;
	break;
    case 6:
	fg = 6;
	efg = 44;
	break;
    case 7:
	fg = 7;
	efg = 242;
	break;
    default:
	fg = 9;
	efg = 250;
	break;
	}
    switch (bgnd)
	{
    case 0:
	bg = 7;
	ebg = 232;
	break;
    case 1:
	bg = 1;
	ebg = 124;
	break;
    case 2:
	bg = 2;
	ebg = 28;
	break;
    case 3:
	bg = 3;
	ebg = 136;
	break;
    case 4:
	bg = 4;
	ebg = 26;
	break;
    case 5:
	bg = 5;
	ebg = 90;
	break;
    case 6:
	bg = 6;
	ebg = 66;
	break;
    case 7:
	bg = 7;
	ebg = 239;
	break;
    default:
	bg = 9;
	ebg = 232;
	break;
	}
    char msg[31];
    snprintf(msg, 32, "\033[3%d;38;05;%d;4%d;48;05;%dm", fg, efg, bg, ebg);
    tx(msg);
    }

/* ATTRIBUTE:  0-reset all attributes, 1-bold, 4-italic(underline), 7-negative,
 * 22-bold off, 24-italic(underline) off, 27-negative off */
void txATTR(int bold, int italic, int negative)
    {
    tx("\033[22;24;27m"); /*RESET all font attributes*/
    if (bold)
	tx("\033[1m");
    if (italic)
	tx("\033[4m");
    if (negative)
	tx("\033[7m");
    }

/*Transmit to UART symbol by ANSI charcode (32-255)*/
/*Windows - cp1251, DOS - cp866 (cyrillic2)*/
void txANSI(uint8_t charcode)
    {
    char ansi[2];
    charcode = charcode < 0x20 ? 0x3F : charcode;
    ansi[0] = charcode, ansi[1] = 0;
    tx(ansi);
    }

/**
 * @brief Transmit to UART symbol by UTF8 code point
 * where the assert_param error has occurred.
 * @param utfpoint: UTF8 code point like 0x0041 for U+0041(A)
 * @retval None
 */
void txUTF8(uint32_t utfpoint)
    {
    char utf8[5];

    utfpoint =
	    ((utfpoint < 32 && utfpoint != 10 && utfpoint != 13)
		    || (utfpoint > 0x007E && utfpoint < 0x00A1)) ?
		    0x3F : utfpoint;

    if (utfpoint < 0x80)/*00000000-0000007F, 1 byte*/
	utf8[0] = utfpoint, utf8[1] = 0;

    else if (utfpoint < 0x800)/*00000080-000007FF, 2 bytes*/
	utf8[0] = 192 + utfpoint / 64, utf8[1] = 128 + utfpoint % 64, utf8[2] =
		0;

    /*else if (c-0xd800u<0x800) goto error;*/

    else if (utfpoint < 0x10000)/*00000800-0000FFFF, 3 bytes*/
	utf8[0] = 224 + utfpoint / 4096, utf8[1] = 128 + utfpoint / 64 % 64, utf8[2] =
		128 + utfpoint % 64, utf8[3] = 0;

    /*00010000-001FFFFF, 4 bytes*/
    else if (utfpoint < 0x110000)
	utf8[0] = 240 + utfpoint / 262144, utf8[1] = 128 + utfpoint / 4096 % 64, utf8[2] =
		128 + utfpoint / 64 % 64, utf8[3] = 128 + utfpoint % 64, utf8[4] =
		0;

    tx(utf8);
    }


