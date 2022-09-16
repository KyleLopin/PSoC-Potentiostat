/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c  
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*    This file provides the source code to the API for the 8-bit Dithered 
*    Voltage DAC (DVDAC) Component.
*
*   Note:
*     Any unusual or non-standard behavior should be noted here. Other-
*     wise, this section should remain blank.
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include "cytypes.h"
#include "`$INSTANCE_NAME`.h"
#include "`$INSTANCE_NAME`_DMA_dma.h"
#include "`$INSTANCE_NAME`_Clk.h"

uint8  `$INSTANCE_NAME`_initVar = 0;
uint16 `$INSTANCE_NAME`_currentValue = `$INSTANCE_NAME`_DEFAULT_DATA; 

/* Variable declarations for DAC_DMA */
uint8 `$INSTANCE_NAME`_DArray[`$INSTANCE_NAME`_DITHER_SIZE];
uint8 `$INSTANCE_NAME`_TD;
uint8 `$INSTANCE_NAME`_Channel;


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Init
********************************************************************************
*
* Summary:
*  The start function initializes the DAC with the default values.  
*
* Parameters:  
*  (void)
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
  
   /* Hardware initiazation only needs to occure the first time */

   `$INSTANCE_NAME`_CR0 = (`$INSTANCE_NAME`_MODE_V | `$INSTANCE_NAME`_HS_HIGHSPEED )  ;    

   /* Set default data source */
   `$INSTANCE_NAME`_CR1 = (`$INSTANCE_NAME`_DEFAULT_CNTL | `$INSTANCE_NAME`_DACBUS_DISABLE) ;  

   /* Set default strobe mode */
   `$INSTANCE_NAME`_Strobe |= `$INSTANCE_NAME`_STRB_EN ;

   /* Set default range */
   `$INSTANCE_NAME`_SetRange(`$INSTANCE_NAME`_DEFAULT_RANGE); 
	 
   /* Set default value */
   `$INSTANCE_NAME`_SetValue(`$INSTANCE_NAME`_DEFAULT_DATA);
   
   /* Setup DMA */
    `$INSTANCE_NAME`_setupDMA();

}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Enable
********************************************************************************
* Summary:
*     Enable the DVDAC
* 
* Parameters:  
*  void:  
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
    `$INSTANCE_NAME`_STBY_PWRMGR |= `$INSTANCE_NAME`_STBY_PWR_EN;
    
	 /* Restore the last value */
	`$INSTANCE_NAME`_RestoreConfig();
	
	/* Start Clock */
    `$INSTANCE_NAME`_Clk_Start();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
*
* Summary:
*  The start function initializes the DAC with the default values.
*
* Parameters:  
*  (void)
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
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:  
*  (void)  
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_Stop(void) `=ReentrantKeil($INSTANCE_NAME . "_Stop")`
{
   /* Disble power to DAC */
   `$INSTANCE_NAME`_SaveConfig();
   
   `$INSTANCE_NAME`_Clk_Stop();
   
   `$INSTANCE_NAME`_PWRMGR &= ~`$INSTANCE_NAME`_ACT_PWR_EN;
}



/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetRange
********************************************************************************
*
* Summary:
*  Set one of two current ranges.
*
* Parameters:  
*  Range:  1 or 4 volt range.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetRange(uint8 range)  `=ReentrantKeil($INSTANCE_NAME . "_SetRange")`
{
   `$INSTANCE_NAME`_CR0 &= ~`$INSTANCE_NAME`_RANGE_MASK ;      /* Clear existing mode */
   `$INSTANCE_NAME`_CR0 |= ( range & `$INSTANCE_NAME`_RANGE_MASK ) ; /*  Set Range  */
   `$INSTANCE_NAME`_DacTrim();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetValue
********************************************************************************
*
* Summary:
*  Set new output value.
*
* Parameters:  
*  (uint16) value:  Sets DAC value between 0 and 2^bits - 1.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetValue(uint16 value)  `=ReentrantKeil($INSTANCE_NAME . "_SetValue")`
{
   uint8 msb;
   uint8 lsb;
   uint8 i;
   
   `$INSTANCE_NAME`_currentValue = value;
   msb = (uint8)(value >> `$INSTANCE_NAME`_SHIFT_LEN); 
   lsb = (uint8)(value & `$INSTANCE_NAME`_LSB_MASK);  /* Mask off the two LSbs */
   
   if (msb == 0xFF)     /* If DAC is at max value force limit */
   {
   		for(i=0; i < `$INSTANCE_NAME`_DITHER_SIZE; i++)
		{
	  		`$INSTANCE_NAME`_DArray[i] = 0xFF;
		}
   }
   else
   {
		for(i=0; i < `$INSTANCE_NAME`_DITHER_SIZE; i++)
		{
			if(i > lsb )
			{
				`$INSTANCE_NAME`_DArray[i] = msb;  
			}
			else
			{
				`$INSTANCE_NAME`_DArray[i] = msb+1;
			}
		}
	}
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:  
*   range:  1V or 4V range.  See constants.  
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_DacTrim(void) `=ReentrantKeil($INSTANCE_NAME . "_DacTrim")`
{
    uint8 mode;
	
	mode = ((`$INSTANCE_NAME`_CR0 & `$INSTANCE_NAME`_RANGE_MASK) >> 2) + `$INSTANCE_NAME`_TRIM_M7_1V_RNG_OFFSET;
	
	`$INSTANCE_NAME`_TR = CY_GET_XTND_REG8((uint8 *)(`$INSTANCE_NAME`_DAC_TRIM_BASE + mode));
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_setupDMA
********************************************************************************
*
* Summary:
*  Setup and start DMA channel.
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
void `$INSTANCE_NAME`_setupDMA(void)
{
    /* DMA Configuration for DAC_DMA */

	`$INSTANCE_NAME`_Channel = `$INSTANCE_NAME`_DMA_DmaInitialize(`$INSTANCE_NAME`_DMA_BYTES_PER_BURST, 
	`$INSTANCE_NAME`_DMA_REQUEST_PER_BURST, 
    HI16(`$INSTANCE_NAME`_DMA_SRC_BASE), 
	HI16(`$INSTANCE_NAME`_DMA_DST_BASE));
	
	`$INSTANCE_NAME`_TD = CyDmaTdAllocate();
	CyDmaTdSetConfiguration(`$INSTANCE_NAME`_TD, `$INSTANCE_NAME`_DITHER_SIZE, `$INSTANCE_NAME`_TD, TD_INC_SRC_ADR);	
	CyDmaTdSetAddress(`$INSTANCE_NAME`_TD, LO16((uint32)`$INSTANCE_NAME`_DArray), LO16((uint32)`$INSTANCE_NAME`_Data_PTR));	
	CyDmaChSetInitialTd(`$INSTANCE_NAME`_Channel, `$INSTANCE_NAME`_TD);	
	CyDmaChEnable(`$INSTANCE_NAME`_Channel, 1);

}
/* [] END OF FILE */

