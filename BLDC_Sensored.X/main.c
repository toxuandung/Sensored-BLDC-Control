/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  Description:
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
#include <stdint.h>
#include "mcc_generated_files/mcc.h"

const uint16_t PWM_STATE1_CW[8] = {0xC301, 0xC341, 0xC301, 0xC341, 0xC101, 0xC301, 0xC101, 0xC301};
const uint16_t PWM_STATE2_CW[8] = {0xC301, 0xC301, 0xC101, 0xC101, 0xC341, 0xC341, 0xC301, 0xC301};
const uint16_t PWM_STATE3_CW[8] = {0xC301, 0xC101, 0xC341, 0xC301, 0xC301, 0xC101, 0xC341, 0xC301};

#define MIN_DUTY    400
#define MAX_DUTY    5600
#define STARTUP_DUTY    500
#define MAX_RPM    6000
#define MIN_RPM    700
#define TIME_ROTATE    5000

#define CYCLES_ROTATE 200 // Number cycles motor rotating  
#define V_MAX 1000         // Max RRM rotating
#define A_MAX 1            // Acceleration ms
#define RPM_TO_RPmS_SCALE 60000  

#define	S3	!PORTGbits.RG6	//S2 button
#define S2	!PORTGbits.RG7	//S3 button

unsigned int DutyCycle ;
unsigned int DutyCycle_PI ;


uint16_t CommutationIdx = 0;
uint16_t CommutationTime;

int16_t ADC1_Ch1,ADC1_Ch2,ADC1_Ch3,V_pot;


uint16_t Current_RPM ;
uint16_t Desired_RPM ;
float Error_Integral ;

int16_t Speed_Error ;

float kp ;
float ki ;

uint16_t speed_ms_count ;
uint16_t speed_old_ms_count ;
uint16_t stoped_detect_ms_count ;
uint16_t detect_first_circle_count ;
uint16_t PI_ms_count ;

uint16_t Cycles_count ;

uint16_t CW ;
uint16_t Start;

uint16_t T1, T2, T3, C1, C2, C3; // motion profile
uint16_t T_Motion; // current time for motion profile calculation
uint16_t T_Total;
uint16_t S_Curve_RPM;

unsigned int HallValue;
unsigned int Hall_A;
unsigned int Hall_B;
unsigned int Hall_C;

unsigned int HallValue_1ms_update;
unsigned int HallValue_2ms_update;
unsigned int HallValue_3ms_update;
unsigned int HallValue_5ms_update;
unsigned int HallValue_7ms_update;
unsigned int HallValue_9ms_update;
unsigned int HallValue_10ms_update;
unsigned int HallValue_15ms_update;
unsigned int HallValue_20ms_update;
unsigned int HallValue_25ms_update;
unsigned int HallValue_30ms_update;

unsigned int Motor_Status;


int delay_counter;

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalDisable();
    
    RCONbits.SWDTEN = 0; // Disable Watch Dog Timer
    X2CScope_Init();
    
    calculateSCurveTime() ;
    // Get the PTPER value for 1 ms

       
    IOCON1 = IOCON2 = IOCON3 =0xC001;
    Motor_Status = 0 ;
    speed_ms_count = 0 ;
    speed_old_ms_count = 0 ;
    PI_ms_count = 0 ;
    Start = 0 ;
    CW = 1 ;
//    Desired_RPM = 1000 ;
    kp = 0.07 ; 
    ki = 0.003 ; 

    HallValue = (0x0007&((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue())) ;
    
     
    while (1)
    {
        
        //Desired_RPM = (int)(((V_pot+32768.000)/65536.000)*MAX_RPM);
        Desired_RPM  = S_Curve_RPM ;
        if (Desired_RPM < MIN_RPM)
        {
            Desired_RPM = MIN_RPM ;
        }
        if (Desired_RPM > MAX_RPM)
        {
            Desired_RPM = MAX_RPM ;
        }
        //DutyCycle = Desired_RPM ;
        DutyCycle = DutyCycle_PI;
        
        if(DutyCycle < MIN_DUTY)DutyCycle=MIN_DUTY;
        if(DutyCycle > MAX_DUTY)DutyCycle=MAX_DUTY;
        
        if (Motor_Status == 1)
        {
            if (speed_old_ms_count !=0)
            {
                Current_RPM = (int)(1000*60/speed_old_ms_count);
            }
            else 
            {
                Current_RPM = 0 ;
            }                      
        }
        else 
        {
            Current_RPM = 0 ;
        }
        
        if (Current_RPM > MAX_RPM)
        {
            Current_RPM = MAX_RPM ;
        }
        
//        Start = (Start||S2||S3) && (!(Cycles_count == CYCLES_ROTATE));
//        CW = (CW||  (S2 &&(!(Motor_Status==1)))   ) && (!(S3&&(!(Motor_Status==1)) )) ;
        
        Start = (Start||S2) && (!(Cycles_count == CYCLES_ROTATE)) && (!S3) ;
       
        
        if ((Cycles_count >= CYCLES_ROTATE)||(Start == 0)) 
        {
            Cycles_count = 0 ;
        }
        
        
        if (Start == 1)
        {
            LED_D2_SetHigh() ;
            PWM_DutyCycleSet(1,DutyCycle);
            PWM_DutyCycleSet(2,DutyCycle);
            PWM_DutyCycleSet(3,DutyCycle);
            
            if (CW == 1)
            {
                IOCON1 = PWM_STATE1_CW[HallValue];
                IOCON2 = PWM_STATE2_CW[HallValue];
                IOCON3 = PWM_STATE3_CW[HallValue];
            }
            else
            {
                IOCON1 = PWM_STATE1_CW[7-HallValue];
                IOCON2 = PWM_STATE2_CW[7-HallValue];
                IOCON3 = PWM_STATE3_CW[7-HallValue];
            }

        }
        else
        {
            Error_Integral = 0 ;
            
            LED_D2_SetLow() ;
            PWM_DutyCycleSet(1,0);
            PWM_DutyCycleSet(2,0);
            PWM_DutyCycleSet(3,0);
        }
              
        X2CScope_Communicate();
       
        
    }
}


void TMR2_CallBack(void)
{
                  
    if (Start == 1)
    {
        speed_ms_count++; 
        T_Motion ++ ;
        calculateSCurveMotion_();
    }
    else
    {
        speed_ms_count = 0;
        speed_old_ms_count = 0 ;
        detect_first_circle_count = 0 ;
        T_Motion = 0 ;
        S_Curve_RPM = 0 ;
    }
       
    stoped_detect_ms_count++ ;
         
    if (stoped_detect_ms_count > 30)
    {
        stoped_detect_ms_count = 0 ;
    }
    
    switch(stoped_detect_ms_count) {

        case 1:
            HallValue_1ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;
        case 2:
            HallValue_2ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;
        case 3:
            HallValue_3ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;        
        case 5:
            HallValue_5ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;
        case 7:
            HallValue_7ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;       
        case 9:
            HallValue_9ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;       
        case 10:
            HallValue_10ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;        
        case 15:
            HallValue_15ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;        
        case 20:
            HallValue_20ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;         
        case 25:
            HallValue_25ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;         
        case 30:
            HallValue_30ms_update = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
        break;         
        
        default :
            break ;           
        
    }
        
    if ((HallValue_1ms_update == HallValue_2ms_update)&&
            (HallValue_2ms_update == HallValue_3ms_update)&&
            (HallValue_3ms_update == HallValue_5ms_update)&&
            (HallValue_7ms_update == HallValue_9ms_update)&&
            (HallValue_9ms_update == HallValue_10ms_update)&&
            (HallValue_10ms_update == HallValue_15ms_update)&&
            (HallValue_15ms_update == HallValue_20ms_update)&&
            (HallValue_20ms_update == HallValue_25ms_update)&&
            (HallValue_25ms_update == HallValue_30ms_update)&&
            (HallValue_30ms_update == HallValue_1ms_update)
            )
    {
        Motor_Status = 0 ;
    }   
    else 
    {
        Motor_Status = 1 ; 
        
    }
    
    PI_ms_count++ ;
    if (PI_ms_count==30)        
    {
        CalculateDC();
        PI_ms_count = 0 ;
    }
//    CalculateDC();

}

void ADC_CallBack(void){
    
    V_pot = ADC1_Channel0ConversionResultGet();
    ADC1_Ch1 = ADC1_Channel1ConversionResultGet();
    ADC1_Ch2 = ADC1_Channel2ConversionResultGet();
    ADC1_Ch3 = ADC1_Channel3ConversionResultGet();
    
    X2CScope_Update(); 
}

void IOC_CallBack(void) {

            HallValue = (0x0007 &((HALLA_GetValue()<<2) + (HALLC_GetValue()<<1) + HALLB_GetValue()));
            Hall_A = HALLA_GetValue() ;
            Hall_B = HALLB_GetValue() ;
            Hall_C = HALLC_GetValue() ;
            
            if ((HallValue == 1)&&(Start==1))
            {
                detect_first_circle_count ++ ;
                Cycles_count ++ ;
                if (detect_first_circle_count>1)
                {
                    speed_old_ms_count = speed_ms_count;
                    speed_ms_count = 0 ;
                    detect_first_circle_count = 2 ;                   
                }
                else
                {
                    speed_old_ms_count = 0 ;
                }
                
            }                                                                      
}


void CalculateDC(void)
{
       
    Speed_Error =  Desired_RPM - Current_RPM;

    if (Start == 1)
    {
        Error_Integral = Error_Integral + Speed_Error;
    
        if (Error_Integral > 20000)
        {
            Error_Integral = 0;
        }
        if (Error_Integral < -20000)
        {
            Error_Integral = 0;
        }
        
        DutyCycle_PI = DutyCycle_PI + (int)( (float)kp*Speed_Error*1.000 + (float)(ki*Error_Integral*1.000) );
        
        if (DutyCycle_PI > MAX_DUTY)
        {
            DutyCycle_PI = MAX_DUTY;
        }
         
    }
    else 
    {
        DutyCycle_PI = 0 ;
        Error_Integral = 0;
    }
    
}

void calculateSCurveTime() 
{
    //RPM_TO_RPmS_SCALE = 60000
    // Calculate time parameters
    T1 = V_MAX / A_MAX;    // 4000/2 = 2000 2s
    T3 = T1;

    // Calculate distance parameters
    C1 = (uint16_t)(0.5*A_MAX*T1*T1/RPM_TO_RPmS_SCALE);  // = 0.5*2*2000*2000/60000 = 66 (cycles)
    C3 = C1;
    C2 = CYCLES_ROTATE - (C1+C3); // = 2000-66*2 = 1868

    // Calculate time for constant velocity phase
    T2 = (uint16_t)(C2*RPM_TO_RPmS_SCALE/V_MAX);

    // Calculate total time
    T_Total = T1 + T2 + T3;

}

void calculateSCurveMotion_()
{
    if (Cycles_count<=C1)
    {
        S_Curve_RPM = S_Curve_RPM + A_MAX ;
    }
    else if (Cycles_count<=(C1+C2))
    {
        S_Curve_RPM = V_MAX ;
    }
    else if (Cycles_count<=(C1+C2+C3))
    {
        S_Curve_RPM = S_Curve_RPM - A_MAX ;
    }
    else
    {
        S_Curve_RPM = S_Curve_RPM ;
    }        
}

void calculateSCurveMotion()
{
    if (T_Motion<=T1)
    {
        S_Curve_RPM = A_MAX*T_Motion ;
    }
    else if (T_Motion<=(T1+T2))
    {
        S_Curve_RPM = V_MAX ;
    }
    else if (T_Motion<=T_Total)
    {
        S_Curve_RPM = A_MAX*(T_Total-T_Motion) ;
    }
    else
    {
        S_Curve_RPM = S_Curve_RPM ;
    }        
}
/**
 End of File
*/