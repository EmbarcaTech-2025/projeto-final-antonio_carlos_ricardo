#include <stdio.h>

#include "hardware/adc.h"
#include "hardware/clocks.h" 
#include "hardware/pll.h" 
#include "hardware/regs/usb.h"
#include "hardware/structs/usb.h"

#include "tusb.h"

#include "../include/code_config.h"
#include "../include/hw_sleep.h"

static bool     hw_pll_usb_on = true;
static bool     hw_ad_on;
static UsbMode  hw_usb_mode;
static uint16_t hw_sleep_minutes;
static uint64_t t_end;
static uint64_t t_now;




//clock_stop(clk_usb);
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




static void hw_set_speed_full(){
    pll_init(
        pll_sys,
        1,          // refdiv
        1500 * MHZ, // VCO = 1500 MHz
        6,          // postdiv1
        2           // postdiv2
    );
    clock_configure(
        clk_sys,
        CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
        CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        125000000,
        125000000
    );

    // liga se o usm mode não for USB_MODE_OFF ou se 
    if(((hw_usb_mode != USB_MODE_OFF)||(hw_ad_on))&&(!hw_pll_usb_on)){
        pll_init(       // Liga o PLL
            pll_usb,
            1,          // refdiv (12 MHz / 1)
            480 * MHZ,  // VCO = 480 MHz
            5,          // postdiv1
            2           // postdiv2
        );
        hw_pll_usb_on = true;
    }
    switch(hw_usb_mode){
        case USB_MODE_OFF:  // não liga
        case USB_MODE_ON:   // já está ligada
            break;
        case USB_MODE_OFF_ON:   // Religa a USB
        /*
            pll_init(       // Liga o PLL
                pll_usb,
                1,          // refdiv (12 MHz / 1)
                480 * MHZ,  // VCO = 480 MHz
                5,          // postdiv1
                2           // postdiv2
            );*/
            clock_configure(clk_usb,    // seta o clock para o do PLL
                0,                  // src (não usado)
                CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                48000000,
                48000000
            );
            stdio_usb_init();   // Reinicializa stdio USB

            // Força reconexão
            usb_hw->sie_ctrl &= ~USB_SIE_CTRL_PULLUP_EN_BITS;
            sleep_ms(50);   // Original era 10
            usb_hw->sie_ctrl |= USB_SIE_CTRL_PULLUP_EN_BITS;
            sleep_ms(5000); // original não tinha
            
            
            
            /*sleep_ms(50); // original não tinha


            for(int i=0;i<50;i++){
                tud_task();
                sleep_ms(10);
            }
            // Aguarda enumeração
            while (!tud_ready()) {
                tud_task();
                sleep_ms(10);
            }

            // Aguarda terminal serial (opcional, mas comum)
            while (!tud_cdc_connected()) {
                tud_task();
                sleep_ms(10);
            }
            break;*/
    }
}

static void hw_set_speed_low(){
    switch(hw_usb_mode){
        case USB_MODE_OFF:  // não desliga pois já esta desliga
        case USB_MODE_ON:   // não desliga
            break;
        case USB_MODE_OFF_ON:   // Desliga USB
            stdio_usb_deinit();
            clock_stop(clk_usb);
            //pll_deinit(pll_usb);
            break;
    }
    if((hw_usb_mode != USB_MODE_ON)&&(hw_pll_usb_on)){  // Só não desliga o pll usb se USB_MODE_ON
        pll_deinit(pll_usb);
        hw_pll_usb_on = false;
    }

    clock_configure(
        clk_sys,
        CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF,
        0,
        12000000,
        12000000
    );
}

void hw_sleep_init(UsbMode usb_mode, bool ad_on, uint16_t sleep_minutes){
    hw_usb_mode      = usb_mode;
    hw_ad_on         = ad_on;
    hw_sleep_minutes = sleep_minutes;

    t_now = timer_hw->timerawl;
    t_end = t_now;
    if(hw_sleep_minutes) t_end += 60000000 * hw_sleep_minutes;
                    else t_end += 10000000;

    if(!HW_SLEEP_LOW_POWER) return;

    // Desativar clk_gpio
    clock_stop(clk_gpout0);
    clock_stop(clk_gpout1);
    clock_stop(clk_gpout2);
    clock_stop(clk_gpout3);

//    adc_run(false);
//    adc_set_temp_sensor_enabled(false);
//    clock_stop(clk_adc);

    switch(hw_usb_mode){
        case USB_MODE_OFF:
            printf("Disable USB forever\n");
            sleep_ms(200);
            stdio_usb_deinit();
            clock_stop(clk_usb);
            //pll_deinit(pll_usb); // será desligado no próximo sleep
            break;
        case USB_MODE_ON:       // não desliga a USB
        case USB_MODE_OFF_ON:   // a USB só é desligada durante o sleep
            break;
    }

    //clock_stop(clk_peri);

    // lock_stop(clk_rtc);

    //rosc_disable();
    //rosc_hw->ctrl = ROSC_CTRL_ENABLE_VALUE_DISABLE;

    sleep_ms(100);
}



void hw_sleep(){
    sleep_ms(100);

    #ifdef ENABLE_GPIO_TEST
    gpio_put(GPIO_TEST_0, true);
    #endif
    if(!HW_SLEEP_LOW_POWER){
        t_now = timer_hw->timerawl;
        while(t_end > t_now)t_now = timer_hw->timerawl;
    }else{
        hw_set_speed_low();

        bool st = false;
        #ifdef ENABLE_GPIO_TEST
        gpio_put(GPIO_TEST_1, st);
        #endif
        t_now = timer_hw->timerawl;
        while(t_end > t_now){
            t_now = timer_hw->timerawl;
            gpio_put(GPIO_TEST_1, st);
            st = !st;
        }
        #ifdef ENABLE_GPIO_TEST
        gpio_put(GPIO_TEST_1, false);
        #endif

        hw_set_speed_full();
    }
    #ifdef ENABLE_GPIO_TEST
    gpio_put(GPIO_TEST_0, false);
    #endif

    if(hw_sleep_minutes) t_end += 60000000 * hw_sleep_minutes;
                    else t_end += 10000000;
}