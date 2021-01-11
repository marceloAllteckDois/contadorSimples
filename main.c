/*
 * File:   main.c
 * Author: eu
 *
 * Created on October 20, 2020, 7:33 AM
 */

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#define PRESCALER 1
#define _XTAL_FREQ 20000000
#define UNIDADE 0X10
#define DEZENA  0X11
#define CENTENA 0X12
#include "bcd.h"

void setup(void);
void run(void);
int contador=0;
int memoria=0;
int prescaler=PRESCALER;

void __interrupt() func(){
    BCDChar buffer;
    if(INTCONbits.INTF){
        memoria=contador;
        buffer=ConverteToBCD(memoria);
        eeprom_write(UNIDADE,buffer.u);
        eeprom_write(DEZENA,buffer.d);
        eeprom_write(CENTENA,buffer.c);
        contador=0;
        INTCONbits.INTF=0;
        return;
    }
    if(INTCONbits.TMR0IF){
        prescaler--;
        if(prescaler<=0){
            if(contador<999)
                contador++;
            prescaler=PRESCALER;
        }
        INTCONbits.TMR0IF=0;
        return;
    }
}

void main(void) {
    setup();
    //ei();
    run();
    return;
}
void setup(){
    TRISA=0;
    TRISD=0;
    TRISC=0;
    TRISE=0;
    TRISB=0X0F;
    OPTION_REG=0X97;
    INTCON=0xB0;
    //INTCON=0;
    ADCON1=0x07;
    PORTA=0;
    PORTB=0;
    PORTC=0XFF;
    PORTD=0XFF;
    PORTE=0;
    TMR0=0;
}
void run(){   
    BCDChar c,m;
    m.u=eeprom_read(UNIDADE);//EepromRead(UNIDADE);
    m.d=eeprom_read(DEZENA);//EepromRead(DEZENA);
    m.c=eeprom_read(CENTENA);//EepromRead(CENTENA);
    memoria=(m.c*100)+(m.d*10)+m.u;
    do{
        if(PORTBbits.RB1==0){
            INTCON=0;
            contador=0;
            memoria=0;
            do{
                __delay_ms(10);
            }while(PORTBbits.RB1==0);
            INTCON=0XB0;
        }
        c=ConverteToBCD(contador);
        m=ConverteToBCD(memoria);
        BCDOut(c,m);
    }while(1);
}
