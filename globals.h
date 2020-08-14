/*******************************************************************************
* File Name: globals.h
*
* Description:
*  Global variables, unions and structures to use in the project
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(GLOBALS_H)
#define GLOBALS_H
    
#include "cytypes.h"

/**************************************
*        USB INPUT OPTIONS
**************************************/ 
    
#define EXPORT_STREAMING_DATA           'F'
#define EXPORT_ADC_ARRAY                'E'
#define CALIBRATE_TIA_ADC               'B'
#define SET_PWM_TIMER_COMPARE           'C'
#define SET_PWM_TIMER_PERIOD            'T'
#define SET_TIA_ADC                     'A'
#define CHECK_VOLTAGE_SOURCE            'V'
#define START_CYCLIC_VOLTAMMETRY        'R'
#define RESET_DEVICE                    'X'
#define DEVICE_IDENTIFY                 'I'
#define CHANGE_NUMBER_ELECTRODES        'L'
#define CHRONOAMPEROMETRY_HACK          'Q'
#define MAKE_LOOK_UP_TABLE              'S'
#define SET_DAC_VALUE                   'D'
#define RUN_AMPEROMETRY                 'M'
#define START_HARDWARE                  'H'
#define SHORT_TIA                       's'
#define STOP_SHORTING_TIA               'd'
#define DPV_LUT                         'G'
    
/**************************************
*           ADC Constants
**************************************/  
    
// define how big to make the arrays for the lut for dac and how big
// to make the adc data array     
#define MAX_LUT_SIZE 5000
#define ADC_CHANNELS 4
 
    
/**************************************
*           API Constants
**************************************/
#define true                        1
#define false                       0
    
#define VIRTUAL_GROUND              2048  // TODO: make variable

// Define the AMux channels
#define two_electrode_config_ch     0
#define three_electrode_config_ch   1 
#define AMux_V_VDAC_source_ch       0
#define AMux_V_DVDAC_source_ch      1
#define AMux_TIA_working_electrode_ch 1

/**************************************
*        EEPROM API Constants
**************************************/

#define VDAC_NOT_SET 0
#define VDAC_IS_VDAC 1
#define VDAC_IS_DVDAC 2
    
#define VDAC_ADDRESS 0
    
    
/**************************************
*        AMuX API Constants
**************************************/
    
//#define DVDAC_channel 1
//#define VDAC_channel 0
    
    
/**************************************
*        Global Variables
**************************************/   
    
uint16 dac_ground_value;  // value to load in the DAC 
    
/* Make global variables needed for the DAC/ADC interrupt service routines */
uint16 lut_value;  // value need to load DAC
uint16 waveform_lut[MAX_LUT_SIZE];  // look up table to store waveform for variable potential experiments
uint16 lut_index;  // look up table index

/* Global structs */

union data_usb_union {
    uint8 usb[2*MAX_LUT_SIZE];
    int16 data[MAX_LUT_SIZE];
};
union data_usb_union ADC_array[ADC_CHANNELS];  // allocate space to put adc measurements
    
struct TIAMux {  // not used currently
    uint8 use_extra_resistor;
    uint8 user_channel;
};
struct TIAMux tia_mux;


#endif    
/* [] END OF FILE */
