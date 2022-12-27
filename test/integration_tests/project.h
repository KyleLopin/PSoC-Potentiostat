#ifndef _PROJECT_H_
#define _PROJECT_H_

int ADC_SigDel_StartConvert() {return 1;}
int isr_dac_Enable() {return 1;}
int VDAC_TIA_Start() {return 1;}
int isr_dac_GetState() {return 1;}
int AMux_electrode_Select(uint16_t foo) {return 1;}
int ADC_SigDel_Wakeup() {return 1;}
uint8_t ADC_buffer_index;
int PWM_isr_Wakeup() {return 1;}
int PWM_isr_Sleep() {return 1;}
int AMux_TIA_input_Init() {return 1;}
int PWM_isr_Start() {return 1;}
int VDAC_TIA_Wakeup() {return 1;}
int isr_adcAmp_GetState() {return 1;}
int AMux_TIA_resistor_bypass_Disconnect(uint16_t foo) {return 1;}
int PWM_isr_WriteCounter(uint16_t foo) {return 1;}
void DAC_Sleep(void){}
int EEPROM_Start() {return 1;}
int isr_adc_Disable() {return 1;}
int AMux_electrode_Init() {return 1;}
int EEPROM_ReadByte() {return 1;}
int TIA_Wakeup() {return 1;}
int AMux_TIA_resistor_bypass_Init(uint16_t foo) {return 1;}
int Opamp_Aux_Wakeup() {return 1;}
int EEPROM_UpdateTemperature() {return 1;}
int CyDelay(uint16_t foo) {return 1;}
int TIA_Start() {return 1;}
int ADC_SigDel_Start() {return 1;}
int PWM_isr_WriteCompare(uint16_t foo) {return 1;}
int isr_adcAmp_Enable() {return 1;}
int EEPROM_WriteByte(uint16_t foo, uint16_t bar) {return 1;}
int Opamp_Aux_Sleep() {return 1;}
void USB_Export_Data(uint8_t array[], uint16_t size){}
int isr_adc_Enable() {return 1;}
int Opamp_Aux_Start() {return 1;}
int AMux_TIA_resistor_bypass_Select() {return 1;}
void DAC_SetValue(uint16_t value){}
int DVDAC_Stop() {return 1;}
int isr_adcAmp_Disable() {return 1;}
void DAC_Wakeup(void){}
int AMux_TIA_resistor_bypass_Connect(int foobar) {return 1;}
int TIA_Sleep() {return 1;}
int ADC_SigDel_SelectConfiguration() {return 1;}
int CyDelayUs(uint16_t foo) {return 1;}
int ADC_SigDel_GetResult16() {return 1;}
int VDAC_source_Stop() {return 1;}
int TIA_SetResFB() {return 1;}
void DAC_Start(void){}
uint8_t TIA_resistor_value_index;
int AMux_TIA_input_Select(uint16_t foo) {return 1;}
int ADC_SigDel_Sleep() {return 1;}
int ADC_SigDel_SetBufferGain() {return 1;}
int isr_dac_Disable() {return 1;}
int VDAC_TIA_Sleep() {return 1;}
int AMux_V_source_Init() {return 1;}
int EEPROM_Stop() {return 1;}
int PWM_isr_WritePeriod(uint16_t foo) {return 1;}

#endif
