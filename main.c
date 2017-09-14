/* 
 * File:   main.c
 * Author: A19356
 *
 * Created on August 22, 2017, 7:42 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#define CCP1 14

void initialize_System(void);
void initialize_OSC(void);
void unlock_PPS(void);
void lock_PPS(void);



/*==============================================================================
 ===== MAIN ====================================================================
 =============================================================================*/
int main(int argc, char** argv) {
    
    initialize_System();
    
    
    unlock_PPS();
    RPOR2 = CCP1;
    lock_PPS();
          
    
    while(1){
    }

    return (EXIT_SUCCESS);
}

void initialize_System(void){
    initialize_OSC();
    PIN_MANAGER_Initialize();
    TMR2_Initialize();
    PWM4_Initialize();
}

void initialize_OSC(void){
    // SCS FOSC; HFIOFS not stable; IDLEN disabled; IRCF 16MHz_HF; 
    OSCCON = 0x70;
    // INTSRC INTRC; PLLEN disabled; TUN 0; 
    OSCTUNE = 0x00;
    // ROSEL System Clock(FOSC); ROON disabled; ROSSLP Disabled in Sleep mode; RODIV Fosc; 
    REFOCON = 0x00;
}

void unlock_PPS(void){
    if(GIE){                //save the status of global interrupt
        STATUSbits.CARRY = GIE;
        GIE = 0;
    }
#asm
    MOVLB 0x0E              //sequence followed for unlocking PPS
    MOVLW 0x55
    MOVWF EECON2
    MOVLW 0xAA
    MOVWF EECON2
    BCF   PPSCON, 0
#endasm
}

void lock_PPS(void){
#asm
    MOVLW   0x55
    MOVWF   EECON2
    MOVLW   0xAA
    MOVWF   EECON2
    BSF     PPSCON, 0
#endasm
    if(STATUSbits.CARRY){   //restore global interrupts if any
        GIE = 1;
        STATUS = 0;
    }
}


