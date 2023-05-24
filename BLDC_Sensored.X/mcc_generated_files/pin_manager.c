/**
  System Interrupts Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for MPLAB(c) Code Configurator interrupts.
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - 4.45
        Device            :  dsPIC33EP256MC506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.33
        MPLAB             :  MPLAB X v4.05

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


/**
    Section: Includes
*/
#include <xc.h>
#include "pin_manager.h"

/**
    void PIN_MANAGER_Initialize(void)
*/
void PIN_MANAGER_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x0000;
    LATE = 0x0000;
    LATF = 0x0002;
    LATG = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x1F93;
    TRISB = 0x03FF;
    TRISC = 0xBFFF;
    TRISD = 0x0000;
    TRISE = 0xF000;
    TRISF = 0x0001;
    TRISG = 0x03C0;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPDD = 0x0000;
    CNPDE = 0x0000;
    CNPDF = 0x0000;
    CNPDG = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x0000;
    CNPUC = 0x0000;
    CNPUD = 0x0000;
    CNPUE = 0x0000;
    CNPUF = 0x0000;
    CNPUG = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;
    ODCF = 0x0000;
    ODCG = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x1813;
    ANSELB = 0x010F;
    ANSELC = 0x0807;
    ANSELE = 0xF000;


    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPINR12bits.FLT1R = 0x0024;   //RB4->PWM:FLT1;
    RPINR18bits.U1RXR = 0x0035;   //RC5->UART1:U1RX;
    RPOR7bits.RP97R = 0x0001;   //RF1->UART1:U1TX;

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    /****************************************************************************
     * Interrupt On Change for group CNENA - any
     ***************************************************************************/
	CNENAbits.CNIEA8 = 1; // Pin : RA8

    /****************************************************************************
     * Interrupt On Change for group CNENC - any
     ***************************************************************************/
	CNENCbits.CNIEC6 = 1; // Pin : RC6

    /****************************************************************************
     * Interrupt On Change for group CNENF - any
     ***************************************************************************/
	CNENFbits.CNIEF0 = 1; // Pin : RF0

    IEC1bits.CNIE = 1; // Enable CNI interrupt 
}

void __attribute__ (( interrupt, no_auto_psv )) _CNInterrupt ( void )
{
    if(IFS1bits.CNIF == 1)
    {
        // Clear the flag
        IFS1bits.CNIF = 0;
        IOC_CallBack();
    }
}

void __attribute__ ((weak)) IOC_CallBack(void)
{
    // Add your custom callback code here
}