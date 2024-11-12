/*******************************************************************************
* File Name: Sensor_Out_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Sensor_Out.h"

static Sensor_Out_backupStruct Sensor_Out_backup;


/*******************************************************************************
* Function Name: Sensor_Out_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Sensor_Out_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Sensor_Out_SaveConfig(void) 
{

    #if(!Sensor_Out_UsingFixedFunction)
        #if(!Sensor_Out_PWMModeIsCenterAligned)
            Sensor_Out_backup.PWMPeriod = Sensor_Out_ReadPeriod();
        #endif /* (!Sensor_Out_PWMModeIsCenterAligned) */
        Sensor_Out_backup.PWMUdb = Sensor_Out_ReadCounter();
        #if (Sensor_Out_UseStatus)
            Sensor_Out_backup.InterruptMaskValue = Sensor_Out_STATUS_MASK;
        #endif /* (Sensor_Out_UseStatus) */

        #if(Sensor_Out_DeadBandMode == Sensor_Out__B_PWM__DBM_256_CLOCKS || \
            Sensor_Out_DeadBandMode == Sensor_Out__B_PWM__DBM_2_4_CLOCKS)
            Sensor_Out_backup.PWMdeadBandValue = Sensor_Out_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Sensor_Out_KillModeMinTime)
             Sensor_Out_backup.PWMKillCounterPeriod = Sensor_Out_ReadKillTime();
        #endif /* (Sensor_Out_KillModeMinTime) */

        #if(Sensor_Out_UseControl)
            Sensor_Out_backup.PWMControlRegister = Sensor_Out_ReadControlRegister();
        #endif /* (Sensor_Out_UseControl) */
    #endif  /* (!Sensor_Out_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Sensor_Out_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Sensor_Out_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Sensor_Out_RestoreConfig(void) 
{
        #if(!Sensor_Out_UsingFixedFunction)
            #if(!Sensor_Out_PWMModeIsCenterAligned)
                Sensor_Out_WritePeriod(Sensor_Out_backup.PWMPeriod);
            #endif /* (!Sensor_Out_PWMModeIsCenterAligned) */

            Sensor_Out_WriteCounter(Sensor_Out_backup.PWMUdb);

            #if (Sensor_Out_UseStatus)
                Sensor_Out_STATUS_MASK = Sensor_Out_backup.InterruptMaskValue;
            #endif /* (Sensor_Out_UseStatus) */

            #if(Sensor_Out_DeadBandMode == Sensor_Out__B_PWM__DBM_256_CLOCKS || \
                Sensor_Out_DeadBandMode == Sensor_Out__B_PWM__DBM_2_4_CLOCKS)
                Sensor_Out_WriteDeadTime(Sensor_Out_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Sensor_Out_KillModeMinTime)
                Sensor_Out_WriteKillTime(Sensor_Out_backup.PWMKillCounterPeriod);
            #endif /* (Sensor_Out_KillModeMinTime) */

            #if(Sensor_Out_UseControl)
                Sensor_Out_WriteControlRegister(Sensor_Out_backup.PWMControlRegister);
            #endif /* (Sensor_Out_UseControl) */
        #endif  /* (!Sensor_Out_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Sensor_Out_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Sensor_Out_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Sensor_Out_Sleep(void) 
{
    #if(Sensor_Out_UseControl)
        if(Sensor_Out_CTRL_ENABLE == (Sensor_Out_CONTROL & Sensor_Out_CTRL_ENABLE))
        {
            /*Component is enabled */
            Sensor_Out_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Sensor_Out_backup.PWMEnableState = 0u;
        }
    #endif /* (Sensor_Out_UseControl) */

    /* Stop component */
    Sensor_Out_Stop();

    /* Save registers configuration */
    Sensor_Out_SaveConfig();
}


/*******************************************************************************
* Function Name: Sensor_Out_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Sensor_Out_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Sensor_Out_Wakeup(void) 
{
     /* Restore registers values */
    Sensor_Out_RestoreConfig();

    if(Sensor_Out_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Sensor_Out_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
