// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// 11/15/2021
#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>
//#include "Images.h"

// Period =  80000000/64/Freq=1250000/Freq
#define C1   597   // 2093 Hz
#define B1   633   // 1975.5 Hz
#define BF1  670   // 1864.7 Hz
#define A1   710   // 1760 Hz
#define AF1  752   // 1661.2 Hz
#define G1   797   // 1568 Hz
#define GF1  845   // 1480 Hz
#define F1   895   // 1396.9 Hz
#define E1   948   // 1318.5 Hz
#define EF1  1004   // 1244.5 Hz
#define D1   1064   // 1174.7 Hz
#define DF1  1127   // 1108.7 Hz
#define C    1194   // 1046.5 Hz
#define B    1265   // 987.8 Hz
#define BF   1341   // 932.3 Hz
#define A    1420   // 880 Hz
#define AF   1505   // 830.6 Hz
#define G    1594   // 784 Hz
#define GF  1689   // 740 Hz
#define F   1790   // 698.5 Hz
#define E   1896   // 659.3 Hz
#define EF  2009   // 622.3 Hz
#define D   2128   // 587.3 Hz
#define DF  2255   // 554.4 Hz
#define C0  2389   // 523.3 Hz
#define B0  2531   // 493.9 Hz
#define BF0 2681   // 466.2 Hz
#define A0  2841   // 440 Hz
#define AF0 3010   // 415.3 Hz
#define G0  3189   // 392 Hz
#define GF0 3378   // 370 Hz
#define F0  3579   // 349.2 Hz
#define E0  3792   // 329.6 Hz
#define EF0 4018   // 311.1 Hz
#define D0  4257   // 293.7 Hz
#define DF0 4510   // 277.2 Hz
#define C7  4778   // 261.6 Hz middle c
#define B7  5062   // 246.9 Hz
#define BF7 5363   // 233.1 Hz
#define A7  5682   // 220 Hz
#define AF7 6020   // 207.7 Hz
#define G7  6378   // 196 Hz
#define GF7 6757   // 185 Hz
#define F7  7159   // 174.6 Hz
#define E7  7584   // 164.8 Hz
#define EF7 8035   // 155.6 Hz
#define D7  8513   // 146.8 Hz
#define DF7 9019   // 138.6 Hz
#define C6  9556   // 130.8 Hz
#define rest 0xFFFFFFFF // period too large to create sound


#define bpm80 2500000 //interrupt period for 80 bpm -- 24 interrupts per beat -- formula in lab 10 onenote
#define bpm110 1818182
#define bpm158 1265823
#define bpm100 2000000

#define whole 96
#define half 48
#define dottedhalf 72
#define quarter 24		//24 pixels in quarter note
#define eighth 12
#define sixteenth 6
#define triplet 8
#define twotriplet 16
#define quartertriplet 32
#define wholeeighth 108
#define dottedeighth 18
#define dottedhalfeighth 84
#define dottedquarter 36


#define lamb 0
#define rainy 1
#define nookscranny 2
#define newhorizons 3

struct note {
	uint8_t alterflag;		//should button handler play alternate note (row)
	uint8_t idx; //0,1,2,3,4,5 - index to song's pitches array (column)
	uint32_t length; // units of 12.5 nanoseconds
	uint32_t pixels;
};
typedef struct note note_t;
struct song {
	uint32_t pitches[13]; // 2d array of pitches necessary for song: columns are keys and rows are the pitches per key (acess w alternate global variable)
	uint32_t tempo;		//formula in one note
	uint32_t N;			//number of notes
	note_t notes[162];
};
typedef struct song song_t;



void Sound_Init(void);

//******* Sound_Start ************
// This function does not output to the DAC.
// Rather, it sets a pointer and counter, and then enables the timer interrupt.
// It starts the sound, and the timer ISR does the output
// feel free to change the parameters
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count, uint32_t period, uint8_t NorE);
void Sound_Stop(void);
void Sound_Note(uint32_t period);

void Sound_Isabella_Voice(void);
void Sound_TomNook_Voice(void);
void Sound_Marshal_Voice(void);


#endif
