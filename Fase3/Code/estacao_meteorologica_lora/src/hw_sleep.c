#include "hardware/clocks.h" 
#include "hardware/pll.h" 
#include "../include/code_config.h"
#include "../include/hw_sleep.h"


static void __not_in_flash_func(my_delay_s)(uint32_t delay_s) {
    clock_configure(clk_sys,
        CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
        CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_XOSC_CLKSRC,
        12 * MHZ,      // entrada = XOSC
        //2 * MHZ       // saída desejada 4,5mA
         4 * MHZ       // saída desejada 5,0mA
        //12 * MHZ       // saída desejada 6,3mA
    );

    uint64_t t_end = timer_hw->timerawl + (delay_s * 1000000);
    uint64_t t     = timer_hw->timerawl;
    while(t < t_end) t = timer_hw->timerawl;

    clock_configure(clk_sys,
        CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
        CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        125 * MHZ,   // entrada = PLL_SYS estável em 125 MHz
        125 * MHZ    // saída desejada
    );
}


void hw_sleep_init(){
    if(!HW_SLEEP_LOW_POWER) return;
    pll_deinit(pll_usb);
}

void hw_sleep_min(uint16_t sleep_minutes){
    sleep_ms(100);
    if(HW_SLEEP_LOW_POWER) my_delay_s(60 * sleep_minutes);
    else sleep_ms(60000 * sleep_minutes);
}