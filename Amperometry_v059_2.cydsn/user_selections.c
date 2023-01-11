/*******************************************************************************
* File Name: user_selections.c
*
* Description:
*  Make some of functions the user can call through the USB. 
*  Only for the longer functions to keep main clearer
*
**********************************************************************************
 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include <project.h>
#include "stdio.h"  // gets rid of the type errors

#include "user_selections.h"
extern char LCD_str[];  // for debug


/******************************************************************************
* Function Name: user_setup_TIA_ADC
*******************************************************************************
*
* Summary:
*  Set up the TIA and ADC circuit by changing the TIA resistor value, the ADC gain
*    setup if the user wants to use an external resistor
*  Parameters:
*  uint8 data_buffer[]: array of chars with the parameters to set the adc and tia components
*  input is A|U|X|Y|Z|W: where 
*  U is ADC configuration to use where config 1 uses a Vref of +-2.048 V and config 2 uses +-1.024 V
*  X - the TIA resistor value index, a string between 0-7 that sets the TIA resisot value
*  Y - the adc buffer gain setting
*  Z - T or F for if an external resistor is to be used and the AMux_working_electrode should be set according
*  W - 0 or 1 for which user resistor should be selected by AMux_working_electrode
*  
* Global variables: - found in calibrate.h and used by calibrate.c
*  uint8 TIA_resistor_value_index: index of whick TIA resistor to use, Supplied by USB input
*  uint8 ADC_buffer_index: which ADC buffer is used, gain = 2**ADC_buffer_index
*
* Return:
*  array of 20 bytes is loaded into the USB in endpoint (into the computer)
*
*******************************************************************************/

void user_setup_TIA_ADC(uint8_t data_buffer[]) {
    uint8_t adc_config = data_buffer[2]-'0';
    if (adc_config == 1 || adc_config == 2) {
        ADC_SigDel_SelectConfiguration(adc_config, DO_NOT_RESTART_ADC); 
    }
    TIA_resistor_value_index = data_buffer[4]-'0';
    if (TIA_resistor_value_index >= 0 || TIA_resistor_value_index <= 7) {
        TIA_SetResFB(TIA_resistor_value_index);  // see TIA.h for how the values work, basically 0 - 20k, 1 -30k, etc.
    }
    ADC_buffer_index = data_buffer[6]-'0';
    if (ADC_buffer_index >= 0 || ADC_buffer_index <= 3) {
        ADC_SigDel_SetBufferGain(ADC_buffer_index); 
    }
    if (data_buffer[8] == 'T') {
        tia_mux.use_extra_resistor = true;
        tia_mux.user_channel = data_buffer[10]-'0';  // not used yet
        data_buffer[8] = 0;
        AMux_TIA_resistor_bypass_Connect(0);
    }
    else {
        AMux_TIA_resistor_bypass_Disconnect(0);
    }
}


void user_export_lut(uint8_t data_buffer[]) {
    uint16_t length = LUT_Convert2Dec(&data_buffer[2], 4);
    USB_Export_Data(waveform_lut, length);
}


/******************************************************************************
* Function Name: user_voltage_source_funcs
*******************************************************************************
*
* Summary:
*  Check if the user wants to read what type of DAC is to be used or the user can set the DAC to be used

* Parameters:
*  uint8 data_buffer[]: array of chars used to setup the DAC or to read the DAC settings
*  input is VXY: where X is either 'R' or 'S' for read or set
*  Y is '0' or '1'
*
* Return:
*  export the DAC information to the USB, or set the DAC source depending on user input
*
*******************************************************************************/

void user_voltage_source_funcs(uint8_t data_buffer[]) {
    if (data_buffer[1] == 'R') {  // User wants to read status of VDAC
        uint8_t export_array[2];
        export_array[0] = 'V';
        export_array[1] = helper_check_voltage_source();
        USB_Export_Data(export_array, 2);
    }
    else if (data_buffer[1] == 'S') {  // User wants to set the voltage source
        helper_set_voltage_source(data_buffer[2]-'0');
        DAC_Start();
        DAC_Sleep();
    }
}

/******************************************************************************
* Function Name: user_start_cv_run
*******************************************************************************
*
* Summary:
*  Start a cyclic voltammetry experiment.  The look up table in waveform_lut should
*  already be created.  If the dac isr is already running this will not start and throws
*  and error through the USB.  
*  
* Global variables:
*  uint16_t lut_value: value gotten from the look up table that is to be applied to the DAC
*  uint16_t lut_index: current index of the look up table
*  uint16_t waveform_lut[]:  look up table of the waveform to apply to the DAC
*
* Parameters:
*  None
*
* Return:
*  Starts the isr's used to perform an experiment else if the dac is already running,
*  possibly because another experiment is already running, return an error through the USB
*
*******************************************************************************/

void user_start_cv_run(void){
    if (!isr_dac_GetState()){  // enable the dac isr if it isnt already enabled
        if (isr_adcAmp_GetState()) {  // User has started cyclic voltammetry while amp is already running so disable amperometry
            isr_adcAmp_Disable();
        }
        lut_index = 0;  // start at the beginning of the look up table
        lut_value = waveform_lut[0];
        helper_HardwareWakeup();  // start the hardware
        DAC_SetValue(lut_value);  // let the electrode equilibriate
        CyDelay(20);  // let the electrode voltage settle
        ADC_SigDel_StartConvert();  // start the converstion process of the delta sigma adc so it will be ready to read when needed
        CyDelay(10);
        PWM_isr_WriteCounter(100);  // set the pwm timer so that it will trigger adc isr first
        ADC_array[0].data[lut_index] = ADC_SigDel_GetResult16();  // Hack, get first adc reading, timing element doesn't reverse for some reason
        
        DAC_SetValue(lut_value);  // let the electrode equilibriate
        
        CyDelay(20);  // let the electrode voltage settle
        
        isr_dac_Enable();  // enable the interrupts to start the dac
        isr_adc_Enable();  // and the adc
    }
    else {  // if another experiment is running, throw an error
        USB_Export_Data((uint8_t*)"Error1", 7);
    }
}

/******************************************************************************
* Function Name: user_reset_device
*******************************************************************************
*
* Summary:
*  Stop all operations by disabling all the isrs and reset the look up table index to 0
*  
* Global variables:
*  uint16_t lut_index: current index of the look up table
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

void user_reset_device(void) {
    isr_dac_Disable();
    isr_adc_Disable();
    isr_adcAmp_Disable();
    helper_HardwareSleep();
    
    lut_index = 0;  
}

/******************************************************************************
* Function Name: user_identify
*******************************************************************************
*
* Summary:
*  identify the device and test if the usb is working properly, this is what the program sends at the beginning
*  disable all interruptsincase the program has restarted so the device will also reset
*
* Parameters:
*  None
*
* Return:
*  which version of the device is attached to the USB port
*
*******************************************************************************/

void user_identify(void) {
    isr_adcAmp_Disable();
    isr_adc_Disable();
    isr_dac_Disable();
    USB_Export_Data((uint8_t*)"Naresuan University", 19);
    // TODO:  Put in a software reset incase something goes wrong the program can reattach
}

/******************************************************************************
* Function Name: user_set_isr_timer
*******************************************************************************
*
* Summary:
*  Set the PWM that is used as the isr timer
* 
* Parameters:
*  uint8 data_buffer[]: array of chars used to setup the DAC or to read the DAC settings
*  input is T|XXXXX
*  XXXXX - uint16_t with the number to put in the period register
*  the compare register will be loaded with XXXXX / 2
*  
* Return:
*  Set the compare and period register of the pwm 
*
*******************************************************************************/


void user_set_isr_timer(uint8_t data_buffer[]) {
    PWM_isr_Wakeup();
    uint16_t timer_period = LUT_Convert2Dec(&data_buffer[2], 5);
    PWM_isr_WriteCompare(timer_period / 2);  // not used in amperometry run so just set in the middle
    PWM_isr_WritePeriod(timer_period);
    PWM_isr_Sleep();
}

/******************************************************************************
* Function Name: user_chrono_lut_maker
*******************************************************************************
*
* Summary:
*  Make a look up table that will run a chronoamperometry experiment.  Hackish now
* 
* Parameters:
*  uint8 data_buffer[]: array of chars used to make the look up table
*  input is Q|XXXX|YYYY|ZZZZZ: 
*  XXXX - uint16_t with the number to put in the DAC for the baseline voltage and the 
*  voltage to be applied after the pulse.
*  YYYY - uint16_t with the number to put in the dac during the pulse of the experiment
*  ZZZZZ - uint16_t to put in the period of the PWM timer to set the sampling rate
*  
* Global variables:
*  uint16_t lut_value: value gotten from the look up table that is to be applied to the DAC
*  uint16_t waveform_lut[]:  look up table of the waveform to apply to the DAC
*  
* Return:
*  4000 - how long the look up table will be
*
*******************************************************************************/

uint16_t user_chrono_lut_maker(uint8_t data_buffer[]) {
    PWM_isr_Wakeup();
    uint16_t baseline = LUT_Convert2Dec(&data_buffer[2], 4);
    uint16_t pulse = LUT_Convert2Dec(&data_buffer[7], 4);
    uint16_t timer_period = LUT_Convert2Dec(&data_buffer[12], 5);
                
    PWM_isr_WritePeriod(timer_period);
                
    LUT_MakePulse(baseline, pulse);
    lut_value = waveform_lut[0];  // setup the dac so when it starts it will be at the correct voltage
                
    PWM_isr_Sleep();
    return 4000; // the look up table length will be 4000
}





/******************************************************************************
* Function Name: user_dpv_lut_maker
*******************************************************************************
*
* Summary:
*  Make a look up table that will run a differential pulse experiment.  Hackish now
* 
* Parameters:
*  uint8 data_buffer[]: array of chars used to make the look up table
*  input is G|XXXX|YYYY|UUU|VVV|ZZZZZ|AB: 
*  XXXX  - uint16_t the number to put in the DAC for the baseline voltage 
*  YYYY  - uint16_t the dac value to stop at
*  UUU   - uint16_t the increase in the dac value for the pulse
*  VVV   - uint16_t the pulse increment of the DPV protocol
*  ZZZZZ - uint16_t to put in the period of the PWM timer to set the sampling rate
*  A - char of 'L' or 'C' to make a linear sweep ('L') or a cyclic voltammetry ('C')
*      look up table
*  B - char of 'Z' or 'S' to start the waveform at 0 Volts ('Z') or at the value 
*      entered in the XXXX field
*  
* Global variables:
*  uint16_t lut_value: value gotten from the look up table that is to be applied to the DAC
*  uint16_t waveform_lut[]:  look up table of the waveform to apply to the DAC
*  
* Return:
*  index - how far in the look up table that has been filled
*
*******************************************************************************/
// TODO: move the hardware parts out
uint16_t user_dpv_lut_maker(uint8_t data_buffer[]) {
    PWM_isr_Wakeup();
    uint16_t start = LUT_Convert2Dec(&data_buffer[INDEX_START_VALUE], 4);
    uint16_t end = LUT_Convert2Dec(&data_buffer[INDEX_END_VALUE], 4);
    uint16_t pulse_height = LUT_Convert2Dec(&data_buffer[INDEX_SWV_PULSE_HEIGHT], 4);
    uint16_t pulse_inc = LUT_Convert2Dec(&data_buffer[INDEX_SWV_INC], 4);

    uint8_t sweep_type = data_buffer[INDEX_SWV_SWEEP_TYPE];
    uint8_t start_volt_type = data_buffer[INDEX_SWV_START_VOLT_TYPE];
    //LCD_ClearDisplay();
    
    //sprintf(LCD_str, "%d|%d|%d|%d", start, end, pulse_height, pulse_inc);
    //sprintf(LCD_str, "%.*s", 16, data_buffer);
    //LCD_PrintString(LCD_str);
    uint16_t timer_period = LUT_Convert2Dec(&data_buffer[INDEX_SWV_TIMER_VALUE], 5);
//                
    PWM_isr_WritePeriod(timer_period);
    uint16_t lut_length = 0;
    if (sweep_type == 'L') {
        lut_length = LUT_make_swv_line(start, end, pulse_height, pulse_inc, 0);
        waveform_lut[lut_length] = waveform_lut[lut_length-1];  // dac is changed once before end so double last voltage
        lut_length += 1;
    }
    else if (start_volt_type == 'Z') {  // Make a Cyclic voltammetry look up table that starts at 0 volts
        lut_length = LUT_MakeCVStartZero_SWV(start, end, pulse_height, pulse_inc);
    }
    else if (start_volt_type == 'S') {  // Make a Cyclic voltammetry look up table that starts at the first dac value
        lut_length = LUT_MakeTriangle_Wave_SWV(start, end, pulse_height, pulse_inc);
    }
    lut_value = waveform_lut[0];  // setup the dac so when it starts it will be at the correct voltage
                
    PWM_isr_Sleep();
    return lut_length; // the look up table length will be 4000
}


/******************************************************************************
* Function Name: user_lookup_table_maker
*******************************************************************************
*
* Summary:
*  Make a look up table that will a cyclic voltammetry or linear sweep experiment
* 
* Parameters:
*  uint8 data_buffer[]: array of chars used to make the look up table
*  input is S|XXXX|YYYY|ZZZZZ|AB: 
*  XXXX - uint16_t with the starting number to put in the DAC for the experiment
*  YYYY - uint16_t with the ending number to put in the dac for the experiment
*  ZZZZZ - uint16_t to put in the period of the PWM timer to set the sampling rate
*  A - char of 'L' or 'C' to make a linear sweep ('L') or a cyclic voltammetry ('C')
*      look up table
*  B - char of 'Z' or 'S' to start the waveform at 0 Volts ('Z') or at the value 
*      entered in the XXXX field
*  
* Global variables:
*  uint16_t lut_value: value to get from the look up table and apply to the DAC
*  uint16_t waveform_lut[]:  look up table of the waveform to apply to the DAC
*  
* Return:
*  uint16_t lut_length - how many look up table elements there are
*
*******************************************************************************/

uint16_t user_lookup_table_maker(uint8_t data_buffer[]) {
    printf("make look up table\n");
    if (data_buffer[0] == 'G') {
        printf("make look up table for swv\n");
        return user_lookup_table_maker_swv(data_buffer);
    }
    PWM_isr_Wakeup();
    uint16_t start_dac_value = LUT_Convert2Dec(&data_buffer[2], 4);
    uint16_t end_dac_value = LUT_Convert2Dec(&data_buffer[7], 4);
    uint16_t timer_period = LUT_Convert2Dec(&data_buffer[12], 5);
    uint8_t sweep_type = data_buffer[18];
    uint8_t start_volt_type = data_buffer[19];
    PWM_isr_WritePeriod(timer_period);
    uint16_t lut_length = 0;
    printf("user lookup table: %i, %i\n", start_dac_value, end_dac_value);
    if (sweep_type == 'L') {  // Make look up table for linear sweep, ignore start volt type
        // The dac changes once after the run, so hold the voltage at the end constant
        // for 1 more tick
        lut_length = LUT_make_line(start_dac_value, end_dac_value, 0);
        waveform_lut[lut_length] = waveform_lut[lut_length-1];
        lut_length += 1;
    }
    else if (start_volt_type == 'Z') {  // Make a Cyclic voltammetry look up table that starts at 0 volts
        lut_length = LUT_MakeCVStartZero(start_dac_value, end_dac_value);
    }
    else if (start_volt_type == 'S') {  // Make a Cyclic voltammetry look up table that starts at the first dac value
        lut_length = LUT_MakeTriangle_Wave(start_dac_value, end_dac_value);
    } 
    
    
    lut_value = waveform_lut[0];  // Initialize for the start of the experiment
    PWM_isr_Sleep();
    return lut_length;
}


uint16_t user_lookup_table_maker_swv(uint8_t data_buffer[]) {
    uint16_t lut_length = 0;
    uint16_t start_dac_value = LUT_Convert2Dec(&data_buffer[2], 4);
    uint16_t end_dac_value = LUT_Convert2Dec(&data_buffer[7], 4);
    uint16_t swv_inc = LUT_Convert2Dec(&data_buffer[12], 3);
    uint16_t swv_pulse_height = LUT_Convert2Dec(&data_buffer[16], 3);
    uint16_t timer_period = LUT_Convert2Dec(&data_buffer[22], 5);
    uint16_t sweep_type = data_buffer[26];
    uint16_t start_volt_type = data_buffer[27];
    PWM_isr_Wakeup();
    PWM_isr_WritePeriod(timer_period);
    PWM_isr_Sleep();
    printf("start_voltage: %i\n", start_dac_value);
    printf("end voltage: %i\n", end_dac_value);
    printf("inc voltage: %i\n", swv_inc);
    printf("swv pulse voltage: %i\n", swv_pulse_height);
    printf("start_voltage_tupe: %i\n", sweep_type);
    if (sweep_type == 'L') {
        return 205;  // Not implimented yet
    }
    else if (start_volt_type == 'Z') {
         printf("make CV look up table and start at 0V\n");
        lut_length = LUT_MakeCVStartZero_SWV(start_dac_value, end_dac_value, swv_pulse_height, swv_inc);
    }
    
    return lut_length;
}


uint16_t user_lookup_table_make_future(uint8_t data_buffer[]) {
    run_params = LUT_make_run_params(data_buffer, &run_params);

    return 1;
}


/******************************************************************************
* Function Name: user_run_amperometry
*******************************************************************************
*
* Summary:
*  Start an amperometry experiment.  Turn on all the hardware required, set the dac,
*  initialize the look up table index that will be a point where to put the data points
*  in the adc buffer, and start the Delta Sigma ADC to start converting.
* 
* Parameters:
*  uint8 data_buffer[]: array of chars used to make the look up table
*  input is M|XXXX|YYYY
*  XXXX - uint16_t number to set the DAC value to so the electrodes are at the approriate voltage
*  YYYY - uint16_t of how many data points to collect in each ADC buffer before exporting the data
*  
* Global variables:
*  uint16_t lut_index: current index of the look up table
*  
* Return:
*  uint16_t - number of data points to collect in each ADC buffer before exporting the data
*
*******************************************************************************/

uint16_t user_run_amperometry(uint8_t data_buffer[]) {
    helper_HardwareWakeup();  
    if (!isr_adcAmp_GetState()) {  // enable isr if it is not already
        if (isr_dac_GetState()) {  // User selected to run amperometry but a CV is still running 
            isr_dac_Disable();
            isr_adc_Disable();
        }
    }
    uint16_t dac_value = LUT_Convert2Dec(&data_buffer[2], 4);  // get the voltage the user wants and set the dac
    lut_index = 0;
    DAC_SetValue(dac_value);
    
    ADC_SigDel_StartConvert();
    CyDelay(15);
    uint16_t buffer_size_data_pts = LUT_Convert2Dec(&data_buffer[7], 4);  // how many data points to collect in each adc channel before exporting the data
    isr_adcAmp_Enable();
    return buffer_size_data_pts;
}


/* [] END OF FILE */
