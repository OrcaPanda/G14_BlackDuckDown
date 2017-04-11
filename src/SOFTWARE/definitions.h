/*
 * definitions.h
 *
 *  Created on: Mar 9, 2017
 *      Author: wenyi
 */

#ifndef SRC_DEFINITIONS_H_
#define SRC_DEFINITIONS_H_

#define BOX_WIDTH 80
#define BOX_HEIGHT 80

#define ROWS 16
#define COLS 16
#define SQUAREN (ROWS * COLS)

// Total of 37 notes.
#define DUCK_SQUARES 37
#define DUCK_LIMIT 30

#define NOTE1 2
#define NOTE2 3
#define NOTE3 4
#define NOTE4 5
#define NOTE5 6
#define NOTE6 7
#define NOTE7 8
#define NOTE8 9
#define NOTE9 10
#define NOTE10 11
#define NOTE11 12
#define NOTE12 13

#define LINE1 (COLS * 2)
#define LINE2 (COLS * 3)
// #define LINE3 (COLS * 2)
#define LINE4 (COLS * 5)
#define LINE5 (COLS * 6)
// #define LINE6 (COLS * 5)
#define LINE7 (COLS * 8)
#define LINE8 (COLS * 9)
// #define LINE9 (COLS * 8)
// #define LINE10 (COLS * 9)
#define LINE11 (COLS * 12)

#define BACKGROUND 0
#define DUCK 1

#define NOTEA 0
#define NOTEAS 2
#define NOTEB 4
#define NOTEC 6
#define NOTECS 8
#define NOTED 10
#define NOTEDS 12
#define NOTEE 14
#define NOTEF 16
#define NOTEFS 18
#define NOTEG 20
#define NOTEGS 22

#define OCTAVE2 0
#define OCTAVE3 32
#define OCTAVE4 64
#define OCTAVE5 96

#define LOWEST_FREQ 64
#define HIGHEST_FREQ 539

#define AMPL_THRESHOLD 200

#define FHT_BASE_ADDR XPAR_FREQCALCCORE_0_S00_AXI_BASEADDR
#define FHT_BUSY_REG 0
#define FHT_FREQ_REG 4
#define FHT_AMPL_REG 8
#define FHT_START_REG 12

#define VGA_BASE_ADDR XPAR_DISPLAY_0_S00_AXI_BASEADDR
#define QUACK_BASE_ADDR XPAR_QUACKER_0_S00_AXI_BASEADDR
#define PB_BASE_ADDR XPAR_PUSHBUTTON_0_S00_AXI_BASEADDR
#define HEX_BASE_ADDR XPAR_HEXPRINT_0_S00_AXI_BASEADDR

#define HEX_OFF 32

#endif /* SRC_DEFINITIONS_H_ */
