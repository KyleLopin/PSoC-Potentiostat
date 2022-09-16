
/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c  
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*    This file provides the source code to the API for the parallel Current DAC 
*    (PIDAC) Component.
*
*   Note:
*     
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "`$INSTANCE_NAME`.h"   

uint8 `$INSTANCE_NAME`_initVar = 0;

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
* Summary:  Initialize and power up both DACs
*
* Parameters:  
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
       `$INSTANCE_NAME`_initVar = 1;
	   `$INSTANCE_NAME`_Init();   
	}
	
	/* Enable power to DAC */
    `$INSTANCE_NAME`_Enable();
   
    /* Set default value */
    `$INSTANCE_NAME`_SetValue(`$INSTANCE_NAME`_DEFAULT_DATA);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Enable
********************************************************************************
* Summary:  
*   Power up both DACs
*
* Parameters:  
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_Enable(void) 
{
  
       /* Enable power to DAC */
   `$INSTANCE_NAME`_MSB_PWRMGR |= `$INSTANCE_NAME`_MSB_ACT_PWR_EN;
   `$INSTANCE_NAME`_LSB_PWRMGR |= `$INSTANCE_NAME`_LSB_ACT_PWR_EN;

}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Init
********************************************************************************
* Summary:
*  Initialize mode, range, and polarity.
*
* Parameters: 
* ( void )
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
   /* Setup block modes */
   `$INSTANCE_NAME`_MSB_CR0 = `$INSTANCE_NAME`_MODE_I;
   `$INSTANCE_NAME`_LSB_CR0 = `$INSTANCE_NAME`_MODE_I;
   
   `$INSTANCE_NAME`_SetRange(`$INSTANCE_NAME`_DEFAULT_RANGE);

   `$INSTANCE_NAME`_MSB_CR1 = (`$INSTANCE_NAME`_DEFAULT_CNTL | `$INSTANCE_NAME`_DACBUS_DISABLE | `$INSTANCE_NAME`_DEFAULT_POLARITY) ;   
   `$INSTANCE_NAME`_LSB_CR1 = (`$INSTANCE_NAME`_DEFAULT_CNTL | `$INSTANCE_NAME`_DACBUS_DISABLE | `$INSTANCE_NAME`_DEFAULT_POLARITY) ;   

    /* Set default strobe mode */
   if(`$INSTANCE_NAME`_DEFAULT_STRB != 0)
   {
       `$INSTANCE_NAME`_MSB_Strobe |= `$INSTANCE_NAME`_STRB_EN ;
       `$INSTANCE_NAME`_LSB_Strobe |= `$INSTANCE_NAME`_STRB_EN ;
   }

   /* Set default speed */
   `$INSTANCE_NAME`_SetSpeed(`$INSTANCE_NAME`_DEFAULT_SPEED); 

   /* Set proper DAC trim */
   `$INSTANCE_NAME`_DacTrim();
  
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
* Summary:
*  Powers down PIDAC to lowest power state.
*
* Parameters:  
*   (void)
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
   `$INSTANCE_NAME`_MSB_PWRMGR &= ~`$INSTANCE_NAME`_MSB_ACT_PWR_EN;
   `$INSTANCE_NAME`_LSB_PWRMGR &= ~`$INSTANCE_NAME`_LSB_ACT_PWR_EN;
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetSpeed
********************************************************************************
* Summary:
*  Set DAC speed
*
* Parameters:  
*  power:   Sets speed value
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetSpeed(uint8 speed) 
{
   /* Clear power mask then write in new value */
   `$INSTANCE_NAME`_MSB_CR0 &= ~`$INSTANCE_NAME`_HS_MASK ;    
   `$INSTANCE_NAME`_MSB_CR0 |=  ( speed & `$INSTANCE_NAME`_HS_MASK) ;    

   `$INSTANCE_NAME`_LSB_CR0 &= ~`$INSTANCE_NAME`_HS_MASK ;    
   `$INSTANCE_NAME`_LSB_CR0 |=  ( speed & `$INSTANCE_NAME`_HS_MASK) ;    
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetPolarity
********************************************************************************
* Summary:
*  Sets IDAC to Sink or Source current
*
* Parameters:  
*  Polarity: Sets the IDAC to Sink or Source
*             0 = sink, 1 = source
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetPolarity(uint8 polarity)
{
   
   `$INSTANCE_NAME`_MSB_CR1 &= ~`$INSTANCE_NAME`_IDIR_MASK ;   
   `$INSTANCE_NAME`_LSB_CR1 &= ~`$INSTANCE_NAME`_IDIR_MASK ;   
   if(polarity == 0)     /* Sink Current */
   {
      `$INSTANCE_NAME`_MSB_CR1 &= ~`$INSTANCE_NAME`_IDIR_SINK ;   
      `$INSTANCE_NAME`_LSB_CR1 &= ~`$INSTANCE_NAME`_IDIR_SINK ;   
   }
   else               /* Source Current */
   {
      `$INSTANCE_NAME`_MSB_CR1 |= `$INSTANCE_NAME`_IDIR_SRC ;   
      `$INSTANCE_NAME`_LSB_CR1 |= `$INSTANCE_NAME`_IDIR_SRC ;   
   }
   `$INSTANCE_NAME`_DacTrim();
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetRange
********************************************************************************
* Summary:
*  Set current range
*
* Parameters:  
*  Range:  Sets on of four valid ranges.
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
   if(range == `$INSTANCE_NAME`_RANGE_255uA)  /* 255uA Range */
   {
      `$INSTANCE_NAME`_MSB_CR0 &= ~`$INSTANCE_NAME`_DAC_RANGE_MASK ;            /* Clear MSB existing mode */
      `$INSTANCE_NAME`_MSB_CR0 |= `$INSTANCE_NAME`_DAC_RANGE_255uA;             /* Set MSB Range to 255uA  */
	  
	  `$INSTANCE_NAME`_LSB_CR0 &= ~`$INSTANCE_NAME`_DAC_RANGE_MASK ;            /* Clear MSB existing mode */
      `$INSTANCE_NAME`_LSB_CR0 |= `$INSTANCE_NAME`_DAC_RANGE_32uA;              /* Set MSB Range to 32uA  */
   }
   else                                       /* 2mA Range */
   {
      `$INSTANCE_NAME`_MSB_CR0 &= ~`$INSTANCE_NAME`_DAC_RANGE_MASK ;            /* Clear MSB existing mode */
      `$INSTANCE_NAME`_MSB_CR0 |= `$INSTANCE_NAME`_DAC_RANGE_2040uA;            /* Set MSB Range to 2040uA  */
	  
	  `$INSTANCE_NAME`_LSB_CR0 &= ~`$INSTANCE_NAME`_DAC_RANGE_MASK ;            /* Clear MSB existing mode */
      `$INSTANCE_NAME`_LSB_CR0 |= `$INSTANCE_NAME`_DAC_RANGE_255uA;             /* Set MSB Range to 255uA  */
   }
	`$INSTANCE_NAME`_DacTrim();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetValue
********************************************************************************
* Summary:
*  Set DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 2047.
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
   uint8 msb;
   uint8 lsb;

    msb = (uint8)(value >> `$INSTANCE_NAME`_MSB_RSHIFT);
	lsb = (uint8) ((value & `$INSTANCE_NAME`_LSB_MASK)<< `$INSTANCE_NAME`_LSB_LSHIFT);
	

    `$INSTANCE_NAME`_MSB_Data = msb;             /*  Set MSB Value  */
    `$INSTANCE_NAME`_LSB_Data = lsb;             /*  Set LSB Value  */
    
    #if (`$INSTANCE_NAME`_PSOC3_ES2 || `$INSTANCE_NAME`_PSOC5_ES1)
	    `$INSTANCE_NAME`_MSB_Data = msb;         /*  Set MSB Value  */
     	`$INSTANCE_NAME`_LSB_Data = lsb;         /*  Set LSB Value  */
    #endif 

}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DacTrim
********************************************************************************
* Summary:
*  Set the trim value for the given range.
*
* Parameters:  
*  value:  Sets DAC value between 0 and 2^bits - 1
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void `$INSTANCE_NAME`_DacTrim(void)
{
 	uint8 mode;
	
   /* Trim MSB */
	mode = ((`$INSTANCE_NAME`_MSB_CR0 & `$INSTANCE_NAME`_DAC_RANGE_MASK) >> 1);
	if((`$INSTANCE_NAME`_IDIR_MASK & `$INSTANCE_NAME`_MSB_CR1) == `$INSTANCE_NAME`_IDIR_SINK)
	{
		mode++;
	}
	`$INSTANCE_NAME`_MSB_TR = CY_GET_XTND_REG8((uint8 *)(`$INSTANCE_NAME`_MSB_DAC_TRIM_BASE + mode));

   /* Trim LSB */
	mode = ((`$INSTANCE_NAME`_LSB_CR0 & `$INSTANCE_NAME`_DAC_RANGE_MASK) >> 1);
	if((`$INSTANCE_NAME`_IDIR_MASK & `$INSTANCE_NAME`_LSB_CR1) == `$INSTANCE_NAME`_IDIR_SINK)
	{
		mode++;
	}
	`$INSTANCE_NAME`_LSB_TR = CY_GET_XTND_REG8((uint8 *)(`$INSTANCE_NAME`_LSB_DAC_TRIM_BASE + mode));
}

/* [] END OF FILE */


