// PocketPiano.c
// Runs on TM4C123
// Jwalanthi Ranganthan and Phebe Tan
// This is a starter project for the EE319K Lab 10

// Last Modified: 1/12/2022 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE5
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer1.h"
#include "../Lab6_EE319K/Debug.h"
#include "../SysTick_4C123/SysTick.h"
#include "Buttons.h"

#define ABS(x) (((x)<(0))?(-x):(x))

extern uint32_t T2Aelapsed;
extern int8_t whichpressed;
extern song_t songs[4];

uint32_t score;
uint32_t myL;
uint32_t myC;
uint8_t notedone=0;
int32_t y[162];
uint32_t beatcount;
uint8_t songtoplay;
uint8_t alternate;
uint32_t j=0;
uint32_t colors[8]= {ST7735_BLUE, ST7735_RED, ST7735_GREEN,ST7735_YELLOW, ST7735_BLUE, ST7735_RED, ST7735_GREEN,ST7735_YELLOW};
struct sprite {
	const unsigned short *pt;
	uint32_t w;
	uint32_t h;
};
typedef struct sprite sprite_t;

sprite_t sprites[9] ={
	{isabella1, 45, 57},
	{isabella2, 45, 57},
	{tulip, 10, 25},
	{tomnook1, 40, 57},
	{tomnook2, 40, 57},
	{hammer, 20, 14},
	{marshal1, 45, 53},
	{marshal2, 45, 53},
	{wood, 20, 14}
};
sprite_t image1;
sprite_t image2;
sprite_t object;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds




// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
typedef enum {English, Spanish} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE, GO, TRY, TRY2, TALK, KEYBYE, SING} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Go_English[]="Go!!";
const char Go_Spanish[]="\xADV\xA0manos!";
const char Try_English[]="Press Reset to";
const char Try2_English[]="try again";
const char Try_Spanish[]="Dale un apret\xA2n al Reset";
const char Try2_Spanish[]="para intentar a nuevo";
const char Talk_English[]="Key0: talk";
const char Talk_Spanish[]="Key0: hablar";
const char Keybye_English[]="Key1: Goodbye";
const char Keybye_Spanish[]="Key1: Adi\xA2s";
const char Sing_English[]="Key0: sing";
const char Sing_Spanish[]="Key0: cantar";


const char *Phrases[9][2]={
  {Hello_English,Hello_Spanish},
  {Goodbye_English,Goodbye_Spanish},
  {Language_English,Language_Spanish},
	{Go_English, Go_Spanish},
	{Try_English, Try_Spanish},
	{Try2_English, Try2_Spanish},
	{Talk_English, Talk_Spanish},
	{Keybye_English,Keybye_Spanish},
	{Sing_English, Sing_Spanish}
};


void Falling_Keys (void) {
	for(uint32_t m=0; m<songs[songtoplay].N; m++) {
		y[m]++;
	}
	if((y[j]+songs[songtoplay].notes[j].pixels)>=100){
		beatcount--;
		if (beatcount == 0) {
			notedone = 1;
			notedone = 1;
			notedone = 1;
			notedone = 1;
			j++;
			beatcount = songs[songtoplay].notes[j].pixels;
			alternate = songs[songtoplay].notes[j].alterflag;
		}
	}
}

void introscreen (void) {
	//show animal crossing logo
	ST7735_DrawBitmap(0,127, titlescreen, 160, 128);
	//delay a bit
	SysTick_Wait10ms(150);
	//language selection
	ST7735_FillScreen(ST7735_BLACK);
	uint8_t y, yold;
	ST7735_SetCursor(0,0);
	ST7735_OutString("Please use slidepot");
	ST7735_OutChar(13);
	ST7735_OutString("to move cursor");
	ST7735_OutChar(13);
	ST7735_OutString("and Key0 to select");
	ST7735_SetCursor(3,5);
	ST7735_OutString((char*)Phrases[LANGUAGE][English]);
	ST7735_SetCursor(3,8);
	ST7735_OutString((char*)Phrases[LANGUAGE][Spanish]);
	while(1) {
		y=(ADC_In())/341;
			//if (y !=yold) {
				if (y>=5 && y<=8) {
					ST7735_SetCursor(15,yold);
					ST7735_OutChar(' ');
					ST7735_SetCursor(15,y);
					ST7735_OutChar(0x3C);
					if(whichpressed==0) {
						if(y==5){
							myL=English;
							break;
						}
						if(y==8){
							myL=Spanish;
							break;
						}
					}
					yold=y;
				}
			}
		//}
	ST7735_FillScreen(ST7735_BLACK);
	
}

void gameinit(void) {
	ST7735_FillScreen(ST7735_BLACK);
	SysTick_Wait10ms(100);
	ST7735_SetCursor(3,4);
	ST7735_OutString("3...");
	SysTick_Wait10ms(100);
	ST7735_SetCursor(3,4);
	ST7735_OutString("2...");
	SysTick_Wait10ms(100);
	ST7735_SetCursor(3,4);
	ST7735_OutString("1...");
	SysTick_Wait10ms(100);
	ST7735_SetCursor(3,4);
	ST7735_OutString((char*)Phrases[GO][myL]);
	SysTick_Wait10ms(200);
	ST7735_FillRect(0,0,92,100,0);
	beatcount = songs[songtoplay].notes[0].pixels;
	alternate = songs[songtoplay].notes[0].alterflag;
	j=0;
	y[0]=0;
	for (uint32_t k=1; k<songs[songtoplay].N; k++) {
		y[k]=(y[k-1]-(songs[songtoplay].notes[k].pixels+2));
	}
	Timer1_Init(Falling_Keys, songs[songtoplay].tempo);
	
}
void gameplay(void) {
	//init score
	score=0;
	//notedone=0;
	ST7735_SetCursor(17,11);
	LCD_OutDec(score);
	ST7735_FillRect(92,0,68,128,ST7735_WHITE);
	ST7735_DrawBitmap(100,40,speech_bubble,60,40);
	ST7735_DrawBitmap(110,100,image1.pt, image1.w,image1.h);
	//init missed flag
	uint8_t missed=0;
	//for each note
		for(uint32_t i=0; i<songs[songtoplay].N; i++) {
			Debug_Beat();
			//wait for correct time
			//SysTick_Wait10ms(((lamb.notes[i].length/1000000)*25)/20);			//convert from units of 12.5 nanoseconds to centiseconds/units of 10 ms //not used anymore, handled by timer1 interrupt
			while (1) {
				ST7735_SetCursor(17,11);
				LCD_OutDec(score);
				//for debugging
				//ST7735_SetCursor(22,10);
				//LCD_OutDec(j);
				//ST7735_SetCursor(22,11);
				//LCD_OutDec(i);
				if (i==31) {
					i=2*i/2;
				};
				ST7735_DrawBitmap (120, 124, object.pt, object.w, object.h);
				ST7735_FillRect(0,100,92,28, ST7735_WHITE);
				for (uint32_t n=0; n<songs[songtoplay].N; n++) {
					if (y[n]>0&&y[n]<=100) {
						if (songs[songtoplay].notes[n].idx == 13) { //rest
							ST7735_FillRect(0,y[n],13,songs[songtoplay].notes[n].pixels, ST7735_BLACK);
						}
						ST7735_FillRect(((songs[songtoplay].notes[n].idx*15)+1),y[n]-3,13,3, ST7735_BLACK);
						ST7735_FillRect(((songs[songtoplay].notes[n].idx*15)+1),y[n],13,songs[songtoplay].notes[n].pixels, colors[songs[songtoplay].notes[n].idx]);
					}
				}
				if (notedone) {
					if (songs[songtoplay].notes[i].idx != 13) {//skip check if rest
						//check if time and key good
						int32_t error = T2Aelapsed-songs[songtoplay].notes[i].length;
						error = ABS(error);
						//award point
						if ((error<20000000) && (whichpressed==songs[songtoplay].notes[i].idx) ){
							score++;
							missed=0;
							ST7735_DrawBitmap(112,33, check, 35, 27);
							ST7735_DrawBitmap(110,100,image1.pt, image1.w,image1.h);
						}
						else {
							ST7735_DrawBitmap(112,33, x, 30, 29);
							ST7735_DrawBitmap(110,100,image2.pt, image2.w,image2.h);
						}
					}
					whichpressed=-1;
					notedone=0;
					break;
				}
			}
			//next note
			//ST7735_FillRect(((lamb.notes[i].idx*12)+1),0,10,100, ST7735_BLACK);
			ST7735_SetCursor(17,11);
			LCD_OutDec(score);
		}
		TIMER1_CTL_R=0;
	}

void tryagain (void) {
	ST7735_SetCursor(0,0);
	ST7735_OutString((char*)Phrases[TRY][myL]);
	ST7735_OutChar(13);
	ST7735_OutString((char*)Phrases[TRY2][myL]);
	ST7735_OutChar(13);
	while(1) {
	}
}

void interact (phrase_t action) {
	ST7735_SetCursor(0,0);
	ST7735_OutString((char*)Phrases[action][myL]);
	ST7735_OutChar(13);
	ST7735_OutString((char*)Phrases[KEYBYE][myL]);
	ST7735_OutChar(13);
	while(1) {
		ST7735_DrawBitmap(110,100,image1.pt, image1.w,image1.h);
		if (whichpressed==0){
			ST7735_DrawBitmap(110,100,image2.pt, image2.w,image2.h);
			if(songtoplay==rainy) {
				Sound_Isabella_Voice();
				SysTick_Wait10ms(300);
			}
			else if(songtoplay==nookscranny||(songtoplay==newhorizons)) {
				Sound_TomNook_Voice();
				SysTick_Wait10ms(700);
			}
			else if(songtoplay==newhorizons) {
				//Sound_Marshal_Voice();
				//SysTick_Wait10ms(500);
			}
		}
		if (whichpressed==1) {
			ST7735_SetCursor(0,5);
			ST7735_OutString((char*)Phrases[GOODBYE][myL]);
			SysTick_Wait10ms(300);
			break;
		}
	}
}

int main(void){ 
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Output_Init();
	ADC_Init();
	ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_SetRotation(1);
	SysTick_Init();
	Sound_Init();
	Debug_Init();
	Button_Init();
	EnableInterrupts();
	Sound_Stop();
		introscreen();
		//songtoplay = lamb;
		//gameinit();
		//gameplay();
		SysTick_Wait10ms(100);
		
		//init global variables for rainy
		songtoplay = rainy;
			image1=sprites[0];
			image2=sprites[1];
			object=sprites[2];
			gameinit();
			gameplay();
			whichpressed=-1;
			if(score<37) {
				tryagain();
				songtoplay=rainy;
			}
		interact(TALK);
		
		//init global variables for nookscranny
		songtoplay = nookscranny;
			image1=sprites[3];
			image2=sprites[4];
			object=sprites[5];
			gameinit();
			gameplay();
			whichpressed=-1;
			if(score<50) {
				tryagain();
				songtoplay=nookscranny;
			}
			interact(SING);
			//init global variables for new horizons
			songtoplay=newhorizons;
			image1=sprites[6];
			image2=sprites[7];
			object=sprites[8];
			gameinit();
			gameplay();
			whichpressed=-1;
			if(score<50) {
				tryagain();
				songtoplay=newhorizons;
			}
			interact(SING);
			while(1) {
			ST7735_DrawBitmap(0,127, finalscreen, 160, 128);
		};
}
/*  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Delay100ms(30);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Delay100ms(20);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }  */
	
//}
/*
int main1(void){
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom

  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);

  Delay100ms(50);              // delay 5 sec at 80 MHz

  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);
  while(1){
  }

}
*/
