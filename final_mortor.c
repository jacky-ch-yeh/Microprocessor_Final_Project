#include <xc.h>
#include <pic18f4520.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#pragma config OSC = INTIO67    // Oscillator Selection bits
#pragma config WDT = OFF        // Watchdog Timer Enable bit 
#pragma config PWRT = OFF       // Power-up Enable bit
#pragma config BOREN = ON       // Brown-out Reset Enable bit
#pragma config PBADEN = OFF     // Watchdog Timer Enable bit 
#pragma config LVP = OFF        // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF        // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)

#define _XTAL_FREQ 125000
// 4 7 10 13 16 19 

// ccp2: right 0x13 up  ccp1: left 0x13 down
int isAlreadyRand = 0;
int position[6] = {14, 9, 4, 18, 14, 9};
int answer[6] = {-1};

int answerInBinary[7];
int state = 0;

int isAnswerSet = 0;

int canSpin = 0;

//int answer[8] = {1, 0, 5, 3, 4, 2};
void __interrupt(high_priority)  randomPattern(void)
{
    if(canSpin)
    {
        // answer has been generated
        isAnswerSet = 1;

        if(!isAlreadyRand)
        {
            isAlreadyRand = 1;
            // generate a pattern
            for(int i = 0; i < 6; i++)
                answer[i] = i;

            // avoid generating replicated position
            for(int i = 0; i < 6; i++) {
                int r = rand() % 6;
                int temp = answer[i];
                answer[i] = answer[r];
                answer[r] = temp;
            }
        }
    //    int idx = 0;
    //    while(idx < 8) 
    //    {
    //        while(1) {
    //            int isReplicated = 0;
    //            answer[idx] = rand() % 8;
    //            
    //            for(int i = 0; i < idx; i ++) {
    //                if(answer[idx] == answer[i]) 
    //                    isReplicated = 1;
    //            }
    //            
    //            if(!isReplicated)
    //                break;
    //        }
    //        
    //        idx += 1;
    //    }

        // move motors based on pattern
        for(int i = 0; i < 6; i ++) {
            // motors move to beginning point
            CCPR1L = 0x04;
            CCPR2L = 0x13;

            __delay_ms(800);

            int fidx = answer[i];
            if(fidx < 3) 
                CCPR2L = position[fidx];
            else 
                CCPR1L = position[fidx];

            __delay_ms(800);
        }
        CCPR1L = 0x04;
        CCPR2L = 0x13;

    }
    
    // clear interrupt flag
    INTCONbits.INT0IF = 0;
    
    return;
}

void main(void)
{
    // set random seed
    srand(time(NULL));
    
    // set RB0 to digital input 
    ADCON1 = 0x0f;
    TRISB = 0xff;
//    TRISBbits.RB0 = 1;
//    TRISBbits.RB4 = 1;
    PORTB = 0;
    LATB = 0;
    
    // enable interrupt bits
    INTCON2bits.INTEDG0 = 0;
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    // for random pattern
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    
    // Timer2 -> On, prescaler -> 4
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b01;

    // Internal Oscillator Frequency, Fosc = 125 kHz, Tosc = 8 µs
    OSCCONbits.IRCF = 0b001;
    
    // Set CCP1 and CCP2 to PWM mode, P1A, P1C active-high; P1B, P1D active-high
    CCP1CONbits.CCP1M = 0b1100;
    CCP2CONbits.CCP2M = 0b1100;
    
    // CCP1/RC2 and CCP2/RC1 -> Output
    TRISC = 0;
    TRISCbits.RC4 = 1; // RC4 input
    LATC = 0;
    
    PR2 = 0x9b;
    
    CCPR1L = 0x04;
    CCP1CONbits.DC1B = 0b00;
    
    CCPR2L = 0x13;
    CCP2CONbits.DC2B = 0b00;
    
    // bomb!
    TRISD = 0xff;
    TRISA = 0;
    PORTA = 0;
    LATA = 0;
    
    while(1)
    {
        LATAbits.LA1 = 0;
        LATAbits.LA2 = 0;
        
        if(PORTBbits.RB2) {
            canSpin = 1;
        }
        
        
        // if user ask for the answer pattern for the first time
        if(isAnswerSet)
        {
            // transfer answer from array form to binary form
            for(int i  = 0; i < 7; i++){
                if (i == 0){
                    answerInBinary[i] = 63;
                }
                else{
                    answerInBinary[i] =  answerInBinary[i-1] - pow(2, answer[i-1]);
                }

            }
            // bomb
            while(1)
            {
                LATAbits.LA1 = 0;
                LATAbits.LA4 = 0;
                if((PORTD & answerInBinary[state]) == answerInBinary[state]){
                    continue;
                }
                else if((PORTD & answerInBinary[state]) == answerInBinary[state + 1]){
                    state++;
                }
                else{ // Fail
                    LATAbits.LA1 = 1;
                    __delay_ms(50);
                    isAnswerSet = 0;
                    isAlreadyRand = 0;
                    canSpin = 0;
                    break;
                    // output fail signal to them
                    // while(1);
                }
                if(state == 6){ // Pass
                    LATAbits.LA4 = 1;
                    __delay_ms(50);
                    isAnswerSet = 0;
                    isAlreadyRand = 0;
                    canSpin = 0;
                    break;
                    // output Pass signal to them
                    // while(1);
                }

            }
        }
    }
    
    return;
}