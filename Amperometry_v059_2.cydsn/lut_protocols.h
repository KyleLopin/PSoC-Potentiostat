/*******************************************************************************
* File Name: lut_protocols.h
*
* Description:
*  This file contains the function prototypes and constants used for
*  the protocols to create look up tables
*
**********************************************************************************
 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/
#if !defined(LUT_PROTOCOLS_H)
#define LUT_PROTOCOLS_H

//#include <project.h>
#include "stdlib.h"  // remove after testing
#include "stdio.h"  // remove after testing
    
// Local files
#include "globals.h"
#include "helper_functions.h"

/***************************************
*        Function Prototypes
***************************************/    
uint16_t LUT_MakeCVStartZero(uint16_t start_value, uint16_t end_value);
uint16_t LUT_MakeTriangle_Wave(uint16_t start_value, uint16_t end_value);

uint16_t LUT_MakeCVStartZero_SWV(uint16_t start_value, uint16_t end_value,
                                 uint16_t swv_height, uint16_t swv_inc);
uint16_t LUT_MakeTriangle_Wave_SWV(uint16_t start_value, uint16_t end_value,
                                   uint16_t swv_height, uint16_t swv_inc);

void LUT_MakePulse(uint16_t base, uint16_t pulse);
uint16_t LUT_make_line(uint16_t start, uint16_t end, uint16_t index);
uint16_t LUT_make_dpv(uint16_t start, uint16_t end, uint16_t height, 
                    uint16_t increment, uint16_t index);
uint16_t LUT_make_swv_line(uint16_t start, uint16_t end, uint16_t pulse_inc,
                         uint16_t pulse_height, uint16_t index);
struct RunParams LUT_make_run_params(const uint8_t data_buffer[], struct RunParams *run_params);
uint16_t LUT_Convert2Dec(const uint8_t array[], const uint8_t len);


/***************************************
* Global variables external identifier
***************************************/
extern uint16_t lut_value;  // value need to load DAC
extern uint16_t waveform_lut[];
extern uint16_t dac_ground_value;  // value to load in the DAC

    
#endif

/* [] END OF FILE */
