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
#include "stdio.h"  // gets rid of the type errors
    
#include "globals.h"
#include "helper_functions.h"
#include "usb_protocols.h"
#include "lut_protocols.h"
    
    
#define DO_NOT_RESTART_ADC      0
   
/***************************************
*        Function Prototypes
***************************************/  
   
void user_setup_TIA_ADC(uint8_t data_buffer[]);
void user_run_cv_experiment(uint8_t data_buffer[]);
void user_voltage_source_funcs(uint8_t data_buffer[]);
void user_start_cv_run(void);
void user_reset_device(void);
void user_identify(void);
void user_set_isr_timer(uint8_t data_buffer[]);
uint16_t user_chrono_lut_maker(uint8_t data_buffer[]);
uint16_t user_dpv_lut_maker(uint8_t data_buffer[]);
//uint16_t user_dpv_lut_make_depr(uint8_t data_buffer[]);
uint16_t user_lookup_table_make_future(uint8_t data_buffer[]);
uint16_t user_lookup_table_maker(uint8_t data_buffer[]);
uint16_t user_lookup_table_maker_swv(uint8_t data_buffer[]);
uint16_t user_run_amperometry(uint8_t data_buffer[]);


/***************************************
* Global variables external identifier
***************************************/

extern uint8_t TIA_resistor_value_index;
extern uint8_t ADC_buffer_index;
    
#endif

/* [] END OF FILE */
