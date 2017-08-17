/*********************************************************************************
* File Name: main.c
* Version 0.4
*
* Description:
*  Main program to use PSoC 5LP as an electrochemcial device
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <project.h>
#include "stdio.h"
#include "stdlib.h"
// local files
#include "calibrate.h"
#include "DAC.h"
#include "globals.h"
#include "helper_functions.h"
#include "lut_protocols.h"
#include "usb_protocols.h"
#include "user_selections.h"

// #define Work_electrode_resistance 1400  // ohms, estimate of resistance from SC block to the working electrode pin

// hack to send small messages
//union small_data_usb_union {
//    uint8 usb[64];
//    int16 data[32];
//};
//union small_data_usb_union amp_union;

/* make buffers for the USB ENDPOINTS */
uint8 IN_Data_Buffer[MAX_NUM_BYTES];
uint8 OUT_Data_Buffer[MAX_NUM_BYTES];

char LCD_str[32];  // buffer for LCD screen, make it extra big to avoid overflow
char usb_str[64];  // buffer for string to send to the usb

uint8 Input_Flag = false;  // if there is an input, set this flag to process it
uint8 AMux_channel_select = 0;  // Let the user choose to use the two electrode configuration (set to 0) or a three
// electrode configuration (set to 1) by choosing the correct AMux channel

uint8 adc_recording_channel = 0;
uint16 lut_length = 3000;  // how long the look up table is,initialize large so when starting isr the ending doesn't get triggered
//uint16 lut_hold = 0;  // for debugging
uint8 adc_hold;  // value to hold what adc buffer was just filled
// uint8 counter = 0;  // for debug
uint16 buffer_size_bytes;  // number of bytes of data stored to export for amperometry experiments
// for amperometry experiments, how many data points to save before exporting the adc buffer
uint16 buffer_size_data_pts = 4000;  // prevent the isr from firing by initializing to 4000
uint16 dac_value_hold = 0;


CY_ISR(dacInterrupt)
{
    
    DAC_SetValue(lut_value);
    lut_index++;
    dac_value_hold = lut_value;  // For debug
    if (lut_index >= lut_length) { // all the data points have been given
        isr_adc_Disable();
        isr_dac_Disable();
        //LCD_Position(1,0);
        //sprintf(LCD_str, "e2:%d|%d", lut_index, lut_length);
        //LCD_PrintString(LCD_str);
        ADC_array[0].data[lut_index] = 0xC000;  // mark that the data array is done
        helper_HardwareSleep();
        lut_index = 0; 
        USB_Export_Data((uint8*)"Done", 5); // calls a function in an isr but only after the current isr has been disabled

    }
    lut_value = waveform_lut[lut_index];
}
CY_ISR(adcInterrupt){
    ADC_array[0].data[lut_index] = ADC_SigDel_GetResult16(); 
    //ADC_array[0].data[lut_index] = lut_value;
    //ADC_array[0].data[lut_index] = dac_value_hold;
}

CY_ISR(adcAmpInterrupt){
    ADC_array[adc_recording_channel].data[lut_index] = ADC_SigDel_GetResult16(); 
    lut_index++;  
    if (lut_index >= buffer_size_data_pts) {
        ADC_array[adc_recording_channel].data[lut_index] = 0xC000;
        // counter += 1;  // for debug
        lut_index = 0;
        adc_hold = adc_recording_channel;
        adc_recording_channel = (adc_recording_channel + 1) % ADC_CHANNELS;
        
        sprintf(usb_str, "Done%d", adc_hold);  // tell the user the data is ready to pick up and which channel its on
        USB_Export_Data((uint8*)usb_str, 6);  // use the 'F' command to retreive the data
    }
}

int main()
{
    /* Initialize all the hardware and interrupts */
    CyGlobalIntEnable; 
    
    USBFS_Start(0, USBFS_DWR_VDDD_OPERATION);  // initialize the USB
    helper_HardwareSetup();
    while(!USBFS_bGetConfiguration());  //Wait till it the usb gets its configuration from the PC ??
    
    isr_dac_StartEx(dacInterrupt);
    isr_dac_Disable();  // disable interrupt until a voltage signal needs to be given
    isr_adc_StartEx(adcInterrupt);
    isr_adc_Disable();
    
    USBFS_EnableOutEP(OUT_ENDPOINT);  // changed
    isr_adcAmp_StartEx(adcAmpInterrupt);
    isr_adcAmp_Disable();
    
    CyWdtStart(CYWDT_1024_TICKS, CYWDT_LPMODE_NOCHANGE);
    
    //helper_Writebyte_EEPROM(0, VDAC_ADDRESS);
    
    for(;;) {
        CyWdtClear();
        if(USBFS_IsConfigurationChanged()) {  // if the configuration is changed reenable the OUT ENDPOINT
            while(!USBFS_GetConfiguration()) {  // wait for the configuration with windows / controller is updated
            }
            USBFS_EnableOutEP(OUT_ENDPOINT);  // reenable OUT ENDPOINT
        }
        if (Input_Flag == false) {  // make sure any input has already been dealt with
            Input_Flag = USB_CheckInput(OUT_Data_Buffer);  // check if there is a response from the computer
        }
        
        if (Input_Flag == true) {
            switch (OUT_Data_Buffer[0]) { 
                
            case EXPORT_STREAMING_DATA: ; // 'F' User wants to export streaming data         
                uint8 user_ch1 = OUT_Data_Buffer[1]-'0';
                USB_Export_Data(&ADC_array[user_ch1].usb[0], buffer_size_bytes); 
                break;
                
            case EXPORT_ADC_ARRAY: ; // 'E' User wants to export the data, the user can choose what ADC array to export
                uint8 user_ch = OUT_Data_Buffer[1]-'0';
                if (user_ch <= ADC_CHANNELS) { // check for buffer overflow
                    // 2*(lut_length+2) because the data is 2 times as long as it has to 
                    // be sent as 8-bits and the data is 16 bit, +1 is for the 0xC000 finished signal
                    USB_Export_Data(&ADC_array[user_ch].usb[0], 2*(lut_length+1));  
                }
                else {
                    USB_Export_Data((uint8*)"Error Exporting", 16);
                }
                break;
            case CALIBRATE_TIA_ADC: ; // 'B' calibrate the TIA / ADC current measuring circuit
                calibrate_TIA();
                break;
            case SET_PWM_TIMER_COMPARE: ;  // 'C' change the compare value of the PWM to start the adc isr
                PWM_isr_WriteCompare(helper_Convert2Dec(&OUT_Data_Buffer[2], 5));
                break;
            case SET_PWM_TIMER_PERIOD: ; // 'T' Set the PWM timer period
                user_set_isr_timer(OUT_Data_Buffer);
                break;
            case SET_TIA_ADC: ;  // 'A' Set the TIA resistor, ADC gain and if external resistor is used
                user_setup_TIA_ADC(OUT_Data_Buffer);
                break;
            case CHECK_VOLTAGE_SOURCE: ;  // 'V' check if the device should use the dithering VDAC of the VDAC
                user_voltage_source_funcs(OUT_Data_Buffer);
                break;
            case START_CYCLIC_VOLTAMMETRY: ;  // 'R' Start a cyclic voltammetry experiment
                user_start_cv_run();
                break;
            case RESET_DEVICE: ; // 'X' reset the device by disabbleing isrs
                user_reset_device();
                break;
            case DEVICE_IDENTIFY: ;  // 'I' identify the device 
                user_identify();
                break;
            case CHANGE_NUMBER_ELECTRODES: ; // 'L' User wants to change the electrode configuration
                AMux_channel_select = helper_Convert2Dec(&OUT_Data_Buffer[2], 1) - 2; // user sends 2 or 3 for the # electrode 
                //config, map this to 0 or 1 for the channel the AMux should select
                AMux_electrode_Select(AMux_channel_select);
                break;

            case CHRONOAMPEROMETRY_HACK: ;  // 'Q' Hack to let the device to run a chronoamperometry experiment, not working properly yet
                lut_length = user_chrono_lut_maker(OUT_Data_Buffer);
                break;
            case MAKE_LOOK_UP_TABLE: ; // 'S' make a look up table (lut) for a cyclic voltammetry experiment
                lut_length = user_lookup_table_maker(OUT_Data_Buffer);
                break; 
            case SET_DAC_VALUE: ; // 'D' set the dac value
                DAC_SetValue(helper_Convert2Dec(&OUT_Data_Buffer[2], 4));
                break;
            case RUN_AMPEROMETRY: ; // 'M' run an amperometric experiment

                adc_recording_channel = 0;
                buffer_size_data_pts = user_run_amperometry(OUT_Data_Buffer);
                buffer_size_bytes = 2*(buffer_size_data_pts + 1); // add 1 bit for the termination code and double size for bytes from uint16 data
                break;
            case START_HARDWARE: ; // 'H' Start all of the hardware, used to start ASV run
                helper_HardwareWakeup();
                
            case SHORT_TIA: ;  // 's' user wants to short the TIA
                
                AMux_TIA_input_Connect(2);
                
            case STOP_SHORTING_TIA: ;  // 'd' user wants to stop shorting the TIA
                AMux_TIA_input_Disconnect(2);

            }  // end of switch statment
            OUT_Data_Buffer[0] = '0';  // clear data buffer cause it has been processed
            Input_Flag = false;  // turn off input flag because it has been processed
        }
    }  // end of for loop in main
    
}  // end of main

/* [] END OF FILE */
