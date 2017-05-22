/**
 ******************************************************************************
 * File Name          : u_tty.h
 * Description        : This file contains
 ******************************************************************************
 *
 * CREATED: 18 apr 2017 Hobbist E
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef U_TTY_H_
#define U_TTY_H_
#endif /* U_TTY_H_ */

/* Includes -------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stdlib.h"
#include "string.h"

/* Type declarations --------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* Function prototypes ----------------------------------------------*/
void txtest(void);
void testSGR(int min, int max);
void tx(char *txt);
void txCPOS(int r, int c);
void txCMOV(int positions, char *direction);
void txCOLOR(int fgnd, int bgnd);
void txATTR(int bold, int italic, int negative);
void txANSI(uint8_t charcode);
void txUTF8(uint32_t utfpoint);
void ts(uint32_t style, char *tst);



/*General useful ASCII codes*/

#define T_BEL "\a"				/*Terminal bell*/
#define T_BS "\b"				/*Backspace*/
#define T_HT "\t"				/*Horizontal TAB*/
#define T_LF "\n"				/*Next string without CR*/
#define T_VT "\v"				/*Vertical TAB*/
#define T_FF "\f"				/*Formfeed (also: New page NP)*/
#define T_CR "\r"				/*Carriage return*/
#define T_CRLF "\r\n"				/*New line - next string + CR*/

/*Cursor handling*/

#define T_HOME "\033[H"			/*Move cursor to home position (0-0)*/
#define T_CUP1 "\033[r;cH"		/*Move cursor to  position (r-c) from 1 (ANSI)*/
#define T_CIVIS "\033[?25l"		/*make cursor invisible (VT100)*/
#define T_CVVIS "\033[?25h"		/*make cursor visible (VT100)*/

/*Erasing text*/
#define T_CLS "\033[2J"			/*Clear the screen.*/
#define T_DEL_R "\033[K"		/* or [0K - Clear line from current cursor position to end of line*/
#define T_DEL_L "\033[1K"		/*Clear line from beginning to current cursor position*/
#define T_ERASE "\033[2K"		/*Clear whole line (cursor position unchanged)*/

/*General text attributes*/
#define NOAT 0		/*Reset all attributes*/
#define BLD 1		/*Set "bold" attribute*/
#define NEG 7		/*Set "reverse" attribute*/
#define ITA 4		/*Set "italic/underline" attribute, terminal dependent*/

/*Color attributes*/
#define NER 0		/*BLACK (NERO)*/
#define RED 1		/*RED*/
#define GRN 2		/*GREEN*/
#define YEL 3		/*YELLOW*/
#define BLU 4		/*BLUE*/
#define MAG 5		/*MAGENTA*/
#define CYN 6		/*CYAN*/
#define WHT 7		/*WHITE*/
#define DEF 9		/*Reset default color*/

/*Styles*/
#define S01 (0x01942) /*LF=0,fg=RED,bg=DEF,font=BLD-not italic-not negative=4,LF=2*/
#define S02 (0x12941) /*LF=1,fg=GREEN,bg=DEF,font=BLD-not italic-not negative=4,LF=1*/
#define TESTBORDER (0x13741) /*LF=1,fg=YEL,bg=GRAY,font=BLD-not italic-not negative=4,LF=2*/
#define WELCOME_STYLE (0x11951) /*LF=1,fg=RED,bg=YEL,font=BLD-not italic-not negative=4,LF=1*/
#define MENU_STYLE (0x13041) /*LF=1,fg=YEL,bg=DARK,font=BLD-not italic-not negative=4,LF=2*/


/*Combined commands*/
/*Clear the screen, reset attributes and cursor*/
#define T_CLSA "\033[2J\033[H\033[0m\033[?25h"
#define RATTR "\033[0m" /*reset attributes only*/

/*Predefined messages*/
#define PROMPT "\r\n"
#define WELCOME_MESSAGE "Welcome to STM32 project!"
#define MAIN_MENU "\r\nMENU:\r\n1-3.Switch LED 1 - 3\r\n4.Show User Button status\r\n5.Reset terminal\r\n"
#define WELCOME_UART "Welcome to UART tect!"
#define START_LINE1 "START of the txtest() function "
#define START_LINE2 "This green line (not bold) is START of the test"
#define END_LINE    "END  of the  txtest() function "

/*ASCII art*/
/*STM32F746ZG*/
#define ASCART01 "  _____ _______ __  __ ____ ___  ______ ______ _  _     __ ___________ \033[E / ____|__   __|  \\/  |___ \\__ \\|  ____|____  | || |   / /|___  / ____|\033[E| (___    | |  | \\  / | __) | ) | |__      / /| || |_ / /_   / / |  __ \033[E \\___ \\   | |  | |\\/| ||__ < / /|  __|    / / |__   _| '_ \\ / /| | |_ |\033[E ____) |  | |  | |  | |___) / /_| |      / /     | | | (_) / /_| |__| |\033[E|_____/   |_|  |_|  |_|____/____|_|     /_/      |_|  \\___/_____\\_____|\033[E                                                                       \033[E                                                                       \033[E\n"
#define ASCART01rn "  _____ _______ __  __ ____ ___  ______ ______ _  _     __ ___________ \r\n / ____|__   __|  \\/  |___ \\__ \\|  ____|____  | || |   / /|___  / ____|\r\n| (___    | |  | \\  / | __) | ) | |__      / /| || |_ / /_   / / |  __ \r\n \\___ \\   | |  | |\\/| ||__ < / /|  __|    / / |__   _| '_ \\ / /| | |_ |\r\n ____) |  | |  | |  | |___) / /_| |      / /     | | | (_) / /_| |__| |\r\n|_____/   |_|  |_|  |_|____/____|_|     /_/      |_|  \\___/_____\\_____|\r\r\n\n"

/*Artist and bear*/
#define ASCART02 "Art by Joris Bellenger\033[E            _\033[E           H||\033[E           H||\033[E __________H||___________\033[E[|.......................|\033[E||.........## --.#.......|\033[E||.........   #  # ......|            @@@@\033[E||.........     *  ......|          @@@@@@@\033[E||........     -^........|   ,      - @@@@\033[E||.....##\\        .......|   |     \'_ @@@\033[E||....#####     /###.....|   |     __\\@ \\@\033[E||....########\\ \\((#.....|  _\\\\  (/ ) @\\_/)____\033[E||..####,   ))/ ##.......|   |(__/ /     /|\% #/\033[E||..#####      \'####.....|    \\___/ ----/_|-*/\033[E||..#####\\____/#####.....|       ,:   \'(\033[E||...######..######......|       |:     \\\033[E||.....\"\"\"\"  \"\"\"\"...b\'ger|       |:      )\033[E[|_______________________|       |:      |\033[E       H||_______H||             |_____,_|\033[E       H||________\\|              |   / (\033[E       H||       H||              |  /\\  )\033[E       H||       H||              (  \\| /\033[E      _H||_______H||__            |  /\'=.\033[E    H|________________|           \'=>/  \\\033[E                                 /  \\ /|/\033[E                               ,___/|\n"
#define ASCART02rn "Art by Joris Bellenger\r\n            _\r\n           H||\r\n           H||\r\n __________H||___________\r\n[|.......................|\r\n||.........## --.#.......|\r\n||.........   #  # ......|            @@@@\r\n||.........     *  ......|          @@@@@@@\r\n||........     -^........|   ,      - @@@@\r\n||.....##\\        .......|   |     \'_ @@@\r\n||....#####     /###.....|   |     __\\@ \\@\r\n||....########\\ \\((#.....|  _\\\\  (/ ) @\\_/)____\r\n||..####,   ))/ ##.......|   |(__/ /     /|\% #/\r\n||..#####      \'####.....|    \\___/ ----/_|-*/\r\n||..#####\\____/#####.....|       ,:   \'(\r\n||...######..######......|       |:     \\\r\n||.....\"\"\"\"  \"\"\"\"...b\'ger|       |:      )\r\n[|_______________________|       |:      |\r\n       H||_______H||             |_____,_|\r\n       H||________\\|              |   / (\r\n       H||       H||              |  /\\  )\r\n       H||       H||              (  \\| /\r\n      _H||_______H||__            |  /\'=.\r\n    H|________________|           \'=>/  \\\r\n                                 /  \\ /|/\r\n                               ,___/|\r\n"

