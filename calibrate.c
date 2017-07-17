/*******************************************************************************
* File Name: calibrate.c
*
* Description:
*  Protocols to calibrate the current measuring circuitry i.e. TIA / delta sigma ADC with an IDAC
*
**********************************************************************************
 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <project.h>
#include "math.h"
#include <stdio.h>
#include "stdlib.h"

#include "calibrate.h"
#include "usb_protocols.h"

//extern char LCD_str[];  // for debug
const uint16 calibrate_TIA_resistor_list[] = {20, 30, 40, 80, 120, 250, 500, 1000}; 
uint16 static ADC_value;

/***************************************
* Forward function references
***************************************/
static void Calibrate_Hardware_Wakeup(void);
static void calibrate_step(uint16 IDAC_value, uint8 IDAC_index);
static void Calibrate_Hardware_Sleep(void);

/******************************************************************************
* Function Name: calibrate_TIA
*******************************************************************************
*
* Summary:
*  Calibrate the TIA circuit each time the current gain settings are changed
*
* Global variables:
*  uint8 TIA_resistor_value_index: index of whick TIA resistor to use, Supplied by USB input
*  uint8 ADC_buffer_index: which ADC buffer is used, gain = 2**ADC_buffer_index
*
* Return:
*  array of 20 bytes is loaded into the USB IN ENDPOINT (into the computer)
*
*******************************************************************************/

//void calibrate_TIA(uint8 TIA_resistor_value_index, uint8 ADC_buffer_index) {
void calibrate_TIA(void) {
    // The IDAC only 
    
    IDAC_calibrate_Start();
    IDAC_calibrate_SetValue(0);
    // start the hardware required
    Calibrate_Hardware_Wakeup();
    CyDelay(100);
    // decide what currents to use based on TIA resistor and ADC buffer settings
    uint16 resistor_value = calibrate_TIA_resistor_list[TIA_resistor_value_index];
    uint var = 2;
    uint16 IDAC_setting = 0;
    uint8 ADC_buffer_value = pow(var, ADC_buffer_index);
    // set input current to zero and read ADC
    ADC_SigDel_StartConvert();
    calibrate_step(IDAC_setting, 2);
    // calculate the IDAC value needed to get a 1 Volt in the ADC
    // the 8000 is because the IDAC has a 1/8 uA per bit and 8000=1000mV/(1/8 uA per bit)
    float32 transfer = 8000./(ADC_buffer_value*resistor_value);
    int transfer_int = (int) transfer;
    if (transfer_int > 250) {  // the TIA needs too much current, reduce needs by half.  Is needed for the 20k resistor setting
        transfer_int /= 2;
    }
    // is not DRY but not sure how to fix
    IDAC_calibrate_SetPolarity(IDAC_calibrate_SINK);
    calibrate_step(transfer_int, 0);
    calibrate_step(transfer_int/2, 1);
    IDAC_calibrate_SetPolarity(IDAC_calibrate_SOURCE);
    calibrate_step(transfer_int/2, 3);
    calibrate_step(transfer_int, 4);
    IDAC_calibrate_SetValue(0);
    Calibrate_Hardware_Sleep();
    
//    LCD_Position(0,0);
//    sprintf(LCD_str, "in:%d |%d| ", resistor_value, ADC_buffer_value);
//    LCD_PrintString(LCD_str);
        
    USB_Export_Data(calibrate_array.usb, 20);
}

/******************************************************************************
* Function Name: calibrate_step
*******************************************************************************
*
* Summary:
*  Gets a single calibration data point by setting the calibration IDAC and reading
*  the ADC count and saving them in the calibration_array
*
* Parameters:
*  uint16 IDAC_value: value to set the calibration  IDAC to before measuring with the ADC
*  uint8 IDAC_index: index of where in calibration_array to save the IDAC and ADC data
*
* Global variables:
*  calibration_array: array of saved IDAC and ADC values
*
*******************************************************************************/

static void calibrate_step(uint16 IDAC_value, uint8 IDAC_index) {
    IDAC_calibrate_SetValue(IDAC_value);
    CyDelay(100);  // allow the ADC to settle
    ADC_value = ADC_SigDel_GetResult16();
    calibrate_array.data[IDAC_index] = IDAC_value;
    calibrate_array.data[IDAC_index+5] = ADC_value;  // 5 because of the way the array is set up
}

/******************************************************************************
* Function Name: Calibrate_Hardware_Wakeup
*******************************************************************************
*
* Summary:
*  Wakeup all the hardware needed for the calibration routine and set the AMux to 
*  the correct channel
*
*******************************************************************************/

static void Calibrate_Hardware_Wakeup(void) {
    AMux_TIA_input_Select(AMux_TIA_calibrat_ch);
    TIA_Wakeup();
    VDAC_TIA_Wakeup();
    ADC_SigDel_Wakeup();
}

/******************************************************************************
* Function Name: Calibrate_Hardware_Sleep
*******************************************************************************
*
* Summary:
*  Put to sleep all the hardware needed for the calibration routine, stop  
*  the IDAC, and set the AMux to the correct channel
*
*******************************************************************************/

static void Calibrate_Hardware_Sleep(void) {
    AMux_TIA_input_Select(AMux_TIA_measure_ch);
    TIA_Sleep();
    VDAC_TIA_Sleep();
    ADC_SigDel_Sleep();
    IDAC_calibrate_Stop();
}

/* [] END OF FILE */
