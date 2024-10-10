# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\degn\OneDrive - Aarhus universitet\Skrivebord\AU\E4\PRJ4\Git\E4PRJ\PSoC\Analogkontroller\PRJ4.cydsn\PRJ4.cyprj
# Date: Tue, 08 Oct 2024 10:56:18 GMT
#set_units -time ns
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {ADC_SAR_1_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 15 31} [list [get_pins {ClockBlock/aclk_glb_0}]]
create_generated_clock -name {UART_1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 53 105} -nominal_period 2166.6666666666665 [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2401 4801} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {Clock_3} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2401 4801} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 120001 240001} [list [get_pins {ClockBlock/dclk_glb_3}]]


# Component constraints for C:\Users\degn\OneDrive - Aarhus universitet\Skrivebord\AU\E4\PRJ4\Git\E4PRJ\PSoC\Analogkontroller\PRJ4.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\degn\OneDrive - Aarhus universitet\Skrivebord\AU\E4\PRJ4\Git\E4PRJ\PSoC\Analogkontroller\PRJ4.cydsn\PRJ4.cyprj
# Date: Tue, 08 Oct 2024 10:56:13 GMT