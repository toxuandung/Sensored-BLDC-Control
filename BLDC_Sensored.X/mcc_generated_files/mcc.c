/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : 1.53.0.1
        Device            :  dsPIC33EP256MC506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.33
        MPLAB             :  MPLAB X v4.05
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

// Configuration bits: selected in the GUI

// FICD
#pragma config ICS = PGD2    // ICD Communication Channel Select bits->Communicate on PGEC2 and PGED2
#pragma config JTAGEN = OFF    // JTAG Enable bit->JTAG is disabled

// FPOR
#pragma config ALTI2C1 = OFF    // Alternate I2C1 pins->I2C1 mapped to SDA1/SCL1 pins
#pragma config ALTI2C2 = OFF    // Alternate I2C2 pins->I2C2 mapped to SDA2/SCL2 pins
#pragma config WDTWIN = WIN25    // Watchdog Window Select bits->WDT Window is 25% of WDT period

// FWDT
#pragma config WDTPOST = PS1    // Watchdog Timer Postscaler bits->1:1
#pragma config WDTPRE = PR32    // Watchdog Timer Prescaler bit->1:32
#pragma config PLLKEN = ON    // PLL Lock Enable bit->Clock switch to PLL source will wait until the PLL lock signal is valid.
#pragma config WINDIS = OFF    // Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config FWDTEN = OFF    // Watchdog Timer Enable bit->Watchdog timer enabled/disabled by user software

// FOSC
#pragma config POSCMD = XT    // Primary Oscillator Mode Select bits->XT Crystal Oscillator Mode
#pragma config OSCIOFNC = ON    // OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#pragma config IOL1WAY = OFF    // Peripheral pin select configuration->Allow multiple reconfigurations
#pragma config FCKSM = CSECMD    // Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FOSCSEL
#pragma config FNOSC = FRC    // Oscillator Source Selection->FRC
#pragma config PWMLOCK = OFF    // PWM Lock Enable bit->PWM registers may be written without key sequence
#pragma config IESO = ON    // Two-speed Oscillator Start-up Enable bit->Start up device with FRC, then switch to user-selected oscillator source

// FGS
#pragma config GWRP = OFF    // General Segment Write-Protect bit->General Segment may be written
#pragma config GCP = OFF    // General Segment Code-Protect bit->General Segment Code protect is Disabled

#include "mcc.h"

/**
 Section: Local Variables
*/

/**
 Section: Function prototypes
*/
bool SYSTEM_ResetCauseFromSoftware(uint16_t resetCause);
bool SYSTEM_ResetCauseFromWatchdogTimer(uint16_t resetCause);
bool SYSTEM_ResetCauseFromConfigurationMismatch(uint16_t resetCause);
bool SYSTEM_ResetCauseFromIllegalOpcode(uint16_t resetCause);
bool SYSTEM_ResetCauseFromExternal(uint16_t resetCause);
bool SYSTEM_ResetCauseFromTrap(uint16_t resetCause);
void SYSTEM_ResetCauseClear(RESET_MASKS resetFlagMask);

/**
* a private place to store the error code if we run into a severe error
*/

void SYSTEM_Initialize(void)
{
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    INTERRUPT_Initialize();
    UART1_Initialize();
    ADC1_Initialize();
    PWM_Initialize();
    TMR2_Initialize();
    INTERRUPT_GlobalEnable();
    CORCON_ModeOperatingSet(CORCON_MODE_PORVALUES);
}

void OSCILLATOR_Initialize(void)
{
    // FRCDIV FRC/1; PLLPRE 2; DOZE 1:8; PLLPOST 1:2; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3000;
    // TUN Center frequency; 
    OSCTUN = 0x0;
    // ROON disabled; ROSEL disabled; RODIV Base clock value; ROSSLP disabled; 
    REFOCON = 0x0;
    // PLLDIV 68; 
    PLLFBD = 0x44;
    // RND disabled; SATB disabled; SATA disabled; ACCSAT disabled; 
	CORCONbits.RND = 0;
	CORCONbits.SATB = 0;
	CORCONbits.SATA = 0;
	CORCONbits.ACCSAT = 0;
    // CF no clock failure; NOSC PRIPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) ((0x300 >> _OSCCON_NOSC_POSITION) & 0x00FF));
    __builtin_write_OSCCONL((uint8_t) ((0x300 | _OSCCON_OSWEN_MASK) & 0xFF));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
}

uint16_t SYSTEM_GetResetCause(void)
{
    return RCON;
}

void __attribute__ ((weak)) SYSTEM_ResetCauseHandler(void)
{
    uint16_t resetCause = SYSTEM_GetResetCause();
    if(SYSTEM_ResetCauseFromTrap(resetCause))
    { 
      SYSTEM_ResetCauseClear(RESET_MASK_TRAPR); 
      //Do something 
    }
    if(SYSTEM_ResetCauseFromIllegalOpcode(resetCause))
    { 
      SYSTEM_ResetCauseClear(RESET_MASK_IOPUWR); 
      //Do something 
    }
    if(SYSTEM_ResetCauseFromConfigurationMismatch(resetCause))
    { 
      SYSTEM_ResetCauseClear(RESET_MASK_CM); 
      //Do something 
    }
    if(SYSTEM_ResetCauseFromExternal(resetCause))
    { 
      SYSTEM_ResetCauseClear(RESET_MASK_EXTR); 
      //Do something 
    }
    if(SYSTEM_ResetCauseFromSoftware(resetCause))
    { 
      SYSTEM_ResetCauseClear(RESET_MASK_SWR); 
      //Do something 
    }
    if(SYSTEM_ResetCauseFromWatchdogTimer(resetCause))
    { 
      SYSTEM_ResetCauseClear(RESET_MASK_WDTO); 
      //Do something 
    }
}

bool SYSTEM_ResetCauseFromTrap(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_TRAPR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

bool SYSTEM_ResetCauseFromIllegalOpcode(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_IOPUWR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

bool SYSTEM_ResetCauseFromConfigurationMismatch(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_CM)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

bool SYSTEM_ResetCauseFromExternal(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_EXTR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

bool SYSTEM_ResetCauseFromSoftware(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_SWR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

bool SYSTEM_ResetCauseFromWatchdogTimer(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_WDTO)
    { 
      resetStatus = true;
    }
    return resetStatus;
}

void SYSTEM_ResetCauseClear(RESET_MASKS resetFlagMask)
{ 
     RCON = RCON & (~resetFlagMask); 
} 

void SYSTEM_ResetCauseClearAll()
{ 
    RCON = 0x00; 
}
/**
 End of File
*/