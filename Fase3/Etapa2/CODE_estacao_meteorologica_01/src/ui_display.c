/**
 * @file    ui_display.c
 * @author  Carlos e Ricardo
 * @brief   Inicializa display OLED e mostra dados da estação
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "../include/config.h"
#include "../include/config_data.h"
#include "../include/estation_data.h"
#include "../include/ui_display.h"
#include "../lib/oled/display.h"



/**
 * @brief Escreve o conteudo de um sensor específico em uma determinada linha
 * 
 * @param estation_data, ponteiro dos dados da estação
 * @param sensor, sensor especifico 
 * @param pos, posição no display
 */
static void draw_line(EstationData * estation_data, int16_t sensor, int16_t pos){
    char str[17] = {0};
    switch(sensor){
        case 0: snprintf(str, 17, "Build : %8s",  SW_BUILD); break;
        case 1: return;
        case 2: snprintf(str, 17, "%s",         config_data.station_name);      break;
        case 3: snprintf(str, 17, "%s",         config_data.station_token);     break;
        case 4: snprintf(str, 17, "%s",         config_data.station_group);     break;
        case 5: snprintf(str, 17, "lat :%11s",  config_data.station_latitude);  break;
        case 6: snprintf(str, 17, "lon :%11s",  config_data.station_longitude); break;
        case 7: snprintf(str, 17, "elev:%9s m", config_data.station_elevation); break;

        case 8: 
            if(estation_data->press  == 0xFFFFFFFF) snprintf(str, 17, "Press:");
                                               else snprintf(str, 17, "Press:%7.2fhPa", estation_data->press * 0.01);
            break;
        case 9: 
            if(estation_data->temp_p == 0xFFFFFFFF) snprintf(str, 17, "Tempp:");
                                               else snprintf(str, 17, "Tempp:%7.2f", estation_data->temp_p * 0.01);
            break;
        case 10: 
            if(estation_data->temp_p == 0xFFFFFFFF) snprintf(str, 17, "Humid:");
                                               else snprintf(str, 17, "Humid:%7.2f \%", estation_data->humidity * 0.01);
            break;
        case 11: 
            if(estation_data->temp_h == 0xFFFFFFFF) snprintf(str, 17, "Temph:");
                                               else snprintf(str, 17, "Temph:%7.2f", estation_data->temp_h * 0.01);
            break;
        case 12: 
            if(estation_data->luminos == 0xFFFFFFFF) snprintf(str, 17, "Lumin:");
                                                else snprintf(str, 17, "Lumin:%7d lx", estation_data->luminos);
            break;
//          "\"lumi_c": tbd, 
        case 13: 
            if(estation_data->wind_dir   == 0xFFFFFFFF) snprintf(str, 17, "WindD:");
                                                   else snprintf(str, 17, "WindD:%7d g", estation_data->wind_dir / 100);
            break;
        case 14: 
            if(estation_data->wind_speed == 0xFFFFFFFF) snprintf(str, 17, "WindS:");
                                                   else snprintf(str, 17, "WindS:%7.2fm/s", estation_data->wind_speed * 0.01);
            break;
        case 15: 
            if(estation_data->rain_intensity == 0xFFFFFFFF) snprintf(str, 17, "Rain :");
                                                       else snprintf(str, 17, "Rain :%7.2f", estation_data->rain_intensity * 0.01);
            break;
        case 16: 
            if(estation_data->analog_ar_co == 0xFFFFFFFF) snprintf(str, 17, "CO   :");
                                                     else snprintf(str, 17, "CO   :%7dppm", estation_data->analog_ar_co / 100);
            break;
        case 17: 
            if(estation_data->analog_ar_co2 == 0xFFFFFFFF) snprintf(str, 17, "CO2  :");
                                                      else snprintf(str, 17, "CO2  :%7dppm", estation_data->analog_ar_co2 / 100);
            break;
        case 18: 
            if(estation_data->analog_ar_o3 == 0xFFFFFFFF) snprintf(str, 17, "O3   :");
                                                     else snprintf(str, 17, "O3   :%7dppm", estation_data->analog_ar_o3 / 100);
            break;
        case 19: 
            if(estation_data->soil_humidity == 0xFFFFFFFF) snprintf(str, 17, "SoilH:");
                                                      else snprintf(str, 17, "SoilH:%7.2f", estation_data->soil_humidity * 0.01);
            break;
        case 20: 
            if(estation_data->soil_temperature == 0xFFFFFFFF) snprintf(str, 17, "SoilT:");
                                                         else snprintf(str, 17, "SoilT:%7.2f", estation_data->soil_temperature * 0.01);
            break;
        case 21: 
            if(estation_data->uv_index == 0xFFFFFFFF) snprintf(str, 17, "UV In:");
                                                 else snprintf(str, 17, "UV In:%7.2f", estation_data->uv_index * 0.01);
            break;
        default: return;
    }
    display_msg(false, 0, pos,  str);
}

/**
 * @brief Inicializa Display OLED, e pode fazer checagem visual
 * 
 * @param only_init, se true não faz checagem visual
 */
void ui_display_init(bool only_init){
    display_init();

    if(!only_init){
        for(int i=0;i<3;i++){
            display_fill(true, true);
            sleep_ms(250);
            display_fill(false, true);
            sleep_ms(250);
        }
        //                        "1234567890123456"
        display_msg(false, 0, 0,  "   BITDOGLAB");
        display_msg(false, 0, 2,  "    ESTACAO");
        display_msg(false, 0, 3,  " METEOROLOGICA");
        display_msg(false, 0, 4,  "       01");
        display_msg(false, 0, 7,  "     BUILD:");
        //display_msg(true, 12, 7,            "0000");
        display_msg(true, 12, 7, SW_BUILD);
        sleep_ms(2500);
    }else{
        display_fill(false, true);
    }
}


static int pos = 8;
/**
 * @brief Atualiza display com os dados da estação
 * 
 * @param estation_data, ponteiro dos dados da estação
 * @param change_pos, muda a posição dos dados a serem mostrados, pode ser positivo, negativo ou zero 
 */
void ui_display_update(EstationData * estation_data, int8_t change_pos){
    // FALTA checar valores NULL

    if(change_pos > 0) pos++;
    if(change_pos < 0) pos--;

    if(pos < 0)        pos = 0;
    if(pos > (21 - 3)) pos = (21 - 3);

    char str[17];

    uint64_t timestamp = estation_data->t_aquire;
    struct tm t;
    gmtime_r((time_t*)&timestamp, &t);
    

    display_fill(false, false);
    //           "1234567890123456"
    sprintf(str, "DATA: %02d/%02d/%04d", t.tm_mday, t.tm_mon + 1, t.tm_year - 100 + 2000);
    display_msg(false, 0, 0, str);
    
    //sprintf(str, "TIME: %02d:%02d'%02d\"", t.tm_hour, t.tm_min, t.tm_sec);
    sprintf(str, "TIME:   %02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
    display_msg(false, 0, 1,  str);

    sprintf(str, "CYCLE: %9llu", estation_data->cycle);
    display_msg(false, 0, 2, str);

    for(int i=0; i<=3;i++){
        draw_line(estation_data, pos + i, i + 4);
    }

    display_update();
}