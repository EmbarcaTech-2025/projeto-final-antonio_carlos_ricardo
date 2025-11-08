/**
 * @file    wifi.c
 * @author  Carlos e Ricardo
 * @brief   Inicializa, Atualiza, Conecta e Desconecta o módulo de WiFi
 * @version 0.1
 * @date    2025-09-05
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/cyw43_arch.h>
#include "../include/config.h"
#include "../include/config_data.h"
#include "../include/wifi.h"


#define SW_PRINT_DEBUG          false

static bool wifi_connected     = false;
static bool wifi_hw_was_inited = false;

/**
 * @brief Desinicializa o módulo de WiFi caso tenha sido inicializado previamente
 * 
 */
static void wifi_disconnect() {
    // Desativa o Driver se previamente inicializado
    if(wifi_hw_was_inited){
        if(SW_PRINT_DEBUG) printf("DeInit WiFi Hardware\n");
        cyw43_arch_deinit();
        sleep_ms(WIFI_DEINIT_DELAY_MS);
        wifi_hw_was_inited = false;
    }
}

/**
 * @brief Tenta fazer a Conexão com o roteador WiFi
 * 
 * @return true, conseguiu conectar 
 * @return false, NÃO conseguiu conectar 
 */
static bool wifi_connect_or_reconnected() {
    wifi_disconnect();

    if(SW_PRINT_DEBUG) printf("Init WiFi Hardware\n");
    wifi_hw_was_inited = true;
    if (cyw43_arch_init()) {
        if(SW_PRINT_DEBUG) printf("error initializing WiFi hardware\n");
        return false;
    }
    if(SW_PRINT_DEBUG) printf("Initialized WiFi Hardware\n");

    // Habilita o modo estação (STA) para se conectar a um ponto de acesso.
    cyw43_arch_enable_sta_mode();
    if(SW_PRINT_DEBUG) printf("WiFi STA mode enabled\n");

    // tenta fazer a conexão
    if (cyw43_arch_wifi_connect_timeout_ms(config_data.wifi_ssid, config_data.wifi_psw, CYW43_AUTH_WPA2_AES_PSK, WIFI_CONNECTION_TIMEOUT_MSW)) {
        if(SW_PRINT_DEBUG) printf("WiFi connection FAIL\n");
        return false;
    }

    // Caso o IP obtido for 0.0.0.0 houve um erro
    if(cyw43_state.netif[0].ip_addr.addr == 0){
        if(SW_PRINT_DEBUG) printf("WiFi Invalid IP = 0.0.0.0\n");
        return false;
    }

    uint8_t *ip   = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
    printf("WiFi connected, IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
    
    return true;
}



/**
 * @brief Inicializa variáveis
 * 
 */
void wifi_init(){
    wifi_connected = false;
}

/**
 * @brief Atualiza o estado do WiFi
 * - Se conectado verifica se continua conectado ao roteador
 * - Se desconectado tenta reconectar ao roteador
 * @return true, está conectado
 * @return false, está desconectado
 */
bool wifi_update(){
    if(wifi_connected){
        if(cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) != CYW43_LINK_UP){
            wifi_connected = false;
            sleep_ms(WIFI_AFTER_CONNECT_FAIL_DELAY_MS);
        }
    }else{
        printf("WiFi desconnected\n");
        wifi_connected = wifi_connect_or_reconnected();
        if (!wifi_connected) {
            sleep_ms(WIFI_AFTER_CONNECT_FAIL_DELAY_MS);  // Pequena espera antes de tentar de novo
        }
    }

    return wifi_connected;
}