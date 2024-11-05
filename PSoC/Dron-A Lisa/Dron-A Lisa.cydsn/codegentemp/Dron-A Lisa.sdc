# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\degn\OneDrive - Aarhus universitet\Skrivebord\AU\E4\PRJ4\Git\E4PRJ\PSoC\Dron-A Lisa\Dron-A Lisa.cydsn\Dron-A Lisa.cyprj
# Date: Mon, 04 Nov 2024 18:35:20 GMT
#set_units -time ns
create_clock -name {Clock_2(routed)} -period 3003000 -waveform {0 1501500} [list [get_pins {ClockBlock/dclk_2}]]
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 25 49} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {UART_1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 53 105} -nominal_period 2166.6666666666665 [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 72065 144145} [list [get_pins {ClockBlock/dclk_glb_2}]]


# Component constraints for C:\Users\degn\OneDrive - Aarhus universitet\Skrivebord\AU\E4\PRJ4\Git\E4PRJ\PSoC\Dron-A Lisa\Dron-A Lisa.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\degn\OneDrive - Aarhus universitet\Skrivebord\AU\E4\PRJ4\Git\E4PRJ\PSoC\Dron-A Lisa\Dron-A Lisa.cydsn\Dron-A Lisa.cyprj
# Date: Mon, 04 Nov 2024 18:35:16 GMT
