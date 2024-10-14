#include "project.h"

#include "interface.h"
extern int toggle;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    init_interface();
    CyDelay(1000); 
    
    for(;;)
    {
        
        if(toggle == 1)
        {             
            mode_1();
            CyDelay(160);
        }
        else if (toggle == 2)
        {
            mode_2();
            CyDelay(160);
        }
        else
        {
            mode_3();
            CyDelay(160);  
        }        
    }
}
