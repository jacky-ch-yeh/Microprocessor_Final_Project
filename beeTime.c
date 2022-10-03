///*
// * File:   main.c
// * Author: User
// *
// * Created on 2022?1?16?, ?? 11:46
// */
//
//#include <xc.h>
//#include <pic18f4520.h>
//#include <htc.h>
//
//#pragma config OSC = INTIO67    // Oscillator Selection bits
//#pragma config WDT = OFF        // Watchdog Timer Enable bit 
//#pragma config PWRT = OFF       // Power-up Enable bit
//#pragma config BOREN = ON       // Brown-out Reset Enable bit
//#pragma config PBADEN = OFF     // Watchdog Timer Enable bit 
//#pragma config LVP = OFF        // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
//#pragma config CPD = OFF        // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
//#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
//
//#define _XTAL_FREQ 40000000
//
//// seven-segment LED signal
//
//unsigned int seg(unsigned int num) {
//    if (num < 0) {
//        return 0xBF;
//    }
//    switch (num) {
//        case 0: return 0xC0;
//        case 1: return 0xF9;
//        case 2: return 0xA4;
//        case 3: return 0xB0;
//        case 4: return 0x99;
//        case 5: return 0x92;
//        case 6: return 0x82;
//        case 7: return 0xF8;
//        case 8: return 0x80;
//        case 9: return 0x90;
//    }
//}
//
//int count;
//int timer;
//int led[4];
//int cnt;
//int finish;
//int lastMin;
//
//void __interrupt(high_priority) Hi_ISR() {
//    // min button
//    if (INTCONbits.INT0IF) {
//        if (!finish) {
//            timer += 100;
//            if (timer >= 6000)
//                timer -= 6000;
//        }
//        INTCONbits.INT0IF = 0;
//    }        // sec button
//    else if (INTCON3bits.INT1IF) {
//        if (!finish) {
//            timer += 10;
//            if (timer % 100 >= 60)
//                timer -= 60;
//        }
//        INTCON3bits.INT1IF = 0;
//    }        // finish button
//    else if (INTCON3bits.INT2IF) {
//        finish += 1;
//        __delay_us(100);
//        INTCON3bits.INT2IF = 0;
//    } else {
//        cnt = !cnt;
//        if (cnt) {
//            if (finish) {
//                timer -= 1;
//            }
//            if (timer % 100 == 99) {
//                timer -= 40;
//            }
//            led[0] = timer % 10;
//            led[1] = timer / 10 % 10;
//            led[2] = timer / 100 % 10;
//            led[3] = timer / 1000;
//            count = 0;
//        }
//        PR2 = 0x79;
//        if (timer < 100 && lastMin) {
//            PORTAbits.RA0 = !PORTAbits.RA0;
//        }
//        PIR1bits.TMR2IF = 0;
//        return;
//    }
//}
//
//void __interrupt(low_priority) lo_isr() {
//    static i = 0;
//    // led
//    PORTD = 0;
//    PORTC = 0XFF;
//    if (i == 0) {
//        PORTDbits.RD0 = 1;
//    } else if (i == 1) {
//        PORTDbits.RD1 = 1;
//    } else if (i == 2) {
//        PORTDbits.RD2 = 1;
//    } else if (i == 3) {
//        PORTDbits.RD3 = 1;
//    }
//    PORTC = seg(led[i]);
//    i += 1;
//    i %= 4;
//    // clear falg bit
//    TMR1 = 0xFF00;
//    PIR1bits.TMR1IF = 0;
//
//    return;
//}
//
//void main(void) {
//    TRISA = 0b00011000;
//    PORTA = 0;
//    LATA = 0;
//    // LEDs
//    TRISD = 0;
//    PORTD = 0;
//    TRISC = 0;
//    PORTC = 0XFF;
//
//    // Fosc
//    OSCCONbits.IRCF = 0b010; // 250kHz    
//
//    // Interrupt
//    RCONbits.IPEN = 1;
//    INTCONbits.GIEH = 1;
//    INTCONbits.GIEL = 1;
//    INTCONbits.INT0IE = 1;
//    INTCON3bits.INT1IE = 1;
//    INTCON3bits.INT1IP = 1;
//    INTCON3bits.INT1IF = 0;
//    INTCON3bits.INT2IE = 1;
//    INTCON3bits.INT2IP = 1;
//    INTCON3bits.INT2IF = 0;
//
//
//    // Timer1
//    T1CONbits.TMR1ON = 0b1;
//    T1CONbits.T1CKPS = 0b00;
//
//    PIE1bits.TMR1IE = 1;
//    IPR1bits.TMR1IP = 0;
//    PIR1bits.TMR1IF = 0;
//    TMR1 = 0xFFF0;
//
//    // Timer2
//    T2CONbits.TMR2ON = 0b1;
//    T2CONbits.T2CKPS = 0b11; // prescale 1:16
//    T2CONbits.T2OUTPS = 0b1111;
//
//    PIE1bits.TMR2IE = 1;
//    IPR1bits.TMR2IP = 1;
//    PIR1bits.TMR2IF = 0;
//
//    PR2 = 0x79;
//
//    ADCON1 = 0x0f;
//    while (1) {
//        PIE1bits.TMR2IE = 1;
//        timer = 0;
//
//        led[0] = 0;
//        led[1] = 0;
//        led[2] = 0;
//        led[3] = 0;
//
//        finish = 0;
//        lastMin = 0;
//        
//        // setting
//        while (!finish) LATAbits.LA2 = 0;
//
//        // counting down
//        //    LATA = 1;
//        
//        while (timer > 0) LATAbits.LA2 = 0;
//
//        // count to 0
//
//        lastMin = 1;
//        timer = 100; // 1 min
//        // RA2 is for canSpin
//        LATAbits.LA2 = 1;
//        while (timer > 0 && !PORTAbits.RA3 && !PORTAbits.RA4); // time out or pass or fail, break
//        if(PORTAbits.RA3){ // pass
//            PIE1bits.TMR2IE = 0;
//            
//            while(1) {
//             PORTAbits.RA5 = 1;
//             __delay_ms(1000);
//             PORTAbits.RA5 = 0;
//             __delay_ms(1000);
//            }
////            while (finish <= 3);
////            LATAbits.LA2 = 0;
////            __delay_us(100);
////            finish = 0;
////            timer = 0;
//        }
//        else if(PORTAbits.RA4){ // fail
//            PIE1bits.TMR2IE = 0;
//            while (finish <= 5);
//            LATAbits.LA2 = 0;
//            __delay_us(100);
//            finish = 0;
//            timer = 0;
//        }
//        else{ // time out
//            PIE1bits.TMR2IE = 0;
//            while (finish <= 5);
//            LATAbits.LA2 = 0;
//            __delay_us(100);
//            finish = 0;
//            timer = 0;
//        }
//        
//    }
//    return;
//}



/*
 * File:   main.c
 * Author: User
 *
 * Created on 2022?1?16?, ?? 11:46
 */

#include <xc.h>
#include <pic18f4520.h>
#include <htc.h>

#pragma config OSC = INTIO67    // Oscillator Selection bits
#pragma config WDT = OFF        // Watchdog Timer Enable bit 
#pragma config PWRT = OFF       // Power-up Enable bit
#pragma config BOREN = ON       // Brown-out Reset Enable bit
#pragma config PBADEN = OFF     // Watchdog Timer Enable bit 
#pragma config LVP = OFF        // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF        // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)

#define _XTAL_FREQ 40000000

// seven-segment LED signal

unsigned int seg(unsigned int num) {
    if (num < 0) {
        return 0xBF;
    }
    switch (num) {
        case 0: return 0xC0;
        case 1: return 0xF9;
        case 2: return 0xA4;
        case 3: return 0xB0;
        case 4: return 0x99;
        case 5: return 0x92;
        case 6: return 0x82;
        case 7: return 0xF8;
        case 8: return 0x80;
        case 9: return 0x90;
    }
}

int count;
int timer;
int led[4];
int cnt;
int finish;
int lastMin;

void __interrupt(high_priority) Hi_ISR() {
    // min button
    if (INTCONbits.INT0IF) {
        if (!finish) {
            timer += 100;
            if (timer >= 6000)
                timer -= 6000;
        }
        __delay_us(100);
        INTCONbits.INT0IF = 0;
    }// sec button
    else if (INTCON3bits.INT1IF) {
        if (!finish) {
            timer += 10;
            if (timer % 100 >= 60)
                timer -= 60;
        }
        __delay_us(100);
        INTCON3bits.INT1IF = 0;
    }// finish button
    else if (INTCON3bits.INT2IF) {
        finish += 1;
        __delay_us(100);
        INTCON3bits.INT2IF = 0;
    } else {
        cnt = !cnt;
        if (cnt) {
            if (finish) {
                timer -= 1;
            }
            if (timer % 100 == 99) {
                timer -= 40;
            }
            led[0] = timer % 10;
            led[1] = timer / 10 % 10;
            led[2] = timer / 100 % 10;
            led[3] = timer / 1000;
            count = 0;
        }
        PR2 = 0x79;
        if (timer < 100 && lastMin) {
            PORTAbits.RA0 = !PORTAbits.RA0;
        }
        PIR1bits.TMR2IF = 0;
        return;
    }
}

void __interrupt(low_priority) lo_isr() {
    static i = 0;
    // led
    PORTD = 0;
    PORTC = 0XFF;
    if (i == 0) {
        PORTDbits.RD0 = 1;
    } else if (i == 1) {
        PORTDbits.RD1 = 1;
    } else if (i == 2) {
        PORTDbits.RD2 = 1;
    } else if (i == 3) {
        PORTDbits.RD3 = 1;
    }
    PORTC = seg(led[i]);
    i += 1;
    i %= 4;
    // clear falg bit
    TMR1 = 0xFF00;
    PIR1bits.TMR1IF = 0;

    return;
}

void main(void) {
    TRISA = 0b00011000;
    TRISBbits.RB5 = 1; 
    PORTA = 0;
    // LEDs
    TRISD = 0;
    PORTD = 0;
    TRISC = 0;
    PORTC = 0XFF;

    // Fosc
    OSCCONbits.IRCF = 0b010; // 250kHz    

    // Interrupt
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    INTCONbits.INT0IE = 1;
    
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IP = 1;
    INTCON3bits.INT1IF = 0;
    
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IP = 1;
    INTCON3bits.INT2IF = 0;


    // Timer1
    T1CONbits.TMR1ON = 0b1;
    T1CONbits.T1CKPS = 0b00;

    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 0;
    PIR1bits.TMR1IF = 0;
    TMR1 = 0xFFF0;

    // Timer2
    T2CONbits.TMR2ON = 0b1;
    T2CONbits.T2CKPS = 0b11; // prescale 1:16
    T2CONbits.T2OUTPS = 0b1111;

    PIE1bits.TMR2IE = 1;
    IPR1bits.TMR2IP = 1;
    PIR1bits.TMR2IF = 0;

    PR2 = 0x79;

    ADCON1 = 0x0f;
    while (1) {
        PIE1bits.TMR2IE = 1;
        timer = 0;

        led[0] = 0;
        led[1] = 0;
        led[2] = 0;
        led[3] = 0;

        finish = 0;
        lastMin = 0;
        // setting
        PORTAbits.RA2 = 0;
        while (!finish);

        // counting down
        //    LATA = 1;
        PORTAbits.RA2 = 0;
        while (timer > 0);

        // count to 0

        lastMin = 1;
        // debug 100->500
        while (timer != 100) {
            timer = 100; // 1 min
        }
        // RA2 is for canSpin
        PORTAbits.RA2 = 1;
        __delay_us(100);

        while (timer > 0 && PORTBbits.RB5 == 0 && PORTAbits.RA4 == 0);

        if (PORTBbits.RB5) { // pass
            PIE1bits.TMR2IE = 0;
            LATAbits.LA0 = 0;
            //            while(1) {
            //             PORTAbits.RA5 = 1;
            //             __delay_ms(1000);
            //             PORTAbits.RA5 = 0;
            //             __delay_ms(1000);
            //            }
            while (finish <= 1);
            finish = 0;
            timer = 0;
        } else if (PORTAbits.RA4) { // fail
            PIE1bits.TMR2IE = 0;
            LATAbits.LA0 = 1;
            while (finish <= 3);
            finish = 0;
            timer = 0;
            __delay_us(100);
            LATAbits.LA0 = 0;
        } else { // time out
            PIE1bits.TMR2IE = 0;
            LATAbits.LA0 = 1;
            while (finish <= 3);
            finish = 0;
            timer = 0;
            __delay_us(100);
            LATAbits.LA0 = 0;
        }
        lastMin = 0;
    }
    return;
}