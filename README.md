# PSoC-Potentiostat
Potentiostat that works on a single PSoC 5LP, currently configured for the CY8CKIT-059

To make a CY8CKIT-059 potentiostat download the Potentiostat.hex in the release tab or at
https://github.com/KyleLopin/PSoC-Potentiostat/releases

Load the hex file using PSoC Programmer into the CY8CKIT-059 and connect the P3[4] pin to the reference electrode, P3[6] pin to the counter electrode and P0[0] to the working electrode.

Download the Graphical User Interface from the Potentiostat_GUI repository.  You can download the python source files or the executable for Windows machines in the release tab at https://github.com/KyleLopin/Potentiostat_GUI/releases

Different electrochemical experiments are shown in different notebook tabs.



# PSoC-Potentiostat Commands

The following are the inputs commands the device will take, all inputs are inputted as ASCII strings.

'I' - Identifies the device, will respond with "USB Test - v04" through the USB

"L|X" - Set electrode configuration to 2 or 3 electrodes.  X is the number of electrodes, only 2 or 3 works

"S|XXXX|YYYY|ZZZZZ|AB" - Make a look up table for a cyclic voltammetry experiment.  XXXX is the  uint16 with the starting number to put in the DAC for the experiment.  YYYY is the uint16 with the ending number to put in the dac for the experiment.  ZZZZZ is the uint16 to put in the period of the PWM timer to set the sampling rate.   A is a char of 'L' or 'C' to make a linear sweep ('L') or a cyclic voltammetry ('C') look up table.  B is a char of 'Z' or 'S' to start the waveform at 0 Volts ('Z') or at the value entered in the XXXX field.

'R' - Start a cyclic voltammerty experiment with the last look up table that was inputted.  To get the data get the ADC Array 0.

"EX" - Export an ADC array.  There are 4 arrays, cyclic voltammetry experiments are stored in the 0 array, the other arrays are used for streaming applications.

"M|XXXX|YYYY" - Run an amperometry experiment.  You need to start to read the data the device will start streaming when given this command. XXXX is an uint16 number to set the DAC value to so the electrodes are at the approriate voltage.  YYYY is an uint16 of how many data points to collect in each ADC buffer before exporting the data

"FX" - Exprot an ADC array for streamming data where X is the number of the ADC array to get from 0-3.

"A|U|X|Y|Z|W" - Set up the TIA and ADC.  U is the ADC configuration to use where config 1 uses a Vref of +-2.048 V and config 2 uses +-1.024 V.  X is the TIA resistor value index, a string between 0-7 that sets the TIA resistor value {0-20k, 1-30k, 2-40k, 3-80k, 4-120k, 5-250k, 6-500k, 7-1000k}.  Y is the adc buffer gain setting {1, 2, 4, 8}.  Z is 'T' or 'F' for if an external resistor is to be used and the AMux_working_electrode should be set according.  W is 0 or 1 for which user resistor should be selected by the AMux_working_electrode.

'B' - Calibrate the ADC and TIA signal chain.

"VXY" - Check or set the voltage source.  X is 'R' to read the voltage source or 'S' to set the voltage source.  When setting the voltage source Y should be '2' for the 12-bit dithering VDAC, all other numbers will default to the 8-bit VDAC.  When reading the voltage source, the device will return the string "VZ" where Z is the voltage source choice selected before.

"S|XXXXX" - set the period value of the PWM used as a timer that starts the isrs to change the DAC and read the ADC.  XXXXX is a uint16 that is put into the PWM that set the timing with a sample rate of 240 kHz / XXXXX

"C|XXXXX" - set the compare value of the PWM used as a timer that sets when the DAC changes compared to when the ADC measures.

'X' - Reset the device.  Disable all isrs and put the hardware to sleep.

"D|XXXX" - Set the voltage control DAC.  XXXX is the value to put in the DAC, which ever one is active.

'H' - Wake up all the hardware.

's' - Short the TIA so the working electrode can sink more current.

'd' - Stop shorting the TIA

# PSoC-Potentiostat Future directions

- add status flag such as running CV, ASV or amperometry, data ready, error to the code that the GUI can poll if there is a problem
- for CVs longer than 500 ms have the device export the data if the computer asks for it
- add DMAs to the timing PWM to save CPU resources.   Leave isrs for now for debug and possible data processing in the future
- add pwm to DAC isr/DMA so DVP / stair case measurements can be made
- add pwm to ADC to take multi-measurements per voltage step to average.
- add electrochemical techniques for SWV, NPV, ac voltammertry, pulsed amperometry, potentiometry, chronopotentiometry


# Preliminary roadmap
## v1.3 - DPV and clean up
- Get a SWV working on CV frame
- get versions of GUI and hex files synced

## v1.4 - hardware optimizations
- Change USB to USBUART to make connections easier
- change adc and dac changes from the isr to hardware
- GUI start adding unittests

## v1.5 - GUI optimizations
- refactor the USB handler in the GUI to seperate into low and high level functions
- add more unittest to GUI
- Change from tabs for each experiment to the user can start a new tab with the experiment type they want

## v1.6 - Improve hardware
- add options to take multiple measurements at the end of the pulse to average
- while running experiments, send data to the computer to graph while running

## v1.7 - add more options
- start adding more experimental options
- add ac voltammertry, pulsed amperometry

## v1.8 - 
- add potentiometry, chronopotentiometry

optional / exploratory
- add auto range for the current.  Can TIA swith resistor that fast?
- add way to have an equilibrium time for CV
- switch voltage command DAC to TIA so that multiple Working electrodes can be used
- free up the counter electrode DAC or TIA DAC with the VRef
- add calibration routine
- add DFB for removing high frequency noise


# PSoC-Potentiostat Hardware changes to make

Add case and connections to use banana clips
Add case and connections to use the 4 twisted wire like EmStat

