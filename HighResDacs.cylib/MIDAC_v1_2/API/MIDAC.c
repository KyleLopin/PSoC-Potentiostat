/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c  
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*    This file provides the source code to the API for the Modulated Load
     Current DAC (MLIDAC) Component.
*
*   Note:
*     
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "`$INSTANCE_NAME`.h"   
#include "`$INSTANCE_NAME`_Dither_Clk.h" 
#include "`$INSTANCE_NAME`_Dither_PWM.h" 

uint8  `$INSTANCE_NAME`_initVar = 0;
uint16 `$INSTANCE_NAME`_currentValue = 0;

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
* Summary:
*  Set power level then turn on MLIDAC.
*
* Parameters:  
* (void)
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_Init(void) 
{
  
      `$INSTANCE_NAME`_CR0 = (`$INSTANCE_NAME`_MODE_I | `$INSTANCE_NAME`_DEFAULT_RANGE | `$INSTANCE_NAME`_HS_HIGHSPEED )  ;    

       /* Set default data source */
	   `$INSTANCE_NAME`_CR1 = (`$INSTANCE_NAME`_DEFAULT_CNTL |  `$INSTANCE_NAME`_IDIR_SRC) ;   
      
      /* Set proper DAC trim */
      `$INSTANCE_NAME`_DacTrim();
   
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Enable
********************************************************************************
* Summary:
*     Enable the DVDAC10
* 
* Parameters:  
*  none  
*
* Return: 
*  (void)
*
* Theory: 
*
* Side Effects: 
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void `$INSTANCE_NAME`_Enable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`
{
	/* Enable power to DAC */
    `$INSTANCE_NAME`_PWRMGR |= `$INSTANCE_NAME`_ACT_PWR_EN;
 //   `$INSTANCE_NAME`_STBY_PWRMGR |= `$INSTANCE_NAME`_STBY_PWR_EN;
    
	 /* Restore the last value */
	 /* Set default value */
    `$INSTANCE_NAME`_SetValue(`$INSTANCE_NAME`_DEFAULT_DATA);
	`$INSTANCE_NAME`_RestoreConfig();
	
   `$INSTANCE_NAME`_Dither_Clk_Start();
   `$INSTANCE_NAME`_Dither_PWM_Start();
   `$INSTANCE_NAME`_Dither_PWM_WritePeriod(`$INSTANCE_NAME`_DEFAULT_PERIOD);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values.
*
* Parameters:  
*  None
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(void) 
{
  
    /* Hardware initiazation only needs to occure the first time */
    if ( `$INSTANCE_NAME`_initVar == 0)  
    { 
        `$INSTANCE_NAME`_Init();
       
        `$INSTANCE_NAME`_initVar = 1;
    }  
   
    /* Enable power to DAC */
    `$INSTANCE_NAME`_Enable();

}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
* Summary:
*  Powers down IDAC8 to lowest power state.
*
* Parameters:  
*   none
*
* Return: 
*  (void)
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_Stop(void)
{
   /* Disble power to DAC */
   `$INSTANCE_NAME`_PWRMGR &= ~`$INSTANCE_NAME`_ACT_PWR_EN;
   `$INSTANCE_NAME`_Dither_PWM_Stop();
   `$INSTANCE_NAME`_Dither_Clk_Stop();
   
}



/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetRange
********************************************************************************
* Summary:
*  Set current range
*
* Parameters:  
*  (uint8) Range:  Sets one of two valid ranges.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetRange(uint8 range)
{
   `$INSTANCE_NAME`_CR0 &= ~`$INSTANCE_NAME`_RANGE_MASK ;       /* Clear existing mode */
   `$INSTANCE_NAME`_CR0 |= ( range & `$INSTANCE_NAME`_RANGE_MASK );  /*  Set Range  */
   `$INSTANCE_NAME`_DacTrim();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetValue
********************************************************************************
* Summary:
*  Set DAC value
*
* Parameters:  
*  (uint16) value:  Sets DAC value between 0 and 255.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetValue(uint16 value)
{
   extern uint16 `$INSTANCE_NAME`_currentValue;
   
   `$INSTANCE_NAME`_currentValue = value;
   `$INSTANCE_NAME`_Data = (uint8)(value >>`$INSTANCE_NAME`_PWM_SHIFT );         /*  Set Value  */
   
    #if (`$INSTANCE_NAME`_PSOC3_ES2 || `$INSTANCE_NAME`_PSOC5_ES1)
	    `$INSTANCE_NAME`_Data = (uint8)(value >> `$INSTANCE_NAME`_PWM_SHIFT );
    #endif 
   
   `$INSTANCE_NAME`_Dither_PWM_WriteCompare((uint8)(value & `$INSTANCE_NAME`_PWM_MASK));
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DacTrim
********************************************************************************
* Summary:
*  Set the trim value for the given range.
*
* Parameters:  
*  None
*
* Return: 
*  (void) 
*
* Theory: 
*  Trim values for the VIDAC blocks are stored in the "Customer Table" area in 
*  Row 1 of the Hidden Flash.  There are 8 bytes of trim data for each VIDAC block.
*  The values are:
*       I Gain offset, min range, Sourcing
*       I Gain offset, min range, Sinking
*       I Gain offset, med range, Sourcing
*       I Gain offset, med range, Sinking
*       I Gain offset, max range, Sourcing
*       I Gain offset, max range, Sinking
*       V Gain offset, 1V range
*       V Gain offset, 4V range
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_DacTrim(void)
{
    uint8 mode;

    mode = ((`$INSTANCE_NAME`_CR0 & `$INSTANCE_NAME`_RANGE_MASK) >> 1);
    
    if((`$INSTANCE_NAME`_IDIR_MASK & `$INSTANCE_NAME`_CR1) == `$INSTANCE_NAME`_IDIR_SINK)
    {
        mode++;
    }

    `$INSTANCE_NAME`_TR = CY_GET_XTND_REG8((uint8 *)(`$INSTANCE_NAME`_DAC_TRIM_BASE + mode));
}

/* [] END OF FILE */


