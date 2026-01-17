#include "hardware/adc.h"
#include "hardware/clocks.h" 
#include "../include/aq_data_cpu_temp.h"
#include "../include/loop_printf.h"


int aqdatacpu_temp_power_on(){
    adc_run(false);
    adc_set_temp_sensor_enabled(false);
    clock_stop(clk_adc);
    return 0;
}
int aqdatacpu_temp_init_aq(){
    clock_configure(
        clk_adc,
        0, // sem glitchless mux
        CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
        48000000, // frequência da fonte (PLL USB)
        48000000  // frequência final do ADC
    );
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(ADC_TEMPERATURE_CHANNEL_NUM);
    return 0;
}
int aqdatacpu_temp_read(int *temp){
    uint16_t ad = adc_read();
    //t=27 - (ADC_Voltage - 0.706)/0.001721
    *temp = (27.0 - ((3.3*ad/4096) - 0.706)/0.001721) * 10.0;
    loop_printf("CPU Temp = %3.1f Celsius\n", *temp * 0.1);
    return 0;
}
int aqdatacpu_temp_sleep(){
    adc_run(false);
    adc_set_temp_sensor_enabled(false);
    clock_stop(clk_adc);
    return 0;
}