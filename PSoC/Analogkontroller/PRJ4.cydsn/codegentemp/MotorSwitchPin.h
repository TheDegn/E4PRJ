/*******************************************************************************
* File Name: MotorSwitchPin.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_MotorSwitchPin_H) /* Pins MotorSwitchPin_H */
#define CY_PINS_MotorSwitchPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MotorSwitchPin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MotorSwitchPin__PORT == 15 && ((MotorSwitchPin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MotorSwitchPin_Write(uint8 value);
void    MotorSwitchPin_SetDriveMode(uint8 mode);
uint8   MotorSwitchPin_ReadDataReg(void);
uint8   MotorSwitchPin_Read(void);
void    MotorSwitchPin_SetInterruptMode(uint16 position, uint16 mode);
uint8   MotorSwitchPin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MotorSwitchPin_SetDriveMode() function.
     *  @{
     */
        #define MotorSwitchPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MotorSwitchPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MotorSwitchPin_DM_RES_UP          PIN_DM_RES_UP
        #define MotorSwitchPin_DM_RES_DWN         PIN_DM_RES_DWN
        #define MotorSwitchPin_DM_OD_LO           PIN_DM_OD_LO
        #define MotorSwitchPin_DM_OD_HI           PIN_DM_OD_HI
        #define MotorSwitchPin_DM_STRONG          PIN_DM_STRONG
        #define MotorSwitchPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MotorSwitchPin_MASK               MotorSwitchPin__MASK
#define MotorSwitchPin_SHIFT              MotorSwitchPin__SHIFT
#define MotorSwitchPin_WIDTH              1u

/* Interrupt constants */
#if defined(MotorSwitchPin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MotorSwitchPin_SetInterruptMode() function.
     *  @{
     */
        #define MotorSwitchPin_INTR_NONE      (uint16)(0x0000u)
        #define MotorSwitchPin_INTR_RISING    (uint16)(0x0001u)
        #define MotorSwitchPin_INTR_FALLING   (uint16)(0x0002u)
        #define MotorSwitchPin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MotorSwitchPin_INTR_MASK      (0x01u) 
#endif /* (MotorSwitchPin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MotorSwitchPin_PS                     (* (reg8 *) MotorSwitchPin__PS)
/* Data Register */
#define MotorSwitchPin_DR                     (* (reg8 *) MotorSwitchPin__DR)
/* Port Number */
#define MotorSwitchPin_PRT_NUM                (* (reg8 *) MotorSwitchPin__PRT) 
/* Connect to Analog Globals */                                                  
#define MotorSwitchPin_AG                     (* (reg8 *) MotorSwitchPin__AG)                       
/* Analog MUX bux enable */
#define MotorSwitchPin_AMUX                   (* (reg8 *) MotorSwitchPin__AMUX) 
/* Bidirectional Enable */                                                        
#define MotorSwitchPin_BIE                    (* (reg8 *) MotorSwitchPin__BIE)
/* Bit-mask for Aliased Register Access */
#define MotorSwitchPin_BIT_MASK               (* (reg8 *) MotorSwitchPin__BIT_MASK)
/* Bypass Enable */
#define MotorSwitchPin_BYP                    (* (reg8 *) MotorSwitchPin__BYP)
/* Port wide control signals */                                                   
#define MotorSwitchPin_CTL                    (* (reg8 *) MotorSwitchPin__CTL)
/* Drive Modes */
#define MotorSwitchPin_DM0                    (* (reg8 *) MotorSwitchPin__DM0) 
#define MotorSwitchPin_DM1                    (* (reg8 *) MotorSwitchPin__DM1)
#define MotorSwitchPin_DM2                    (* (reg8 *) MotorSwitchPin__DM2) 
/* Input Buffer Disable Override */
#define MotorSwitchPin_INP_DIS                (* (reg8 *) MotorSwitchPin__INP_DIS)
/* LCD Common or Segment Drive */
#define MotorSwitchPin_LCD_COM_SEG            (* (reg8 *) MotorSwitchPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define MotorSwitchPin_LCD_EN                 (* (reg8 *) MotorSwitchPin__LCD_EN)
/* Slew Rate Control */
#define MotorSwitchPin_SLW                    (* (reg8 *) MotorSwitchPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MotorSwitchPin_PRTDSI__CAPS_SEL       (* (reg8 *) MotorSwitchPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MotorSwitchPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MotorSwitchPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MotorSwitchPin_PRTDSI__OE_SEL0        (* (reg8 *) MotorSwitchPin__PRTDSI__OE_SEL0) 
#define MotorSwitchPin_PRTDSI__OE_SEL1        (* (reg8 *) MotorSwitchPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MotorSwitchPin_PRTDSI__OUT_SEL0       (* (reg8 *) MotorSwitchPin__PRTDSI__OUT_SEL0) 
#define MotorSwitchPin_PRTDSI__OUT_SEL1       (* (reg8 *) MotorSwitchPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MotorSwitchPin_PRTDSI__SYNC_OUT       (* (reg8 *) MotorSwitchPin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MotorSwitchPin__SIO_CFG)
    #define MotorSwitchPin_SIO_HYST_EN        (* (reg8 *) MotorSwitchPin__SIO_HYST_EN)
    #define MotorSwitchPin_SIO_REG_HIFREQ     (* (reg8 *) MotorSwitchPin__SIO_REG_HIFREQ)
    #define MotorSwitchPin_SIO_CFG            (* (reg8 *) MotorSwitchPin__SIO_CFG)
    #define MotorSwitchPin_SIO_DIFF           (* (reg8 *) MotorSwitchPin__SIO_DIFF)
#endif /* (MotorSwitchPin__SIO_CFG) */

/* Interrupt Registers */
#if defined(MotorSwitchPin__INTSTAT)
    #define MotorSwitchPin_INTSTAT            (* (reg8 *) MotorSwitchPin__INTSTAT)
    #define MotorSwitchPin_SNAP               (* (reg8 *) MotorSwitchPin__SNAP)
    
	#define MotorSwitchPin_0_INTTYPE_REG 		(* (reg8 *) MotorSwitchPin__0__INTTYPE)
#endif /* (MotorSwitchPin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MotorSwitchPin_H */


/* [] END OF FILE */
