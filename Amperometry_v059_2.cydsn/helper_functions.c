/*******************************************************************************
* File Name: helper_functions.c
*
* Description:
*  Functions used by main.
*  basically EEPROM functions at this point.
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include "helper_functions.h"

/******************************************************************************
* Function Name: helper_check_voltage_source
*******************************************************************************
*
* Summary:
*  Look in the EEPROM to for what Voltage source is selected
  # TODO: dont look in eeprom first, check selected_voltage_source before going to the eeprom
*
* Parameters:
*
* Return:
*  VDAC_NOT_SET [0] - if no voltage source has been selected yet
*  VDAC_IS_VDAC [1] - user indicated no external capacitor is installed, 
*                     so 8-bit VDAC should be used
*  VDAC_IS_DVDAC [2] - user has indicated and external capacitor is installed
*                      so the dithering VDAC (DVDAC) should be set
*
* Global variables:
*  OUT_ENDPOINT:  number that is the endpoint coming out of the computer
*
*******************************************************************************/

uint8_t helper_check_voltage_source(void) {
    // start eeprom and read the value at voltage source address
    return helper_Readbyte_EEPROM(VDAC_ADDRESS);
}

/******************************************************************************
* Function Name: helper_set_voltage_source
*******************************************************************************
*
* Summary:
*  Set the voltage source.  Connects the analog mux to the correct channel and 
*  stops the other voltage source if it was on and starts and puts to sleep the DAC
*
* Parameters:
*  uint8 voltage_source: which voltage source has been selected
*
* Global variables:
*  selected_voltage_source:  which DAC is to be used
*
*******************************************************************************/

void helper_set_voltage_source(uint8_t voltage_source) {
    selected_voltage_source = voltage_source;
    helper_Writebyte_EEPROM(voltage_source, VDAC_ADDRESS);
    
    if (selected_voltage_source == VDAC_IS_DVDAC) {
        VDAC_source_Stop();  // incase the other DAC is on, turn it off
        //LCD_Position(1,0);
        //LCD_PrintString("DVDAC AMux");
        
    }
    else {
        DVDAC_Stop();  // incase the other DAC is on, turn it off
        //LCD_Position(1,0);
        //LCD_PrintString("VDAC AMux");
    }
    DAC_Start();
    DAC_Sleep();
}



/******************************************************************************
* Function Name: helper_Writebyte_EEPROM
*******************************************************************************
*
* Summary:
*    Start the eepromm, update the temperature and write a byte to it
*
* Parameters:
*     data: the byte to write in
*     address: the address to write the data at
*
* Return:
*
*******************************************************************************/

uint8_t helper_Writebyte_EEPROM(uint8_t data, uint16_t address) {
    EEPROM_Start();
    CyDelayUs(10);
    EEPROM_UpdateTemperature();
    uint8_t write_results = EEPROM_WriteByte(data, address);
    EEPROM_Stop();
    return write_results;
}

/******************************************************************************
* Function Name: helper_Readbyte_EEPROM
*******************************************************************************
*
* Summary:
*    Start the eepromm, update the temperature and read a byte to it
*
* Parameters:
*     address: the address to read the data from
*
* Return:
*     data that was read
*
*******************************************************************************/

uint8_t helper_Readbyte_EEPROM(uint16_t address) {
    EEPROM_Start();
    CyDelayUs(10);
    EEPROM_UpdateTemperature();
    CyDelayUs(10);
    uint8_t data = EEPROM_ReadByte(address);
    EEPROM_Stop();
    return data;
}

/******************************************************************************
* Function Name: helper_HardwareSetup
*******************************************************************************
*
* Summary:
*    Setup all the hardware needed for an experiment.  This will start all the hardware
*    and then put them to sleep so they can be awoke for an experiment.  Connect all the 
*    defualt analog muxes
*
*******************************************************************************/


void helper_HardwareSetup(void) {
    helper_HardwareStart();
    helper_HardwareSleep();

    AMux_electrode_Init();
    AMux_TIA_input_Init();
    AMux_TIA_resistor_bypass_Init();
    AMux_V_source_Init();
    AMux_TIA_resistor_bypass_Init();
    DAC_Start();  // DAC has to be started after the AMux_V_source because it will set it based what DAC source is selected
    
    // iniatalize the analog muxes connections 
    AMux_electrode_Select(three_electrode_config_ch);  // start with 3 electrode configuration
    AMux_TIA_input_Select(AMux_TIA_working_electrode_ch);  // Connect the working electrode
    AMux_TIA_resistor_bypass_Select(0);  // Start with no extra TIA resistor

}

/******************************************************************************
* Function Name: helper_HardwareStart
*******************************************************************************
*
* Summary:
*    Start all the hardware needed for an experiment.
*
*******************************************************************************/

void helper_HardwareStart(void){  // start all the components that have to be on for a reading
    ADC_SigDel_Start();
    TIA_Start();
    VDAC_TIA_Start();
    Opamp_Aux_Start();
    PWM_isr_Start();
}

/******************************************************************************
* Function Name: helper_HardwareWakeup
*******************************************************************************
*
* Summary:
*    Start all the hardware needed for an experiment.
*
*******************************************************************************/

void helper_HardwareWakeup(void){  // wakeup all the components that have to be on for a reading
    ADC_SigDel_Wakeup();
    TIA_Wakeup();
    VDAC_TIA_Wakeup();
    DAC_Wakeup();
    CyDelay(1);
    DAC_SetValue(lut_value);
    CyDelay(10);
    Opamp_Aux_Wakeup();
    
    PWM_isr_Wakeup();
    
}

/******************************************************************************
* Function Name: helper_HardwareWakeup
*******************************************************************************
*
* Summary:
*    Put to sleep all the hardware needed for an experiment.
*
*******************************************************************************/

void helper_HardwareSleep(void){  // put to sleep all the components that have to be on for a reading
    ADC_SigDel_Sleep();
    DAC_Sleep();
    TIA_Sleep();
    VDAC_TIA_Sleep();
    Opamp_Aux_Sleep();
    PWM_isr_Sleep();
    
}


void make_run_params(const uint8_t data_buffer[], const uint8_t use_swv,
                     struct RunParams *run_params) {
    run_params->start_value = helper_Convert2Dec(&data_buffer[2], 4);
    run_params->end_value = helper_Convert2Dec(&data_buffer[7], 4);
    run_params->timer_period = helper_Convert2Dec(&data_buffer[12], 5);
    if (use_swv == true) {
        run_params->use_swv = true;
        run_params->swv_inc = helper_Convert2Dec(&data_buffer[18], 4);
        run_params->swv_pulse_height = helper_Convert2Dec(&data_buffer[23], 4);
        run_params->sweep_type = data_buffer[28];
        run_params->start_volt_type = data_buffer[29];
    }
    else {
        run_params->sweep_type = data_buffer[18];
        run_params->start_volt_type = data_buffer[19];
    }
    
}


uint16_t helper_Convert2Dec(const uint8_t array[], const uint8_t len){
    printf("in convert2dec");
    uint16_t num = 0;
    for (int i = 0; i < len; i++){
        num = num * 10 + (array[i] - '0');
    }
    return num;
}

/* [] END OF FILE */
