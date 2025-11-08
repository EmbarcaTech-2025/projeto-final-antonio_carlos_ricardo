## Relatório Técnico – Testes de Comunicação LoRa em Topologia Ponto-a-Ponto  
**BitDogLab + RFM95W** – Novembro/2025

### 1. Objetivo
Este documento registra os testes realizados com dois rádios LoRa modelo RFM95W utilizando duas placas BitDogLab (RP2040). O objetivo foi validar a comunicação ponto-a-ponto LoRa utilizando firmware próprio em C, com possibilidade de alternar entre modo transmissor (TX) e modo receptor (RX) através dos botões físicos da BitDogLab.

### 2. Montagem e Configuração
- 2 placas BitDogLab v6.3 e v7 com pico w (RP2040)  
- 3 módulos LoRa RFM95W (SX1276), sendo 2 módulos Escola 4.0 e 1 do kit avançado de periféricos  
- Alimentação via USB e bateria (v7)
- Antenas externas conectadas nos dois módulos da Escola 4.0 e pig tail na outra
- Frequência de teste: AU915, canal 5, 915 MHz (compatível Brasil/ANATEL)

A pinagem do RFM95W foi mapeada para o SPI0 do RP2040.  
Confirmamos que os módulos estavam vivos através da leitura do registrador VERSION (0x42), que retornou 0x12 nos três rádios. Isto garantiu que o SPI estava funcional antes dos testes de RF.  

### 3. Firmware e Operação
Foi desenvolvido firmware em C baseado no driver uLora.  
Através dos botões da BDL, selecionou-se a placa em teste e seu modo de operação.  

Setting LoRa board:  

| Botão | Função |
|-------|--------|
| A (GPIO5) | força placa Escola 4.0 |
| B (GPIO6) | força placa do kit |
Setting de modo TX/RX:

| Botão | Função |
|-------|--------|
| A (GPIO5) | força modo RSSI (RX simplificado) |
| B (GPIO6) | força modo RX |
| Votão do Joystick (GPIO22) | força nodo TX |

### 4. Problemas Encontrados e Mitigações
Durante os testes surgiram problemas técnicos reais, relevantes para projetos futuros:

| Problema | Sintoma | Solução aplicada |
|----------|---------|------------------|
| Incompatibilidade de bibliotecas SSD1306 | OLED não inicializava | correção de pinos SDA/SCL e teste isolado do I2C |
| Falta de feedback visual local | difícil saber se device estava TX ou RX | uso de LEDs RGB da BitDogLab para sinalização |
| Conflito ao tentar monitorar as duas placas simultaneamente no mesmo PC | Thonny não abre 2 instâncias por padrão | gravação do client como `main.py` para rodar sozinho sem debugger |

Além disso, foi necessário alinhar ambos os devices para **mesmo SF/BW/CR**. Qualquer divergência de modem config impede recepção mesmo com SPI e RF ok.

### 5. Resultados
Com ambos os firmwares alinhados e antenas conectadas, obtivemos pacotes LoRa ponto-a-ponto estáveis. Foi possível medir **RSSI** no receptor em tempo real através de callback do driver.

Valores observados em bancada (aprox 1 metro de distância):
- RSSI variando entre **-42 dBm** e **-55 dBm** na mesa de laboratório
- SNR entre **9** e **11 dB** (ambiente interno com baixa interferência)

### 6. Conclusões Técnicas
- Confirmou-se que duas BitDogLab podem operar LoRa puro sem LoRaWAN.
- O firmware pode mudar de TX para RX **em tempo real**, sem regravar código.
- A leitura do REG_VERSION=0x12 é o teste mais eficiente para avaliar “rádio vivo” antes da depuração RF.
- A arquitetura é adequada para:
  - testes de alcance
  - exploração de parâmetros de rádio (SF/BW/CR)
  - protótipos meteorológicos e sensores simplex

### 7. Próximos Passos
- adicionar checksum do payload
- testar modulação com SF variável
- integrar ThingsBoard via Gateway LoRaWAN quando migrar de P2P para LoRaWAN

**Conclusão final**: o sistema é operacional, pode medir RSSI/SNR de forma consistente, e serve como base sólida para etapas posteriores de integração em rede LoRaWAN privada ou pública.
