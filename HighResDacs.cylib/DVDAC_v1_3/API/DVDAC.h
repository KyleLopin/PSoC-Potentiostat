/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h  
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*    This file contains the function prototypes and constants used in
*    the Dithered Voltage DAC (DVDAC) Component.
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

#if !defined(CY_DVDAC_`$INSTANCE_NAME`_H) 
#define CY_DVDAC_`$INSTANCE_NAME`_H

#include "cytypes.h"
#include "cyfitter.h"

/* PSoC3 ES2 or early */
#define `$INSTANCE_NAME`_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))                    
/* PSoC5 ES1 or early */
#define `$INSTANCE_NAME`_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define `$INSTANCE_NAME`_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define `$INSTANCE_NAME`_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))
/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct `$INSTANCE_NAME`_backupStruct
{
    uint8 enableState; 
	uint16 data_value;
}`$INSTANCE_NAME`_backupStruct;

/***************************************
*        Function Prototypes 
***************************************/

void `$INSTANCE_NAME`_Start(void);
void `$INSTANCE_NAME`_Stop(void)            `=ReentrantKeil($INSTANCE_NAME . "_Stop")`;
void `$INSTANCE_NAME`_SetSpeed(uint8 speed) `=ReentrantKeil($INSTANCE_NAME . "_SetSpeed")`;
void `$INSTANCE_NAME`_SetRange(uint8 range) `=ReentrantKeil($INSTANCE_NAME . "_SetRange")`;
void `$INSTANCE_NAME`_SetValue(uint16 value)`=ReentrantKeil($INSTANCE_NAME . "_SetValue")`;
void `$INSTANCE_NAME`_DacTrim(void)         `=ReentrantKeil($INSTANCE_NAME . "_DacTrim")`;
void `$INSTANCE_NAME`_setupDMA(void)        `=ReentrantKeil($INSTANCE_NAME . "_setupDMA")`;
void `$INSTANCE_NAME`_Init(void)            `=ReentrantKeil($INSTANCE_NAME . "_Init")`;
void `$INSTANCE_NAME`_Enable(void)          `=ReentrantKeil($INSTANCE_NAME . "_Enable")`;
void `$INSTANCE_NAME`_SaveConfig(void);
void `$INSTANCE_NAME`_RestoreConfig(void);
void `$INSTANCE_NAME`_Sleep(void);
void `$INSTANCE_NAME`_Wakeup(void)          `=ReentrantKeil($INSTANCE_NAME . "_Wakeup")`;
  
  
  
/***************************************
*            API Constants
***************************************/

/* SetRange constants */
`#cy_declare_enum vDacRange`
#define `$INSTANCE_NAME`_RANGE_1V       0x00u
#define `$INSTANCE_NAME`_RANGE_4V       0x04u


/***************************************
*  Initialization Parameter Constants
***************************************/

#define `$INSTANCE_NAME`_DEFAULT_RANGE    `$DAC_Range`      /* Default DAC range */
#define `$INSTANCE_NAME`_DEFAULT_CNTL     0x00u              /* Default Control */
#define `$INSTANCE_NAME`_DEFAULT_DATA     `$Initial_Value`   /* Initial DAC value */
#define `$INSTANCE_NAME`_RESOLUTION       `$Resolution` /* DAC Resolution */

#define `$INSTANCE_NAME`_SHIFT_LEN         (`$Resolution`-8)  /* Data Shift */
#define `$INSTANCE_NAME`_DITHER_SIZE       (1<<(`$Resolution`-8)) /* Size of dither array */
#define `$INSTANCE_NAME`_LSB_MASK          (`$INSTANCE_NAME`_DITHER_SIZE - 1)          /* Used to mask of the LSbs */


/* DMA Configuration for DAC_DMA */
#define `$INSTANCE_NAME`_DMA_BYTES_PER_BURST 1
#define `$INSTANCE_NAME`_DMA_REQUEST_PER_BURST 1
#define `$INSTANCE_NAME`_DMA_SRC_BASE (CYDEV_SRAM_BASE)
#define `$INSTANCE_NAME`_DMA_DST_BASE (CYDEV_PERIPH_BASE)


/***************************************
*              Registers        
***************************************/

#define `$INSTANCE_NAME`_CR0         (* (reg8 *) `$INSTANCE_NAME`_viDAC8__CR0 )
#define `$INSTANCE_NAME`_CR1         (* (reg8 *) `$INSTANCE_NAME`_viDAC8__CR1 )
#define `$INSTANCE_NAME`_Data        (* (reg8 *) `$INSTANCE_NAME`_viDAC8__D )
#define `$INSTANCE_NAME`_Data_PTR    (  (reg8 *) `$INSTANCE_NAME`_viDAC8__D )
#define `$INSTANCE_NAME`_Strobe      (* (reg8 *) `$INSTANCE_NAME`_viDAC8__STROBE )
#define `$INSTANCE_NAME`_SW0         (* (reg8 *) `$INSTANCE_NAME`_viDAC8__SW0 )
#define `$INSTANCE_NAME`_SW2         (* (reg8 *) `$INSTANCE_NAME`_viDAC8__SW2 )
#define `$INSTANCE_NAME`_SW3         (* (reg8 *) `$INSTANCE_NAME`_viDAC8__SW3 )
#define `$INSTANCE_NAME`_SW4         (* (reg8 *) `$INSTANCE_NAME`_viDAC8__SW4 )
#define `$INSTANCE_NAME`_TR          (* (reg8 *) `$INSTANCE_NAME`_viDAC8__TR )
#define `$INSTANCE_NAME`_PWRMGR      (* (reg8 *) `$INSTANCE_NAME`_viDAC8__PM_ACT_CFG )  /* Power manager */
#define `$INSTANCE_NAME`_STBY_PWRMGR (* (reg8 *) `$INSTANCE_NAME`_viDAC8__PM_STBY_CFG )  /* Standby Power manager */
                                                              

/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE                  */
#define `$INSTANCE_NAME`_HS_MASK        0x02u
#define `$INSTANCE_NAME`_HS_LOWPOWER    0x00u
#define `$INSTANCE_NAME`_HS_HIGHSPEED   0x02u

/* Bit Field  DAC_MODE                  */
#define `$INSTANCE_NAME`_MODE_MASK      0x10u
#define `$INSTANCE_NAME`_MODE_V         0x00u
#define `$INSTANCE_NAME`_MODE_I         0x10u

/* Bit Field  DAC_RANGE                  */
#define `$INSTANCE_NAME`_RANGE_MASK     0x0Cu
#define `$INSTANCE_NAME`_RANGE_0        0x00u
#define `$INSTANCE_NAME`_RANGE_1        0x04u

/* CR1 iDac Control Register 1 definitions */

/* Bit Field  DAC_MX_DATA                  */
#define `$INSTANCE_NAME`_SRC_MASK       0x20u
#define `$INSTANCE_NAME`_SRC_REG        0x00u
#define `$INSTANCE_NAME`_SRC_UDB        0x20u

/* This bit enable reset from UDB array      */
#define `$INSTANCE_NAME`_RESET_MASK     0x10u
#define `$INSTANCE_NAME`_RESET_ENABLE   0x10u
#define `$INSTANCE_NAME`_RESET_DISABLE  0x00u

/* This bit enables data from DAC bus      */
#define `$INSTANCE_NAME`_DACBUS_MASK     0x20u
#define `$INSTANCE_NAME`_DACBUS_ENABLE   0x20u
#define `$INSTANCE_NAME`_DACBUS_DISABLE  0x00u

/* DAC STROBE Strobe Control Register definitions */

/* Bit Field  DAC_MX_STROBE                  */
#define `$INSTANCE_NAME`_STRB_MASK     0x08u
#define `$INSTANCE_NAME`_STRB_EN       0x08u
#define `$INSTANCE_NAME`_STRB_DIS      0x00u


/* PM_ACT_CFG (Active Power Mode CFG Register)     */ 
#define `$INSTANCE_NAME`_ACT_PWR_EN   `$INSTANCE_NAME`_viDAC8__PM_ACT_MSK   /* Power enable mask */ 
#define `$INSTANCE_NAME`_STBY_PWR_EN  `$INSTANCE_NAME`_viDAC8__PM_STBY_MSK  /* Standby Power enable mask */



/***************************************
*              Trim    
* Note - VIDAC trim values are stored in the "Customer Table" area in 
* Row 1 of the Hidden Flash.  There are 8 bytes of trim data for each VIDAC block.
* The values are:
*       I Gain offset, min range, Sourcing
*       I Gain offset, min range, Sinking
*       I Gain offset, med range, Sourcing
*       I Gain offset, med range, Sinking
*       I Gain offset, max range, Sourcing
*       I Gain offset, max range, Sinking
*       V Gain offset, 1V range
*       V Gain offset, 4V range
*
* The data set for the 4 VIDACs are arranged using a left side/right side approach:
*   Left 0, Left 1, Right 0, Right 1.
* When mapped to the VIDAC0 thru VIDAC3 as:
*   VIDAC 0, VIDAC 2, VIDAC 1, VIDAC 3
***************************************/
#define `$INSTANCE_NAME`_TRIM_M7_1V_RNG_OFFSET  0x06u
#define `$INSTANCE_NAME`_TRIM_M8_4V_RNG_OFFSET  0x07u

#define `$INSTANCE_NAME`_DAC_TRIM_BASE     (`$INSTANCE_NAME`_viDAC8__TRIM__M1)


#endif /* CY_DVDVDAC_`$INSTANCE_NAME`_H  */
/* [] END OF FILE */

