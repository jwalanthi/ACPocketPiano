#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "Sound.h"
#include "Timer2.h"
#include "Buttons.h"
//Buttons.c
//Jwalanthi Rangnathan and Phebe Tan
//edge triggered interrupts on port e
//weeeeeeeeee


extern song_t songs[4];
extern uint8_t songtoplay;
extern uint8_t alternate;

uint32_t T2Acount=0;
uint32_t T2Astart;
uint32_t T2Aend;
uint32_t T2Aelapsed;
int8_t whichpressed=-1;


void Timer2A_count(void) {
	T2Acount++;
}
	


void Button_Init(void){volatile long delay;                            
  SYSCTL_RCGCGPIO_R |= 0x10;           // activate port E
  while((SYSCTL_PRGPIO_R&0x10)==0){}; // allow time for clock to start
  delay = 100;                  //    allow time to finish activating
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port E
  GPIO_PORTE_CR_R = 0x3F;           // allow changes to PE5-0                              // 2) GPIO Port E needs to be unlocked
  GPIO_PORTE_AMSEL_R &= ~0x3F;  // 3) disable analog on PF5-0
                                // 4) configure PE3-0 as GPIO
  GPIO_PORTE_PCTL_R &= ~0x00FFFFFF;
  GPIO_PORTE_DIR_R &= ~0x3F;    // 5) make PE5-0 in
  GPIO_PORTE_AFSEL_R &= ~0x3F;  // 6) disable alt funct on PE5-0
	//GPIO_PORTE_PUR_R |= 0x0F;     
  GPIO_PORTE_DEN_R |= 0x3F;     // 7) enable digital I/O on PE5-0
  GPIO_PORTE_IS_R &= ~0x3F;     //    PE5-0 is edge-sensitive
  GPIO_PORTE_IBE_R |= 0x3F;    //    PE5-0 is both edges
  //GPIO_PORTE_IEV_R &= ~0x0F;     //    PF4,0 falling edge event (Neg logic)
  GPIO_PORTE_ICR_R = 0x3F;      //    clear flag3-0
  GPIO_PORTE_IM_R |= 0x3F;      // 8) arm interrupt on PE5-0
                                // 9) GPIOF priority 3
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF0F)|0x00000060;
  NVIC_EN0_R = 1<< 4;   // 10)enable interrupt 2and4 in NVIC
	Timer2_Init(Timer2A_count, 0xFFFFFFFF);
}



void GPIOPortE_Handler(void){
	static uint32_t last0=0, last1=0, last2=0, last3=0, last4=0, last5=0;
	if ((GPIO_PORTE_RIS_R & 0x01)==0x01){ //PE0
		uint32_t now = GPIO_PORTE_DATA_R & 0x01;
		if ((now==1) && (last0==0)){
			//rising edge
			Sound_Note(songs[songtoplay].pitches[0+(6*alternate)]);
			T2Astart=TIMER2_TAR_R;
			T2Acount=0;
			whichpressed=0;
		}
		 if ((now==0)&&(last0==1)){
			//falling edge
			Sound_Stop();
			T2Aend=TIMER2_TAR_R;
			T2Aelapsed = (0x100000000*T2Acount)+(T2Astart-T2Aend);
		}
		last0=now;
	}
	else if ((GPIO_PORTE_RIS_R & 0x02)==0x02){ //PE1
		uint32_t now = GPIO_PORTE_DATA_R & 0x02;
		if ((now==2) && (last1==0)){
			//rising edge
			Sound_Note(songs[songtoplay].pitches[1+(6*alternate)]);
			T2Astart=TIMER2_TAR_R;
			T2Acount=0;
			whichpressed=1;
		}
		if ((now==0)&&(last1==2)){
			//falling edge
			Sound_Stop();
			T2Aend=TIMER2_TAR_R;
			T2Aelapsed = (0x100000000*T2Acount)+(T2Astart-T2Aend);
		}
		last1=now;
	}
	else if ((GPIO_PORTE_RIS_R & 0x04)==0x04){ //PE2
		uint32_t now = GPIO_PORTE_DATA_R & 0x04;
		if ((now==4) && (last2==0)){
			//rising edge
			Sound_Note(songs[songtoplay].pitches[2+(6*alternate)]);
			T2Astart=TIMER2_TAR_R;
			T2Acount=0;
			whichpressed=2;
			
		}
		if ((now==0)&&(last2==4)){
			//falling edge
			Sound_Stop();
			T2Aend=TIMER2_TAR_R;
			T2Aelapsed = (0x100000000*T2Acount)+(T2Astart-T2Aend);
		}
		last2=now;
	}
	else if ((GPIO_PORTE_RIS_R & 0x08)==0x08){ //PE3
		uint32_t now = GPIO_PORTE_DATA_R & 0x08;
		if ((now==8) && (last3==0)){
			//rising edge
			Sound_Note(songs[songtoplay].pitches[3+(6*alternate)]);
			T2Astart=TIMER2_TAR_R;
			T2Acount=0;
			whichpressed=3;
		}
		if ((now==0)&&(last3==8)){
			//falling edge
			Sound_Stop();
			T2Aend=TIMER2_TAR_R;
			T2Aelapsed = (0x100000000*T2Acount)+(T2Astart-T2Aend);
		}
		last3=now;
	}
		else if ((GPIO_PORTE_RIS_R & 0x10)==0x10){ //PE4
		uint32_t now = GPIO_PORTE_DATA_R & 0x10;
		if ((now==0x10) && (last4==0)){
			//rising edge
			Sound_Note(songs[songtoplay].pitches[4+(6*alternate)]);
			T2Astart=TIMER2_TAR_R;
			T2Acount=0;
			whichpressed=4;
		}
		 if ((now==0)&&(last4==0x10)){
			//falling edge
			Sound_Stop();
			T2Aend=TIMER2_TAR_R;
			T2Aelapsed = (0x100000000*T2Acount)+(T2Astart-T2Aend);
		}
		last4=now;
	}
		else if ((GPIO_PORTE_RIS_R & 0x20)==0x20){ //PE5
		uint32_t now = GPIO_PORTE_DATA_R & 0x20;
		if ((now==0x20) && (last5==0)){
			//rising edge
			Sound_Note(songs[songtoplay].pitches[5+(6*alternate)]);
			T2Astart=TIMER2_TAR_R;
			T2Acount=0;
			whichpressed=5;
		}
		 if ((now==0)&&(last5==0x20)){
			//falling edge
			Sound_Stop();
			T2Aend=TIMER2_TAR_R;
			T2Aelapsed = (0x100000000*T2Acount)+(T2Astart-T2Aend);
		}
		last5=now;
	}
	else {
		whichpressed = -1;
	}
  GPIO_PORTE_ICR_R = 0x3F;      // acknowledge flag4
}


