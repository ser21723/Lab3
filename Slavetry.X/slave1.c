////******************************************************************************
//   UNIVERSIDAD DEL VALLE DE GUATEMALA
//   IE3054 ELECTRÓNICA DIGITAL 2 
//   AUTOR: MICHELLE SERRANO
//   COMPILADOR: XC8 (v1.41), MPLAB X IDE (v6.00)
//   PROYECTO: LABORATORIO 3
//   HARDWARE: PIC16F887
//   CREADO: 02/02/2023

#pragma config FOSC = INTRC_NOCLKOUT  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF             // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF            // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF               // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF             // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF              // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V         // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF              // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "spi.h"
#include "ADC.h"

//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 4000000

unsigned char dato_adc;
unsigned char dato_contador;
unsigned char temporal; 
int a;
int a2;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(SSPIF == 1){
        temporal = spiRead(); 
        if (temporal == 1) 
        spiWrite(dato_adc);
        else if (temporal == 0){
        spiWrite (dato_contador);
        }
        SSPIF = 0;
        }
        
}        

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    ADC_Init(AN0);
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        dato_adc = ADC_Read(0);
        __delay_ms(10);
         if (PORTBbits.RB0 == 0) 
            {
              a = 1;
            }
            if (PORTBbits.RB0 == 1 && a == 1) {
                __delay_ms(10);
                dato_contador = dato_contador + 1; 
                a = 0;
            }
            if (PORTBbits.RB1 == 0)//antirrebotes
            {
               a2 = 1;
            }
            if (PORTBbits.RB1 == 1 && a2 == 1)
            {
                __delay_ms(10);
                dato_contador = dato_contador - 1;//decrementamos en uno el puerto D
                a2 = 0;
            }
    }
return;
    }
  
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0b00000011; 
    
    PORTB = 0;
    
    dato_contador = 0;
    a = 0;
    a2 = 0;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}