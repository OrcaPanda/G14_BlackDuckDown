/*
 * main.c
 *
 *  Created on: Mar 24, 2017
 *      Author: wenyi
 */


/*
 * main.c
 *
 *  Created on: Mar 9, 2017
 *      Author: wenyi
 */
// #include <time.h>

#include "display.h"
#include "xil_io.h"
#include "quacker.h"
#include "freqcalccore.h"
#include "xparameters.h"
#include "definitions.h"
#include "duck_list.h"
#include "pushbutton.h"
#include "hexprint.h"
/*
TODO:
- make duck quack if hit.
- display currently sung note to a square (or just background if not loud enough).
- implement push-button interrupt (just use AXI bus to control some bit
  monitored in software) and use interrupt to start the game and to restart the
  game after game is over.

LONGTERM TODO:
- make it impossible to sweep the note spectrum.
- implement persistent high score and show it in all screens along with hiscore
  display.
- generating ducks progressively harder by shortening the migration frequency
  possibly has to be done in hardware to ensure reliable frequency in duck
  spawning.

IF HAVE TIME:
- show duck dying image when a duck is killed.
- make the square where the note is being sung more red.
*/

typedef struct {
	unsigned char name;
	unsigned char reg_i;
} Note;

typedef struct {
	Note note;
	unsigned int ampl;
    unsigned int freq;
} NoteInfo;

// Total 11 bits per register
// bit 0: alive/dead
// bit 1-2: digit/note name or octave name/non-duck/duck
// bit 3-6: note name or octave name.
//          O5/O4/O3/B/A#/A/G#/G/F#/F/E/D#/D/C#/C
//          14/13/........................../ 1/0
// bit 7-10: digits.
//          9/8/7/6/5/4/3/2/1/0

// This function defines the conversion from frequency to
// our note notation as specified in definitions.h
Note freq_to_note(unsigned int frequency)
{
    Note ret;

	if ((frequency > 63) && (frequency <= 67))
	{
        ret.name = NOTEC+OCTAVE2;
        ret.reg_i = NOTE1 + LINE2;
	}
	else if ((frequency > 67) && (frequency <= 71))
	{
        ret.name = NOTECS+OCTAVE2;
        ret.reg_i = NOTE2 + LINE1;
	}
	else if ((frequency > 71) && (frequency <= 76))
	{
        ret.name = NOTED+OCTAVE2;
        ret.reg_i = NOTE3 + LINE2;
	}
	else if ((frequency > 76) && (frequency <= 80))
	{
        ret.name = NOTEDS+OCTAVE2;
        ret.reg_i = NOTE4 + LINE1;
	}
	else if ((frequency > 80) && (frequency <= 85))
	{
        ret.name = NOTEE+OCTAVE2;
        ret.reg_i = NOTE5 + LINE2;
	}
	else if ((frequency > 85) && (frequency <= 90))
	{
        ret.name = NOTEF+OCTAVE2;
        ret.reg_i = NOTE6 + LINE2;
	}
	else if ((frequency > 90) && (frequency <= 95))
	{
        ret.name = NOTEFS+OCTAVE2;
        ret.reg_i = NOTE7 + LINE1;
	}
	else if ((frequency > 95) && (frequency <= 101))
	{
        ret.name = NOTEG+OCTAVE2;
        ret.reg_i = NOTE8 + LINE2;
	}
	else if ((frequency > 101) && (frequency <= 107))
	{
        ret.name = NOTEGS+OCTAVE2;
        ret.reg_i = NOTE9 + LINE1;
	}
	else if ((frequency > 107) && (frequency <= 113))
	{
        ret.name = NOTEA+OCTAVE2;
        ret.reg_i = NOTE10 + LINE2;
	}
	else if ((frequency > 113) && (frequency <= 120))
	{
        ret.name = NOTEAS+OCTAVE2;
        ret.reg_i = NOTE11 + LINE1;
	}
	else if ((frequency > 120) && (frequency <= 127))
	{
        ret.name = NOTEB+OCTAVE2;
        ret.reg_i = NOTE12 + LINE2;
	}
	else if ((frequency > 127) && (frequency <= 135))
	{
        ret.name = NOTEC+OCTAVE3;
        ret.reg_i = NOTE1 + LINE5;
	}
	else if ((frequency > 135) && (frequency <= 143))
	{
        ret.name = NOTECS+OCTAVE3;
        ret.reg_i = NOTE2 + LINE4;
	}
	else if ((frequency > 143) && (frequency <= 151))
	{
        ret.name = NOTED+OCTAVE3;
        ret.reg_i = NOTE3 + LINE5;
	}
	else if ((frequency > 151) && (frequency <= 160))
	{
        ret.name = NOTEDS+OCTAVE3;
        ret.reg_i = NOTE4 + LINE4;
	}
	else if ((frequency > 160) && (frequency <= 170))
	{
        ret.name = NOTEE+OCTAVE3;
        ret.reg_i = NOTE5 + LINE5;
	}
	else if ((frequency > 170) && (frequency <= 180))
	{
        ret.name = NOTEF+OCTAVE3;
        ret.reg_i = NOTE6 + LINE5;
	}
	else if ((frequency > 180) && (frequency <= 190))
	{
        ret.name = NOTEFS+OCTAVE3;
        ret.reg_i = NOTE7 + LINE4;
	}
	else if ((frequency > 190) && (frequency <= 202))
	{
        ret.name = NOTEG+OCTAVE3;
        ret.reg_i = NOTE8 + LINE5;
	}
	else if ((frequency > 202) && (frequency <= 214))
	{
        ret.name = NOTEGS+OCTAVE3;
        ret.reg_i = NOTE9 + LINE4;
	}
	else if ((frequency > 214) && (frequency <= 226))
	{
        ret.name = NOTEA+OCTAVE3;
        ret.reg_i = NOTE10 + LINE5;
	}
	else if ((frequency > 226) && (frequency <= 240))
	{
        ret.name = NOTEAS+OCTAVE3;
        ret.reg_i = NOTE11 + LINE4;
	}
	else if ((frequency > 240) && (frequency <= 254))
	{
        ret.name = NOTEB+OCTAVE3;
        ret.reg_i = NOTE12 + LINE5;
	}
	else if ((frequency > 254) && (frequency <= 269))
	{
        ret.name = NOTEC+OCTAVE4;
        ret.reg_i = NOTE1 + LINE8;
	}
	else if ((frequency > 269) && (frequency <= 285))
	{
        ret.name = NOTECS+OCTAVE4;
        ret.reg_i = NOTE2 + LINE7;
	}
	else if ((frequency > 285) && (frequency <= 302))
	{
        ret.name = NOTED+OCTAVE4;
        ret.reg_i = NOTE3 + LINE8;
	}
	else if ((frequency > 302) && (frequency <= 320))
	{
        ret.name = NOTEDS+OCTAVE4;
        ret.reg_i = NOTE4 + LINE7;
	}
	else if ((frequency > 320) && (frequency <= 339))
	{
        ret.name = NOTEE+OCTAVE4;
        ret.reg_i = NOTE5 + LINE8;
	}
	else if ((frequency > 339) && (frequency <= 359))
	{
        ret.name = NOTEF+OCTAVE4;
        ret.reg_i = NOTE6 + LINE8;
	}
	else if ((frequency > 359) && (frequency <= 381))
	{
        ret.name = NOTEFS+OCTAVE4;
        ret.reg_i = NOTE7 + LINE7;
	}
	else if ((frequency > 381) && (frequency <= 403))
	{
        ret.name = NOTEG+OCTAVE4;
        ret.reg_i = NOTE8 + LINE8;
	}
	else if ((frequency > 403) && (frequency <= 427))
	{
        ret.name = NOTEGS+OCTAVE4;
        ret.reg_i = NOTE9 + LINE7;
	}
	else if ((frequency > 427) && (frequency <= 453))
	{
        ret.name = NOTEA+OCTAVE4;
        ret.reg_i = NOTE10 + LINE8;
	}
	else if ((frequency > 453) && (frequency <= 480))
	{
        ret.name = NOTEAS+OCTAVE4;
        ret.reg_i = NOTE11 + LINE7;
	}
	else if ((frequency > 480) && (frequency <= 508))
	{
        ret.name = NOTEB+OCTAVE4;
        ret.reg_i = NOTE12 + LINE8;
	}
	else if ((frequency > 508) && (frequency <= 539))
	{
        ret.name = NOTEC+OCTAVE5;
        ret.reg_i = NOTE1 + LINE11;
	}

	return ret;
}

/* Get the current note and amplitude from theThis function reads and returns values from the FHT core
// This function should also write to the register which holds the
// current note for the display to show
struct Tuple get_value_from_fht()
{
	unsigned int busy;
	unsigned int freq;
	unsigned int ampl;
	struct Tuple res;

	do {
		busy = FHT_mReadReg(FHT_BASE_ADDR, FHT_BUSY_REG);
	} while (busy);

	freq = FHT_mReadReg(FHT_BASE_ADDR, FHT_FREQ_REG);
	ampl = FHT_mReadReg(FHT_BASE_ADDR, FHT_AMPL_REG);

	res.note = freq_to_note(freq);
	res.ampl = ampl;

	// Re-enable the calculations
	FHT_mWriteReg(FHT_BASE_ADDR, FHT_START_REG, 1);
	return res;
}
*/

// XXX: Uncomment to enable FHT.
/* Polls the FHT for the next note to be updated.
reads and returns the current note and amplitude.
*/
NoteInfo poll_fht()
{
	unsigned int busy;
	unsigned int freq;
	unsigned int ampl;

	busy = FREQCALCCORE_mReadReg(FHT_BASE_ADDR, FHT_BUSY_REG);
	if (busy) {
		NoteInfo ret;
		ret.freq = 0;
		return ret;
	}

	freq = FREQCALCCORE_mReadReg(FHT_BASE_ADDR, FHT_FREQ_REG);
	ampl = FREQCALCCORE_mReadReg(FHT_BASE_ADDR, FHT_AMPL_REG);


	NoteInfo ret;
// 	ret.note = freq_to_note(freq);
	ret.ampl = ampl;
    ret.freq = freq;

//    xil_printf("Ampl: %d\n", ampl);
//    xil_printf("Freq: %d\n", freq);

	// Re-enable the calculations
	FREQCALCCORE_mWriteReg(FHT_BASE_ADDR, FHT_START_REG, 1);

	return ret;
}

// This function writes the specified register of the VGA controller
// with the desired value
void write_vga_note(unsigned reg_number, unsigned value)
{
	DISPLAY_mWriteReg(VGA_BASE_ADDR, reg_number * 4, value);
	return;
}

// This function returns the note information of the specified register.
// This is useful if you want to overwrite only a single bit of the register's
// value.
Note read_vga_note(unsigned reg_number)
{
	unsigned int res;

	res = DISPLAY_mReadReg(VGA_BASE_ADDR, reg_number * 4) - 1;
	return freq_to_note(res);
}

void rewrite_screen(unsigned short reg_vals[]) {
    unsigned int j = 0;
    for (j = 0; j != SQUAREN; ++j)
        write_vga_note(j, reg_vals[j]);
}

void write_hex_note(Note note){
	unsigned char number = ((note.name & (0x60)) >> 5) + 2;
	unsigned char letter = (note.name & (0x0e)) >> 1;
	HEXPRINT_mWriteReg(HEX_BASE_ADDR, 4, (letter >> 1) + 10 + 16*(letter % 2));
	HEXPRINT_mWriteReg(HEX_BASE_ADDR, 0, number);
	return;
}

void write_hex_score(unsigned int score){
	unsigned int hundreds_score = (score / 100) % 10;
	unsigned int tens_score = (score / 10) % 10;
	unsigned int ones_score = score % 10;
	HEXPRINT_mWriteReg(HEX_BASE_ADDR, 16, ones_score);
	if(tens_score != 0){
		HEXPRINT_mWriteReg(HEX_BASE_ADDR, 20, tens_score);
	}
	if(hundreds_score != 0){
		HEXPRINT_mWriteReg(HEX_BASE_ADDR, 24, hundreds_score);
	}
	return;
}

int main() {

	xil_printf("Game Starting...\n");

    DuckList duck_list;
    init_duck_list(&duck_list);

    // display start screen.
    rewrite_screen(duck_display);

    int i;
    // Clear push button states
    for(i = 0; i <= 16; i = i + 4){
    	PUSHBUTTON_mWriteReg(PB_BASE_ADDR, i, 0);
    }

    // Clear hex displays
    for(i = 0; i <= 28; i = i + 4){
    	HEXPRINT_mWriteReg(HEX_BASE_ADDR, i, HEX_OFF);
    }

    const int base_period = 1000000;
    unsigned int spawn_period = base_period * 2;
    const unsigned int spawn_period_dec = base_period / 25;
    unsigned int spawn_counter = 0;
    unsigned int score = 0;

    NoteInfo note_info;
    unsigned char reg_i;
    unsigned short reg_val;
    RegInfo spawned_reg;

    const int freqN = 32, amplN = 32;
    unsigned int prev_amplitudes[amplN];
    unsigned int prev_frequencies[freqN];
    unsigned int tot_ampl = 0, tot_freq = 25600;

    int j;
    for (j = 0; j != freqN; ++j) {
        prev_frequencies[j] = tot_freq / freqN;
    }
    j = 0; // reset last counter.

    int k;
    for (k = 0; k != amplN; ++k) {
		prev_amplitudes[k] = tot_ampl / amplN;
	}
    k = 0;

    // Pause at the start screen for a bit.
//    while (++spawn_counter != spawn_period * 2)
//        ;
//    spawn_counter = 0;
    // Wait for the push button before starting the game
    while(!PUSHBUTTON_mReadReg(PB_BASE_ADDR, 16));

    // Initialize the duck values.
    rewrite_screen(duck_list.regs);

    xil_printf("\nStart button pressed\n");

    while (1) {
        // XXX: Uncomment to enable FHT.
        // Continue polling until seeing a note above the threshold amplitude.
        note_info = poll_fht();

        if (note_info.ampl > AMPL_THRESHOLD) {
//			k = (k + 1) % amplN;
//			tot_ampl -= prev_amplitudes[k];
//			prev_amplitudes[k] = note_info.ampl;
//			tot_ampl += prev_amplitudes[k];

			j = (j + 1) % freqN;
			tot_freq -= prev_frequencies[j];
			prev_frequencies[j] = note_info.freq;
			tot_freq += prev_frequencies[j];

			//int avg_ampl = tot_ampl / amplN;


			//xil_printf("avg ampl = %d\n", avg_ampl);
			//xil_printf("avg freq = %d\n", avg_freq);



			if (j == (freqN - 1)) { // If the buffer is completely full

				int avg_freq = tot_freq / freqN;
				write_vga_note(255, avg_freq);

				// Read the note and kill the duck if there is one there.
				Note note = freq_to_note(avg_freq);
				reg_i = note.reg_i;
				write_hex_note(note);
				//xil_printf("avg freq = %d, freq = %d\n", avg_freq, note_info.freq);

				if (kill_duck(&duck_list, reg_i)) {
					// xil_printf("\nKILL\n");
					// Make sound
					QUACKER_mWriteReg(QUACK_BASE_ADDR, 4, 1);

					// If a duck is killed, write the update to the register holder.
					write_vga_note(reg_i, duck_list.regs[reg_i]);

					// Update score.
					++score;
					write_vga_note(236, ((score / 100) % 10) * 256 + 6);
					write_vga_note(237, ((score / 10) % 10) * 256 + 6);
					write_vga_note(238, (score % 10) * 256 + 6);
					write_hex_score(score);
				}
			}
        }

        // After some time, spawn a new duck.

        if (++spawn_counter == spawn_period) {
            spawn_counter = 0;
            spawn_period = spawn_period > spawn_period_dec ? spawn_period - spawn_period_dec : spawn_period;
            spawned_reg = spawn_random_duck(&duck_list);

            write_vga_note(spawned_reg.reg_i, spawned_reg.reg_val);
        }

        //xil_printf("emptysize = %d\n", duck_list.emptySize);
        //xil_printf(" ");

        if (DUCK_SQUARES - duck_list.emptySize >= DUCK_LIMIT) {
            // Carry over score from the game.
            over_screen[236] = ((score / 100) % 10) * 256 + 6;
            over_screen[237] = ((score / 10) % 10) * 256 + 6;
            over_screen[238] = (score % 10) * 256 + 6;
            // Game Over.
            rewrite_screen(over_screen);
            HEXPRINT_mWriteReg(HEX_BASE_ADDR, 0, HEX_OFF);
            HEXPRINT_mWriteReg(HEX_BASE_ADDR, 4, HEX_OFF);
            while(1);
        }
    }
}

