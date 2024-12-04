 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/

/*This main project lets PIC18 adjust the PWM duty cycle according to the
 * potential meter value, and print the ADC value on the terminal of Data Visualizer
 * at the same time
 * 
 * NOTE: The following pin notes may not be the latest. 
 * Check MCC Pin Grid View for confirmation
 * PWM Output(CCP1): C2
 * Analog Input: A0 
*/
#include "mcc_generated_files/system/system.h"

#define DUTYCYCLE_RATIO_TO_CCPR_CONVERT(P)  (uint16_t)((double)(P)*4*(T2PR+1))

/* Test purpose duty-cycle ratio */
#define DUTYCYCLE_RATIO_MAX                     0.75
#define DUTYCYCLE_RATIO_MIN                     0.25
#define DUTYCYCLE_RATIO_STEP                    0.25

/**
 *  The max and min value of potential meter 
 *  Need to test on terminal print to get the value*/
#define POT_MIN 0
#define POT_MAX 3800


double PWM_Stepper(double duty_ratio)
{
    if (duty_ratio < DUTYCYCLE_RATIO_MAX)
        duty_ratio += DUTYCYCLE_RATIO_STEP;
    else
        duty_ratio = DUTYCYCLE_RATIO_MIN;
    
    CCP1_LoadDutyValue(DUTYCYCLE_RATIO_TO_CCPR_CONVERT(duty_ratio));
    return duty_ratio;
}

// Map the voltage of potential meter to the duty cycle ratio
double mapValue(uint16_t pot_val) {
    return DUTYCYCLE_RATIO_MIN + (pot_val - POT_MIN) * (DUTYCYCLE_RATIO_MAX - DUTYCYCLE_RATIO_MIN) / (POT_MAX - POT_MIN);
}


int main(void)
{/* Initialize the device */
    SYSTEM_Initialize();
    double duty_cycle_ratio = DUTYCYCLE_RATIO_MIN;  /* expressed in percents */
    volatile uint16_t adcResult; //Create variable to hold ADC result

    while (1)
    {
        ADC_SampleCapacitorDischarge();
        adcResult = ADC_ChannelSelectAndConvert(ADC_CHANNEL_ANA0);
        IO_LED_Toggle();
        IO_Debug_Toggle();
        (void) printf("The ADC Result is: %d\r\n", adcResult);
        
//        duty_cycle_ratio = mapValue(adcResult);
        CCP1_LoadDutyValue(DUTYCYCLE_RATIO_TO_CCPR_CONVERT(0.75));

        __delay_ms(300);
    }
}