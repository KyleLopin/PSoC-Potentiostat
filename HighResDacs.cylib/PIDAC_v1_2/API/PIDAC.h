/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h  
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*    This file contains the function prototypes and constants used in
*    the 10/11-bit Current DAC (PIDAC) User Module.
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
#include "cyfitter.h"

#if !defined(CY_PIDAC_`$INSTANCE_NAME`_H)  
#define CY_PIDAC_`$INSTANCE_NAME`_H


/***************************************
* Conditional Compilation Parameters
***************************************/

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

									
/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct `$INSTANCE_NAME`_backupStruct
{
    uint8 enableState;   
	uint8 MSB_data_value;
	uint8 LSB_data_value;
}`$INSTANCE_NAME`_backupStruct;


/***************************************
*        Function Prototypes 
***************************************/

void    `$INSTANCE_NAME`_Start(void);
void    `$INSTANCE_NAME`_Stop(void);
void    `$INSTANCE_NAME`_SetSpeed(uint8 speed);
void    `$INSTANCE_NAME`_SetPolarity(uint8 polarity);
void    `$INSTANCE_NAME`_SetRange(uint8 iRange);
void    `$INSTANCE_NAME`_SetValue(uint16 value);

void    `$INSTANCE_NAME`_DacTrim(void);

/* Sleep Retention APIs */
void `$INSTANCE_NAME`_Init(void);
void `$INSTANCE_NAME`_Enable(void);
void `$INSTANCE_NAME`_SaveConfig(void);
void `$INSTANCE_NAME`_RestoreConfig(void);
void `$INSTANCE_NAME`_Sleep(void);
void `$INSTANCE_NAME`_Wakeup(void);

/***************************************
*       Initialization Values
***************************************/


#define `$INSTANCE_NAME`_DEFAULT_RANGE      `$DAC_Range`      /* Default DAC range */
#define `$INSTANCE_NAME`_DEFAULT_SPEED      ((`$DAC_Speed` ? 1:0)*2)  /* Default DAC speed */
#define `$INSTANCE_NAME`_DEFAULT_CNTL       0x00u             /* Default Control */
#define `$INSTANCE_NAME`_DEFAULT_STRB       `$Strobe_Mode`    /* Default Strobe mode */
#define `$INSTANCE_NAME`_DEFAULT_DATA       `$Initial_Value`  /* Initial DAC value */
#define `$INSTANCE_NAME`_DEFAULT_POLARITY   `$Polarity`       /* Default Sink or Source */
#define `$INSTANCE_NAME`_RESOLUTION         `$Resolution` /* DAC Resolution */

#define `$INSTANCE_NAME`_MSB_RSHIFT         (`$Resolution`-8)  /* MSB right shift */
#define `$INSTANCE_NAME`_LSB_LSHIFT         (11-`$Resolution`)  /* LSB left shift */
#define `$INSTANCE_NAME`_LSB_MASK           ((1<<(`$Resolution`-8))-1) /* Used to mask of the LSbs */


/***************************************
*              Constants        
***************************************/

/* SetRange constants */
`#cy_declare_enum iDacRange`
#define `$INSTANCE_NAME`_RANGE_255uA    0x00u
#define `$INSTANCE_NAME`_RANGE_2mA      0x01u

/* SetPolarity constants */
#define `$INSTANCE_NAME`_SOURCE         0x00u
#define `$INSTANCE_NAME`_SINK           0x04u

/* Power setting for SetSpeed API  */
#define `$INSTANCE_NAME`_LOWSPEED       0x00u
#define `$INSTANCE_NAME`_HIGHSPEED      0x02u

/* CR0 iDac Control Register 0 definitions */  

/* Bit Field  DAC_HS_MODE                  */
#define `$INSTANCE_NAME`_HS_MASK        0x02u
#define `$INSTANCE_NAME`_HS_LOWPOWER    0x00u
#define `$INSTANCE_NAME`_HS_HIGHSPEED   0x02u

/* Bit Field  DAC_MODE                  */
#define `$INSTANCE_NAME`_MODE_MASK      0x10u
#define `$INSTANCE_NAME`_MODE_V         0x00u
#define `$INSTANCE_NAME`_MODE_I         0x10u

/* Bit Field  DAC_RANGE                  */
#define `$INSTANCE_NAME`_DAC_RANGE_MASK     0x0Cu
#define `$INSTANCE_NAME`_DAC_RANGE_32uA     0x00u
#define `$INSTANCE_NAME`_DAC_RANGE_255uA    0x04u
#define `$INSTANCE_NAME`_DAC_RANGE_2040uA   0x08u

/* CR1 iDac Control Register 1 definitions */

/* Bit Field  DAC_MX_DATA                  */
#define `$INSTANCE_NAME`_SRC_MASK       0x20u
#define `$INSTANCE_NAME`_SRC_REG        0x00u
#define `$INSTANCE_NAME`_SRC_UDB        0x20u

/* This bit enable reset from UDB array      */
#define `$INSTANCE_NAME`_RESET_MASK     0x10u
#define `$INSTANCE_NAME`_RESET_ENABLE   0x10u
#define `$INSTANCE_NAME`_RESET_DISABLE  0x00u

/* Bit Field  DAC_MX_IDIR_SRC              */
#define `$INSTANCE_NAME`_IDIR_SRC_MASK  0x08u
#define `$INSTANCE_NAME`_IDIR_SRC_REG   0x00u
#define `$INSTANCE_NAME`_IDIR_SRC_UDB   0x08u

/* Bit Field  DAC_I_DIR                  */
/* Register control of current direction      */
#define `$INSTANCE_NAME`_IDIR_MASK      0x04u   
#define `$INSTANCE_NAME`_IDIR_SRC       0x04u
#define `$INSTANCE_NAME`_IDIR_SINK      0x00u

/* Bit Field  DAC_MX_IOFF_SRC                  */
/* Selects source of IOFF control, reg or UDB  */
#define `$INSTANCE_NAME`_IDIR_CTL_MASK  0x02u
#define `$INSTANCE_NAME`_IDIR_CTL_REG   0x00u
#define `$INSTANCE_NAME`_IDIR_CTL_UDB   0x02u

/* Bit Field  DAC_MX_IOFF                   */
/* Register control of IDAC                 */
/* Only valid if IOFF CTL is set to Reg     */
#define `$INSTANCE_NAME`_I_OFF_MASK     0x01u   
#define `$INSTANCE_NAME`_I_OFF          0x00u
#define `$INSTANCE_NAME`_I_ON           0x01u

/* This bit enables data from DAC bus      */
#define `$INSTANCE_NAME`_DACBUS_MASK     0x20u
#define `$INSTANCE_NAME`_DACBUS_ENABLE   0x20u
#define `$INSTANCE_NAME`_DACBUS_DISABLE  0x00u

/* DAC STROBE Strobe Control Register definitions */


/* Bit Field  DAC_MX_STROBE                  */
#define `$INSTANCE_NAME`_STRB_MASK      0x08u
#define `$INSTANCE_NAME`_STRB_EN        0x08u
#define `$INSTANCE_NAME`_STRB_DIS       0x00u

/* PM_ACT_CFG (Active Power Mode CFG Register)     */ 
#define `$INSTANCE_NAME`_MSB_ACT_PWR_EN   `$INSTANCE_NAME`_viDAC8_MSB__PM_ACT_MSK /* Power enable mask */ 
#define `$INSTANCE_NAME`_LSB_ACT_PWR_EN   `$INSTANCE_NAME`_viDAC8_LSB__PM_ACT_MSK /* Power enable mask */ 

/***************************************
*              Registers        
***************************************/

#define `$INSTANCE_NAME`_MSB_CR0    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__CR0 )
#define `$INSTANCE_NAME`_MSB_CR1    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__CR1 )
#define `$INSTANCE_NAME`_MSB_Data   (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__D )
#define `$INSTANCE_NAME`_MSB_Strobe (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__STROBE )
#define `$INSTANCE_NAME`_MSB_SW0    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__SW0 )
#define `$INSTANCE_NAME`_MSB_SW2    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__SW2 )
#define `$INSTANCE_NAME`_MSB_SW3    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__SW3 )
#define `$INSTANCE_NAME`_MSB_SW4    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__SW4 )
#define `$INSTANCE_NAME`_MSB_TR     (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__TR )
#define `$INSTANCE_NAME`_MSB_PWRMGR (* (reg8 *) `$INSTANCE_NAME`_viDAC8_MSB__PM_ACT_CFG )  /* Power manager */

#define `$INSTANCE_NAME`_LSB_CR0    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__CR0 )
#define `$INSTANCE_NAME`_LSB_CR1    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__CR1 )
#define `$INSTANCE_NAME`_LSB_Data   (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__D )
#define `$INSTANCE_NAME`_LSB_Strobe (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__STROBE )
#define `$INSTANCE_NAME`_LSB_SW0    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__SW0 )
#define `$INSTANCE_NAME`_LSB_SW2    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__SW2 )
#define `$INSTANCE_NAME`_LSB_SW3    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__SW3 )
#define `$INSTANCE_NAME`_LSB_SW4    (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__SW4 )
#define `$INSTANCE_NAME`_LSB_TR     (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__TR )
#define `$INSTANCE_NAME`_LSB_PWRMGR (* (reg8 *) `$INSTANCE_NAME`_viDAC8_LSB__PM_ACT_CFG )  /* Power manager */


/***************************************
*              Trim        
***************************************/
#define `$INSTANCE_NAME`_TRIM_M1_RNG0_SRC_OFFSET   0x00u
#define `$INSTANCE_NAME`_TRIM_M2_RNG0_SINK_OFFSET  0x01u
#define `$INSTANCE_NAME`_TRIM_M3_RNG1_SRC_OFFSET   0x02u
#define `$INSTANCE_NAME`_TRIM_M4_RNG1_SINK_OFFSET  0x03u
#define `$INSTANCE_NAME`_TRIM_M5_RNG2_SRC_OFFSET   0x04u
#define `$INSTANCE_NAME`_TRIM_M6_RNG2_SINK_OFFSET  0x05u


#define `$INSTANCE_NAME`_TRIM_M7_4V_RNG_OFFSET  0x07u

/* MSB Trim */
#define `$INSTANCE_NAME`_MSB_DAC_POS  (`$INSTANCE_NAME`_viDAC8_MSB__D - CYDEV_ANAIF_WRK_DAC0_BASE)

#if(`$INSTANCE_NAME`_MSB_DAC_POS == 0)
#define `$INSTANCE_NAME`_MSB_DAC_TRIM_BASE  0x0C011Cu
#endif

#if(`$INSTANCE_NAME`_MSB_DAC_POS == 1)
#define `$INSTANCE_NAME`_MSB_DAC_TRIM_BASE  0x0C012Cu
#endif

#if(`$INSTANCE_NAME`_MSB_DAC_POS == 2)
#define `$INSTANCE_NAME`_MSB_DAC_TRIM_BASE  0x0C0124u
#endif

#if(`$INSTANCE_NAME`_MSB_DAC_POS == 3)
#define `$INSTANCE_NAME`_MSB_DAC_TRIM_BASE  0x0C0134u
#endif

/* LSB Trim */
#define `$INSTANCE_NAME`_LSB_DAC_POS  (`$INSTANCE_NAME`_viDAC8_LSB__D - CYDEV_ANAIF_WRK_DAC0_BASE)

#if(`$INSTANCE_NAME`_LSB_DAC_POS == 0)
#define `$INSTANCE_NAME`_LSB_DAC_TRIM_BASE  0x0C011Cu
#endif

#if(`$INSTANCE_NAME`_LSB_DAC_POS == 1)
#define `$INSTANCE_NAME`_LSB_DAC_TRIM_BASE  0x0C012Cu
#endif

#if(`$INSTANCE_NAME`_LSB_DAC_POS == 2)
#define `$INSTANCE_NAME`_LSB_DAC_TRIM_BASE  0x0C0124u
#endif

#if(`$INSTANCE_NAME`_LSB_DAC_POS == 3)
#define `$INSTANCE_NAME`_LSB_DAC_TRIM_BASE  0x0C0134u
#endif

#endif /* CY_IDAC8_`$INSTANCE_NAME`_H  */
/*  [] END OF FILE  */
