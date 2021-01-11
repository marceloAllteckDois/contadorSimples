#include <xc.h>
#define ZERO        0X03
#define UM          0X9F
#define DOIS        0X25
#define TRES        0X0d
#define QUATRO      0X99
#define CINCO       0X49
#define SEIS        0X41
#define SETE        0X1f
#define OITO        0X01
#define NOVE        0X09
#define DG1C        PORTAbits.RA0
#define DG2C        PORTAbits.RA1
#define DG3C        PORTAbits.RA2
#define DG1M        PORTAbits.RA5
#define DG2M        PORTEbits.RE0
#define DG3M        PORTEbits.RE2
#define _XTAL_FREQ 20000000
//#define CONTADOR    PORTD;
//#define MEMORIA     PORTC;
 struct BCDChar{
    unsigned char u;
    unsigned char d;
    unsigned char c;
};
typedef struct BCDChar BCDChar;
BCDChar ConverteToBCD(int);
void BCDOut( BCDChar, BCDChar);
unsigned char BCDCase(int);

 BCDChar ConverteToBCD(int n){
    BCDChar c;
    c.c=BCDCase(n/100);
    c.d=BCDCase((n%100)/10);
    c.u=BCDCase((n%100)%10);
    return c;
}
unsigned char BCDCase(int n){
    switch (n){
        case 0:
            return ZERO;
        case 1:
            return UM;
        case 2:
            return DOIS;
        case 3:
            return TRES;
        case 4:
            return QUATRO;
        case 5:
            return CINCO;
        case 6:
            return SEIS;
        case 7:
            return SETE;
        case 8:
            return OITO;
        case 9:
            return NOVE;
        default:
            return 0xff;
    }
}
void BCDOut( BCDChar c, BCDChar m){
    PORTD=0XFF;
    PORTC=0XFF;
    //__delay_ms(25);
    PORTD=c.u;
    PORTC=m.u;
    PORTAbits.RA0=1;//DG1C=1;
    PORTAbits.RA5=1;//DG1M=1;
    PORTAbits.RA1=0;//DG2C=0;
    PORTEbits.RE0=0;//DG2M=0;
    PORTAbits.RA2=0;//DG3C=0;
    PORTEbits.RE2=0;//DG3M=0;
    __delay_ms(25);
    PORTD=c.d;
    PORTC=m.d;
    PORTAbits.RA0=0;//DG1C=1;
    PORTAbits.RA5=0;//DG1M=1;
    PORTAbits.RA1=1;//DG2C=0;
    PORTEbits.RE0=1;//DG2M=0;
    PORTAbits.RA2=0;//DG3C=0;
    PORTEbits.RE2=0;//DG3M=0;
    __delay_ms(25);
    PORTD=c.c;
    PORTC=m.c;
    PORTAbits.RA0=0;//DG1C=1;
    PORTAbits.RA5=0;//DG1M=1;
    PORTAbits.RA1=0;//DG2C=0;
    PORTEbits.RE0=0;//DG2M=0;
    PORTAbits.RA2=1;//DG3C=0;
    PORTEbits.RE2=1;//DG3M=0;
    __delay_ms(25);
}