/*******************************************************************************
* File Name: Sensor_Out.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Sensor_Out.h"

/* Error message for removed <resource> through optimization */
#ifdef Sensor_Out_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* Sensor_Out_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 Sensor_Out_initVar = 0u;


/*******************************************************************************
* Function Name: Sensor_Out_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Sensor_Out_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Sensor_Out_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(Sensor_Out_initVar == 0u)
    {
        Sensor_Out_Init();
        Sensor_Out_initVar = 1u;
    }
    Sensor_Out_Enable();

}


/*******************************************************************************
* Function Name: Sensor_Out_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  Sensor_Out_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Sensor_Out_Init(void) 
{
    #if (Sensor_Out_UsingFixedFunction || Sensor_Out_UseControl)
        uint8 ctrl;
    #endif /* (Sensor_Out_UsingFixedFunction || Sensor_Out_UseControl) */

    #if(!Sensor_Out_UsingFixedFunction)
        #if(Sensor_Out_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 Sensor_Out_interruptState;
        #endif /* (Sensor_Out_UseStatus) */
    #endif /* (!Sensor_Out_UsingFixedFunction) */

    #if (Sensor_Out_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        Sensor_Out_CONTROL |= Sensor_Out_CFG0_MODE;
        #if (Sensor_Out_DeadBand2_4)
            Sensor_Out_CONTROL |= Sensor_Out_CFG0_DB;
        #endif /* (Sensor_Out_DeadBand2_4) */

        ctrl = Sensor_Out_CONTROL3 & ((uint8 )(~Sensor_Out_CTRL_CMPMODE1_MASK));
        Sensor_Out_CONTROL3 = ctrl | Sensor_Out_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Sensor_Out_RT1 &= ((uint8)(~Sensor_Out_RT1_MASK));
        Sensor_Out_RT1 |= Sensor_Out_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        Sensor_Out_RT1 &= ((uint8)(~Sensor_Out_SYNCDSI_MASK));
        Sensor_Out_RT1 |= Sensor_Out_SYNCDSI_EN;

    #elif (Sensor_Out_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = Sensor_Out_CONTROL & ((uint8)(~Sensor_Out_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~Sensor_Out_CTRL_CMPMODE1_MASK));
        Sensor_Out_CONTROL = ctrl | Sensor_Out_DEFAULT_COMPARE2_MODE |
                                   Sensor_Out_DEFAULT_COMPARE1_MODE;
    #endif /* (Sensor_Out_UsingFixedFunction) */

    #if (!Sensor_Out_UsingFixedFunction)
        #if (Sensor_Out_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            Sensor_Out_AUX_CONTROLDP0 |= (Sensor_Out_AUX_CTRL_FIFO0_CLR);
        #else /* (Sensor_Out_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            Sensor_Out_AUX_CONTROLDP0 |= (Sensor_Out_AUX_CTRL_FIFO0_CLR);
            Sensor_Out_AUX_CONTROLDP1 |= (Sensor_Out_AUX_CTRL_FIFO0_CLR);
        #endif /* (Sensor_Out_Resolution == 8) */

        Sensor_Out_WriteCounter(Sensor_Out_INIT_PERIOD_VALUE);
    #endif /* (!Sensor_Out_UsingFixedFunction) */

    Sensor_Out_WritePeriod(Sensor_Out_INIT_PERIOD_VALUE);

        #if (Sensor_Out_UseOneCompareMode)
            Sensor_Out_WriteCompare(Sensor_Out_INIT_COMPARE_VALUE1);
        #else
            Sensor_Out_WriteCompare1(Sensor_Out_INIT_COMPARE_VALUE1);
            Sensor_Out_WriteCompare2(Sensor_Out_INIT_COMPARE_VALUE2);
        #endif /* (Sensor_Out_UseOneCompareMode) */

        #if (Sensor_Out_KillModeMinTime)
            Sensor_Out_WriteKillTime(Sensor_Out_MinimumKillTime);
        #endif /* (Sensor_Out_KillModeMinTime) */

        #if (Sensor_Out_DeadBandUsed)
            Sensor_Out_WriteDeadTime(Sensor_Out_INIT_DEAD_TIME);
        #endif /* (Sensor_Out_DeadBandUsed) */

    #if (Sensor_Out_UseStatus || Sensor_Out_UsingFixedFunction)
        Sensor_Out_SetInterruptMode(Sensor_Out_INIT_INTERRUPTS_MODE);
    #endif /* (Sensor_Out_UseStatus || Sensor_Out_UsingFixedFunction) */

    #if (Sensor_Out_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        Sensor_Out_GLOBAL_ENABLE |= Sensor_Out_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        Sensor_Out_CONTROL2 |= Sensor_Out_CTRL2_IRQ_SEL;
    #else
        #if(Sensor_Out_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Sensor_Out_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Sensor_Out_STATUS_AUX_CTRL |= Sensor_Out_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(Sensor_Out_interruptState);

            /* Clear the FIFO to enable the Sensor_Out_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            Sensor_Out_ClearFIFO();
        #endif /* (Sensor_Out_UseStatus) */
    #endif /* (Sensor_Out_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Sensor_Out_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void Sensor_Out_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Sensor_Out_UsingFixedFunction)
        Sensor_Out_GLOBAL_ENABLE |= Sensor_Out_BLOCK_EN_MASK;
        Sensor_Out_GLOBAL_STBY_ENABLE |= Sensor_Out_BLOCK_STBY_EN_MASK;
    #endif /* (Sensor_Out_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (Sensor_Out_UseControl || Sensor_Out_UsingFixedFunction)
        Sensor_Out_CONTROL |= Sensor_Out_CTRL_ENABLE;
    #endif /* (Sensor_Out_UseControl || Sensor_Out_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Sensor_Out_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void Sensor_Out_Stop(void) 
{
    #if (Sensor_Out_UseControl || Sensor_Out_UsingFixedFunction)
        Sensor_Out_CONTROL &= ((uint8)(~Sensor_Out_CTRL_ENABLE));
    #endif /* (Sensor_Out_UseControl || Sensor_Out_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (Sensor_Out_UsingFixedFunction)
        Sensor_Out_GLOBAL_ENABLE &= ((uint8)(~Sensor_Out_BLOCK_EN_MASK));
        Sensor_Out_GLOBAL_STBY_ENABLE &= ((uint8)(~Sensor_Out_BLOCK_STBY_EN_MASK));
    #endif /* (Sensor_Out_UsingFixedFunction) */
}

#if (Sensor_Out_UseOneCompareMode)
    #if (Sensor_Out_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Sensor_Out_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Sensor_Out_SetCompareMode(uint8 comparemode) 
        {
            #if(Sensor_Out_UsingFixedFunction)

                #if(0 != Sensor_Out_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << Sensor_Out_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != Sensor_Out_CTRL_CMPMODE1_SHIFT) */

                Sensor_Out_CONTROL3 &= ((uint8)(~Sensor_Out_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                Sensor_Out_CONTROL3 |= comparemodemasked;

            #elif (Sensor_Out_UseControl)

                #if(0 != Sensor_Out_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << Sensor_Out_CTRL_CMPMODE1_SHIFT)) &
                                                Sensor_Out_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & Sensor_Out_CTRL_CMPMODE1_MASK;
                #endif /* (0 != Sensor_Out_CTRL_CMPMODE1_SHIFT) */

                #if(0 != Sensor_Out_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << Sensor_Out_CTRL_CMPMODE2_SHIFT)) &
                                               Sensor_Out_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & Sensor_Out_CTRL_CMPMODE2_MASK;
                #endif /* (0 != Sensor_Out_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                Sensor_Out_CONTROL &= ((uint8)(~(Sensor_Out_CTRL_CMPMODE1_MASK |
                                            Sensor_Out_CTRL_CMPMODE2_MASK)));
                Sensor_Out_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (Sensor_Out_UsingFixedFunction) */
        }
    #endif /* Sensor_Out_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (Sensor_Out_CompareMode1SW)


        /*******************************************************************************
        * Function Name: Sensor_Out_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Sensor_Out_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != Sensor_Out_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << Sensor_Out_CTRL_CMPMODE1_SHIFT)) &
                                           Sensor_Out_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & Sensor_Out_CTRL_CMPMODE1_MASK;
            #endif /* (0 != Sensor_Out_CTRL_CMPMODE1_SHIFT) */

            #if (Sensor_Out_UseControl)
                Sensor_Out_CONTROL &= ((uint8)(~Sensor_Out_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                Sensor_Out_CONTROL |= comparemodemasked;
            #endif /* (Sensor_Out_UseControl) */
        }
    #endif /* Sensor_Out_CompareMode1SW */

#if (Sensor_Out_CompareMode2SW)


    /*******************************************************************************
    * Function Name: Sensor_Out_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Sensor_Out_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != Sensor_Out_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << Sensor_Out_CTRL_CMPMODE2_SHIFT)) &
                                                 Sensor_Out_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & Sensor_Out_CTRL_CMPMODE2_MASK;
        #endif /* (0 != Sensor_Out_CTRL_CMPMODE2_SHIFT) */

        #if (Sensor_Out_UseControl)
            Sensor_Out_CONTROL &= ((uint8)(~Sensor_Out_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            Sensor_Out_CONTROL |= comparemodemasked;
        #endif /* (Sensor_Out_UseControl) */
    }
    #endif /*Sensor_Out_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!Sensor_Out_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Sensor_Out_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void Sensor_Out_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(Sensor_Out_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(Sensor_Out_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(Sensor_Out_CAPTURE_LSB_PTR));
    }

    #if (Sensor_Out_UseStatus)


        /*******************************************************************************
        * Function Name: Sensor_Out_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void Sensor_Out_ClearFIFO(void) 
        {
            while(0u != (Sensor_Out_ReadStatusRegister() & Sensor_Out_STATUS_FIFONEMPTY))
            {
                (void)Sensor_Out_ReadCapture();
            }
        }

    #endif /* Sensor_Out_UseStatus */

#endif /* !Sensor_Out_UsingFixedFunction */


/*******************************************************************************
* Function Name: Sensor_Out_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void Sensor_Out_WritePeriod(uint8 period) 
{
    #if(Sensor_Out_UsingFixedFunction)
        CY_SET_REG16(Sensor_Out_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(Sensor_Out_PERIOD_LSB_PTR, period);
    #endif /* (Sensor_Out_UsingFixedFunction) */
}

#if (Sensor_Out_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Sensor_Out_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void Sensor_Out_WriteCompare(uint8 compare) \
                                       
    {
        #if(Sensor_Out_UsingFixedFunction)
            CY_SET_REG16(Sensor_Out_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(Sensor_Out_COMPARE1_LSB_PTR, compare);
        #endif /* (Sensor_Out_UsingFixedFunction) */

        #if (Sensor_Out_PWMMode == Sensor_Out__B_PWM__DITHER)
            #if(Sensor_Out_UsingFixedFunction)
                CY_SET_REG16(Sensor_Out_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(Sensor_Out_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (Sensor_Out_UsingFixedFunction) */
        #endif /* (Sensor_Out_PWMMode == Sensor_Out__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: Sensor_Out_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Sensor_Out_WriteCompare1(uint8 compare) \
                                        
    {
        #if(Sensor_Out_UsingFixedFunction)
            CY_SET_REG16(Sensor_Out_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(Sensor_Out_COMPARE1_LSB_PTR, compare);
        #endif /* (Sensor_Out_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: Sensor_Out_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Sensor_Out_WriteCompare2(uint8 compare) \
                                        
    {
        #if(Sensor_Out_UsingFixedFunction)
            CY_SET_REG16(Sensor_Out_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(Sensor_Out_COMPARE2_LSB_PTR, compare);
        #endif /* (Sensor_Out_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (Sensor_Out_DeadBandUsed)


    /*******************************************************************************
    * Function Name: Sensor_Out_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Sensor_Out_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!Sensor_Out_DeadBand2_4)
            CY_SET_REG8(Sensor_Out_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            Sensor_Out_DEADBAND_COUNT &= ((uint8)(~Sensor_Out_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(Sensor_Out_DEADBAND_COUNT_SHIFT)
                Sensor_Out_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << Sensor_Out_DEADBAND_COUNT_SHIFT)) &
                                                    Sensor_Out_DEADBAND_COUNT_MASK;
            #else
                Sensor_Out_DEADBAND_COUNT |= deadtime & Sensor_Out_DEADBAND_COUNT_MASK;
            #endif /* (Sensor_Out_DEADBAND_COUNT_SHIFT) */

        #endif /* (!Sensor_Out_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!Sensor_Out_DeadBand2_4)
            return (CY_GET_REG8(Sensor_Out_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(Sensor_Out_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(Sensor_Out_DEADBAND_COUNT & Sensor_Out_DEADBAND_COUNT_MASK)) >>
                                                                           Sensor_Out_DEADBAND_COUNT_SHIFT));
            #else
                return (Sensor_Out_DEADBAND_COUNT & Sensor_Out_DEADBAND_COUNT_MASK);
            #endif /* (Sensor_Out_DEADBAND_COUNT_SHIFT) */
        #endif /* (!Sensor_Out_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (Sensor_Out_UseStatus || Sensor_Out_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Sensor_Out_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Sensor_Out_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(Sensor_Out_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(Sensor_Out_STATUS_PTR));
    }

#endif /* (Sensor_Out_UseStatus || Sensor_Out_UsingFixedFunction) */


#if (Sensor_Out_UseControl)


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(Sensor_Out_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: Sensor_Out_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Sensor_Out_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(Sensor_Out_CONTROL_PTR, control);
    }

#endif /* (Sensor_Out_UseControl) */


#if (!Sensor_Out_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadCapture(void) 
    {
        return (CY_GET_REG8(Sensor_Out_CAPTURE_LSB_PTR));
    }

#endif /* (!Sensor_Out_UsingFixedFunction) */


#if (Sensor_Out_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadCompare(void) 
    {
        #if(Sensor_Out_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(Sensor_Out_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(Sensor_Out_COMPARE1_LSB_PTR));
        #endif /* (Sensor_Out_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadCompare1(void) 
    {
        return (CY_GET_REG8(Sensor_Out_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadCompare2(void) 
    {
        return (CY_GET_REG8(Sensor_Out_COMPARE2_LSB_PTR));
    }

#endif /* (Sensor_Out_UseOneCompareMode) */


/*******************************************************************************
* Function Name: Sensor_Out_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 Sensor_Out_ReadPeriod(void) 
{
    #if(Sensor_Out_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Sensor_Out_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Sensor_Out_PERIOD_LSB_PTR));
    #endif /* (Sensor_Out_UsingFixedFunction) */
}

#if ( Sensor_Out_KillModeMinTime)


    /*******************************************************************************
    * Function Name: Sensor_Out_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Sensor_Out_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(Sensor_Out_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: Sensor_Out_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 Sensor_Out_ReadKillTime(void) 
    {
        return (CY_GET_REG8(Sensor_Out_KILLMODEMINTIME_PTR));
    }

#endif /* ( Sensor_Out_KillModeMinTime) */

/* [] END OF FILE */
