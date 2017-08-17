/*******************************************************************************
* File Name: user_selections.h
*
* Description:
*  
*
**********************************************************************************
 * Copyright Kyle Vitautas Lopin, Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(USER_SELECTIONS_H)
#define USER_SELECTIONS_H
    
#include <project.h>
#include "globals.h"
#include "helper_functions.h"
#include "lut_protocols.h"
#include "usb_protocols.h"
    
    
#define DO_NOT_RESTART_ADC      0
   
/***************************************
*        Function Prototypes
***************************************/  
   
void user_setup_TIA_ADC(uint8 data_buffer[]);
void user_run_cv_experiment(uint8 data_buffer[]);
void user_voltage_source_funcs(uint8 data_buffer[]);
void user_start_cv_run(void);
void user_reset_device(void);
void user_identify(void);
void user_set_isr_timer(uint8 data_buffer[]);
uint16 user_chrono_lut_maker(uint8 data_buffer[]);
uint16 user_lookup_table_maker(uint8 data_buffer[]);
uint16 user_run_amperometry(uint8 data_buffer[]);


/***************************************
* Global variables external identifier
***************************************/

extern uint8 TIA_resistor_value_index;
extern uint8 ADC_buffer_index;


    
    
#endif

/* [] END OF FILE */
