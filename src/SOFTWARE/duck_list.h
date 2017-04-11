// #include <stdlib.h>
//#include <stdbool.h>

/*
Dual data structure to allow for O(1) random fill of empty nodes, and
O(1) selective resetting of non-empty nodes.
*/

typedef struct {
    unsigned char reg_i;
    unsigned short reg_val;
} RegInfo;

// Total 12 bits per register
// bit 0: alive/dead
// bit 1-2: digit/note name or octave name/background/duck
// bit 3-6: note name or octave name.
//          O5/O4/O3/B /A#/A/G#/G/F#/F/E/D#/D/C#/C
//          14/13/12/11/...................../ 1/0
// bit 8-11: digits.
//          9/8/7/6/5/4/3/2/1/0

// Initialization values for the squares.
const unsigned short init[SQUAREN] = {
    // Interestingly, the display here corresponds with the screen.
    0x2, 0x2, 0x4, 0xc, 0x14, 0x1c, 0x24, 0x2c, 0x34, 0x3c, 0x44, 0x4c, 0x54, 0x5c, 0x2, 0x2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0x02, 2, 2, 0, 2, 0, 2, 2, 0, 2, 0, 2, 0, 2, 2, 2,
    0x64, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0x02, 2, 2, 0, 2, 0, 2, 2, 0, 2, 0, 2, 0, 2, 2, 2,
    0x6c, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0x02, 2, 2, 0, 2, 0, 2, 2, 0, 2, 0, 2, 0, 2, 2, 2,
    0x74, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0x02, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0x7c, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 6, 6, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

// Initialization values for the squares.
unsigned short over_screen[SQUAREN] = {
    // Interestingly, the display here corresponds with the screen.
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 1, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 1, 2,
    2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 1, 2, 1, 2, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 6, 6, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

// Initialization values for the squares.
unsigned short duck_display[SQUAREN] = {
    // Interestingly, the display here corresponds with the screen.
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 1, 1, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 1, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 1, 2,
    2, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 6, 6, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

typedef struct {
    unsigned short regs[SQUAREN];
    unsigned short emptyVec[SQUAREN];
    unsigned int emptySize;
} DuckList;

// Initializes a duck list.
void init_duck_list(DuckList* list) {
    list->emptySize = 0;

    unsigned int i;
    for (i = 0; i != SQUAREN; ++i) {
        // NOTE: 0s are dead ducks.
        if ((list->regs[i] = init[i]) == 0) {
            list->emptyVec[list->emptySize++] = i;
        }
    }
}

/* Set duck to killed and put this in the killed vector.
Return value denotes whether a duck was really killed or was this an empty
square.*/
int kill_duck(DuckList* list, unsigned char duck_i) {
    // Do not kill an already-dead duck.
    if ((list->regs[duck_i]) != 1) {
        if ((list->regs[duck_i]) != 0) {
            //xil_printf("kill_duck != 1 != 0\n");
        }
        return 0;
    }
    // NOTE: 0s are ducks.

    // Zero the alive bit and record dead duck in the emptyVec.
    list->regs[duck_i] = 0;
    list->emptyVec[list->emptySize++] = duck_i;

    return 1;
}

unsigned char my_rand()
{
    //static unsigned int state = 777;

    static unsigned int i = 0;
    static const unsigned char randoms[256] = {43, 189, 2, 204, 148, 174, 235, 187, 171, 224, 247, 201, 228, 18, 97, 74, 73, 136, 151, 198, 6, 13, 5, 127, 214, 196, 75, 66, 230, 108, 230, 230, 86, 135, 186, 22, 68, 228, 168, 122, 44, 43, 82, 195, 236, 115, 116, 245, 84, 120, 17, 105, 235, 185, 244, 92, 95, 198, 41, 6, 126, 50, 244, 181, 50, 236, 113, 101, 208, 26, 92, 68, 132, 120, 145, 82, 166, 89, 243, 76, 224, 112, 200, 226, 39, 118, 153, 165, 27, 51, 174, 215, 167, 68, 81, 136, 7, 103, 65, 9, 252, 144, 206, 169, 127, 68, 193, 79, 56, 46, 100, 74, 179, 130, 124, 47, 203, 222, 142, 76, 28, 173, 196, 13, 222, 119, 134, 211, 41, 3, 210, 166, 141, 196, 216, 3, 47, 50, 133, 135, 77, 84, 195, 92, 249, 217, 33, 218, 91, 14, 224, 191, 73, 61, 12, 189, 253, 85, 108, 162, 178, 239, 33, 251, 81, 180, 184, 59, 234, 220, 253, 146, 17, 149, 221, 211, 198, 138, 59, 102, 192, 63, 51, 170, 69, 163, 184, 124, 57, 186, 229, 189, 139, 63, 213, 6, 203, 92, 175, 216, 123, 27, 86, 15, 165, 164, 199, 10, 145, 113, 28, 184, 68, 111, 251, 18, 96, 99, 63, 9, 171, 79, 50, 214, 233, 118, 231, 244, 140, 81, 64, 147, 210, 12, 139, 26, 238, 180, 196, 126, 95, 117, 114, 202, 69, 207, 67, 234, 223, 216, 139, 1, 253, 190, 104, 137};

    //state = state * 1664525 + 1013904223;
    //return state >> 24;
    //return 24;

    i = (i + 1) % 256;
    return randoms[i];
}

/* Gives the ID of a random duck that is already dead. */
// XXX: Does not check that there are no ducks remaining to be spawned. YOU
// need to make sure that this is the case.
RegInfo spawn_random_duck(DuckList* list) {
    // TODO: remove this do nothing if block after limit has been imposed in main function.
    if (list->emptySize == 0) {
        RegInfo ret;
        ret.reg_val = list->regs[0];
        ret.reg_i = 0;
        return ret;
    }

    RegInfo ret;

    // First, get a random duck that's already dead.
    unsigned char rand_i = my_rand() % list->emptySize;
    // This is the duck ID to be returned.
    ret.reg_i = list->emptyVec[rand_i];

    // Now record the duck as being alive in the state vector.
    list->regs[list->emptyVec[rand_i]] = 1;
    // Swap the last duck to this position, so that the vector can still be a
    // contiguous array of dead ducks.
    list->emptyVec[rand_i] = list->emptyVec[--list->emptySize];

    ret.reg_val = 1;

    return ret;
}

