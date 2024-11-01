/*******************************************************************************
* File Name: Counter_Sensor_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter_Sensor.h"

static Counter_Sensor_backupStruct Counter_Sensor_backup;


/*******************************************************************************
* Function Name: Counter_Sensor_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Sensor_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_Sensor_SaveConfig(void) 
{
    #if (!Counter_Sensor_UsingFixedFunction)

        Counter_Sensor_backup.CounterUdb = Counter_Sensor_ReadCounter();

        #if(!Counter_Sensor_ControlRegRemoved)
            Counter_Sensor_backup.CounterControlRegister = Counter_Sensor_ReadControlRegister();
        #endif /* (!Counter_Sensor_ControlRegRemoved) */

    #endif /* (!Counter_Sensor_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_Sensor_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Sensor_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_Sensor_RestoreConfig(void) 
{      
    #if (!Counter_Sensor_UsingFixedFunction)

       Counter_Sensor_WriteCounter(Counter_Sensor_backup.CounterUdb);

        #if(!Counter_Sensor_ControlRegRemoved)
            Counter_Sensor_WriteControlRegister(Counter_Sensor_backup.CounterControlRegister);
        #endif /* (!Counter_Sensor_ControlRegRemoved) */

    #endif /* (!Counter_Sensor_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_Sensor_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Sensor_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_Sensor_Sleep(void) 
{
    #if(!Counter_Sensor_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_Sensor_CTRL_ENABLE == (Counter_Sensor_CONTROL & Counter_Sensor_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_Sensor_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_Sensor_backup.CounterEnableState = 0u;
        }
    #else
        Counter_Sensor_backup.CounterEnableState = 1u;
        if(Counter_Sensor_backup.CounterEnableState != 0u)
        {
            Counter_Sensor_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_Sensor_ControlRegRemoved) */
    
    Counter_Sensor_Stop();
    Counter_Sensor_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_Sensor_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Sensor_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_Sensor_Wakeup(void) 
{
    Counter_Sensor_RestoreConfig();
    #if(!Counter_Sensor_ControlRegRemoved)
        if(Counter_Sensor_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_Sensor_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_Sensor_ControlRegRemoved) */
    
}


/* [] END OF FILE */
