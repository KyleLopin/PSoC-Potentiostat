/*******************************************************************************
* File Name: lut_protocols.c
*
* Description:
*  This file contains the protocols to create look up tables
*
**********************************************************************************
 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include "lut_protocols.h"
#include "globals.h"
//extern char LCD_str[];  // for debug

/******************************************************************************
* Function Name: LUT_MakeTriangleWave
*******************************************************************************
*
* Summary:
*  Fill in the look up table (waveform_lut) for the DACs to perform a cyclic voltammetry experiment.
*  Start the CV protocol at the user defined start value
*
* Parameters:
*  uint16_t start_value: first value to put in the dac
*  uint16_t end_value: to peak dac value
*
* Return:
*  uint16: how long the look up table is
*
* Global variables:
*  waveform_lut: Array the look up table is stored in
*
*******************************************************************************/

uint16_t LUT_MakeTriangle_Wave(uint16_t start_value, uint16_t end_value) {
    uint16_t _lut_index = 0;  // start at the beginning of the lut
    
    //LCD_Position(1,0);
    //LCD_PrintDecUint16(end_value);
    printf("p: %i\n", _lut_index);
    
    _lut_index = LUT_make_line(start_value, end_value, 0);
    printf("z: %i\n", _lut_index);
    
    _lut_index = LUT_make_line(end_value, start_value, _lut_index-1);
    printf("v: %i\n", _lut_index);
    waveform_lut[_lut_index] = start_value;  // the DAC is changed before the value is checked in the isr so it will go 1 over so make it stay at last voltage
    _lut_index++;
    //LCD_PutChar('|');
    //LCD_PrintDecUint16(_lut_index);
    printf("%i\n", _lut_index);
    //LCD_PutChar('|');
    return _lut_index;  
}

int LUT_test() {
    return 1;
}



/******************************************************************************
* Function Name: LUT_MakeCVStartZero
*******************************************************************************
*
* Summary:
*  Fill in the look up table (waveform_lut) for the DACs to perform a cyclic voltammetry experiment.
*  Start the CV protocol at 0 volts
*
* Parameters:
*  uint16_t start_value: first value the dac goes after it starts at 0 V
*  uint16_t end_value: the dac value to go to after going to start_value, dac then goes to 0 V (e.g. virtual ground)
*
* Return:
*  uint16: how long the look up table is
*
* Global variables:
*  waveform_lut: Array the look up table is stored in
*
*******************************************************************************/

uint16_t LUT_MakeCVStartZero(const uint16_t start_value, const uint16_t end_value){
    uint16_t _lut_index = 0;  // start at the beginning of the lut
    
    //LCD_Position(1,0);
    //LCD_PrintDecUint16(end_value);
    
    _lut_index = LUT_make_line(dac_ground_value, start_value, 0);
    
    _lut_index = LUT_make_line(start_value, end_value, _lut_index-1);
    _lut_index = LUT_make_line(end_value, dac_ground_value, _lut_index-1);
    waveform_lut[_lut_index] = dac_ground_value;  // the DAC is changed before the value is checked in the isr so it will go 1 over so make it stay at virtual ground
    _lut_index++;
    //LCD_PutChar('|');
    //LCD_PrintDecUint16(_lut_index);
    //LCD_PutChar('|');
    return _lut_index;  
}

/******************************************************************************
* Function Name: LUT_make_side
*******************************************************************************
*
* Summary:
*  Make a ramp from start to end in waveform_lut starting at index
*  Does not matter if start or end is higher
*
* Parameters:
*  uint16_t start: first value to put in the look up table
*  uint16_t end: end value to put in the look up table
*  uint16_t index: the place to start putting in numbers in the look up table
*
* Return:
*  uint16: first place after the filled in area of the look up table
*
* Global variables:
*  waveform_lut: Array the look up table is stored in
*
*******************************************************************************/

uint16_t LUT_make_line(uint16_t start, uint16_t end, uint16_t index) {
    printf("start: %i, end: %i\n", start, end);
    if (start < end) {
        for (int16_t value = start; value <= end; value++) {
            waveform_lut[index] = value;
            index ++;
            printf("l: %i, %i\n", index, value);
            if (index >= 4000) {
                return index;
            }
        }
    }
    else {
        for (int16_t value = start; value >= end; value--) {
            waveform_lut[index] = value;
            index ++;
            printf("b: %i, %i\n", index, value);
            if (index >= 4000) {
                return index;
            }
        }
    }
    printf("m: %i\n", index);
    return index;
}

/******************************************************************************
* Function Name: LUT_make_swv_side
*******************************************************************************
*
* Summary:
*  Make a ramp with a square wave super imposed, from start to end in waveform_lut 
*  starting at index
*  Does not matter if start or end is higher
*
* Parameters:
*  uint16_t start: first value to put in the look up table
*  uint16_t end: end value to put in the look up table
*  uint16_t pulse_inc: increment between the square pulse steps
*  uint16_t pulse_height: height of each square wave pulse
*  uint16_t index: the place to start putting in numbers in the look up table
*
* Return:
*  uint16: first place after the filled in area of the look up table
*
* Global variables:
*  waveform_lut: Array the look up table is stored in
*
*******************************************************************************/

uint16_t LUT_make_swv_line(uint16_t start, uint16_t end, uint16_t pulse_inc, 
                         uint16_t pulse_height, uint16_t index) {
    printf("start: %i, end: %i\n", start, end);
    if (index > 4000) {
        return index;
    }
    if (start < end) {
        for (int16_t value = start; value <= end; value+=pulse_inc) {
            waveform_lut[index] = value + pulse_height;
            index ++;
            waveform_lut[index] = value - pulse_height;
            index ++;
            printf("l: %i, %i\n", index, value);
            if (index > 4000) {
                break;
            }
        }
    }
    else {
        for (int16_t value = start; value >= end; value-=pulse_inc) {
            waveform_lut[index] = value + pulse_height;
            index ++;
            waveform_lut[index] = value - pulse_height;
            index ++;
            printf("b: %i, %i\n", index, value);
            if (index > 4000) {
                break;
            }
        }
    }
    printf("m: %i\n", index);
    return index;
}

/******************************************************************************
* Function Name: LUT_make_dpv
*******************************************************************************
*
* Summary:
*  Make a Differential Pulse Voltammetry (DPV) look up table profile
*
* Parameters:
*  uint16_t start: starting dac value
*  uint16_t end: value the dac will end with, not including the pulse dac settings
*  uint16_t index: the place to start putting in numbers in the look up table
*  uint16_t height: height of the DPV pulse
*  uint16_t increment: increment of each step
*
* Return:
*  uint16: first place after the filled in area of the look up table
*
* Global variables:
*  waveform_lut: Array the look up table is stored in
*
*  NOTES: NOT FINISHED JUST TO RUN UPWARDS DPV AFTER ASV
*
*******************************************************************************/

uint16_t LUT_make_dpv(uint16_t start, uint16_t end, uint16_t height, 
                    uint16_t increment, uint16_t index) {
    uint16_t level = start;  // variable to store currend dac values in 
    
    waveform_lut[index] = level;
    while (level >= end) {  // >= because the voltages are inverted compared to the dac
        index += 1;
        waveform_lut[index] = level - height - increment;  // negative bec
        index += 1;
        waveform_lut[index] = level - increment;
        level = waveform_lut[index];
    }
//    LCD_ClearDisplay();
//    
//    sprintf(LCD_str, "%d|%d|%d", waveform_lut[0], waveform_lut[1], waveform_lut[2]);
//    LCD_PrintString(LCD_str);
//    LCD_Position(1, 0);
    
//    sprintf(LCD_str, "%d|%d|%d", waveform_lut[3], waveform_lut[4], waveform_lut[5]);
//    LCD_PrintString(LCD_str);
    
    return index;
}

/******************************************************************************
* Function Name: LUT_MakePulse
*******************************************************************************
*
* Summary:
*  Make a look up table that stores a square pulse sequence
*  Quick hack for chronoamperomerty
*  TODO: THIS SHOULD BE REPLACED
*
* Parameters:
*  uint16_t base: value to be placed in the DAC to maintain the baseline potential
*  uint16_t pulse: value to put in the DAC for the voltage pulse
*
* Global variables:
*  waveform_lut: Array the look up table is stored in
*
*******************************************************************************/

void LUT_MakePulse(uint16_t base, uint16_t pulse) {
    int _lut_index = 0;
    while (_lut_index < 1000) {
        waveform_lut[_lut_index] = base;
        _lut_index++;
    }
    while (_lut_index < 2000) {
        waveform_lut[_lut_index] = pulse;
        _lut_index++;
    }
    while (_lut_index < 4000) {
        waveform_lut[_lut_index] = base;
        _lut_index++;
    }
}

/* [] END OF FILE */
