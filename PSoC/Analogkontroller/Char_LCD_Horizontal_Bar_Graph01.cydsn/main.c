/******************************************************************************
* File Name: main.c
*
* Version 2.20
*
* Description: 
*  This code example project demonstrates the usage of the Character LCD Component with 
*  the Horizontal Bar Graph feature.
*
* Related Document: 
*  CE195291_Char_LCD_Horizontal_Bar_Graph_PSoC3_4_5LP.pdf
*
* Hardware Dependency: See 
*  CE195291_Char_LCD_Horizontal_Bar_Graph_PSoC3_4_5LP.pdf
*
*******************************************************************************
* Copyright (2017), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death ("High Risk Product"). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/

#include <project.h>

/* The LCD format in characters */
#define LCD_ROWS                (2u)
#define LCD_COLUMNS             (16u)

/* The delay between displaying bar graphs */
#define BARGRAPH_DELAY_MS       (15u)


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  The main function:
*  1. Starts the LCD.
*  3. Shows a start of a demo string on the LCD.
*  4. For each row, displays a bar graph from left to right with the initial position 
*     moving towards right. After each graph, the display is cleared.
*  5. Shows a completion of the demo message.
*
*******************************************************************************/
int main()
{
    uint8 row = 0u;
    uint8 column = 0u;
    uint8 length = 0u;
    
    /* Start the LCD */
    LCD_Start();

    /* Show the demo start message */
    LCD_Position(0u, 0u);
    LCD_PrintString("Horizontal BG");
    LCD_Position(1u, 0u);
    LCD_PrintString("Demo started");
    CyDelay(1000u);

    /* Clear the LCD display */
    LCD_ClearDisplay();
    
    /* Display the bar graph in each row of the LCD */
    for(row = 0u; row < LCD_ROWS; row++)
    {
        /* Display the bar graph from left to right with moving towards right */
        for(column = 0u; column <= LCD_COLUMNS; column++)
        {
            for(length = 0u; length < (LCD_CHARACTER_WIDTH * (LCD_COLUMNS - column)); length++)
            {
                LCD_DrawHorizontalBG(row, column, LCD_COLUMNS - column, length);
                CyDelay(BARGRAPH_DELAY_MS);
            }
            
            /* Clear the display */
            LCD_ClearDisplay();
        }
    }
    
    /* Wait for some time */
    CyDelay(500u);
    
    /* Show the demo completion message */
    LCD_Position(0u, 0u);
    LCD_PrintString("Horizontal BG");
    LCD_Position(1u, 0u);
    LCD_PrintString("Demo completed");
    
    for(;;)
    {
        
    }
}


/* [] END OF FILE */
