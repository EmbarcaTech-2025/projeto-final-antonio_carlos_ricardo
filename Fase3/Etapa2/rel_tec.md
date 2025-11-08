## Relatório Técnico – Testes de Comunicação LoRa em Topologia Ponto-a-Ponto  
**BitDogLab + RFM95W** – Novembro/2025

### 1. Objetivo
Este documento registra os testes realizados com dois rádios LoRa modelo RFM95W utilizando duas placas BitDogLab (RP2040). O objetivo foi validar a comunicação ponto-a-ponto LoRa utilizando firmware próprio em MicroPython, com possibilidade de alternar entre **modo transmissor (TX)** e **modo receptor (RX)** através dos botões físicos da BitDogLab.

### 2. Montagem e Configuração
- 2 placas BitDogLab v7 (RP2040)
- 2 módulos LoRa Semtech SX1276/RFM95W
- Alimentação via USB
- Antenas externas conectadas nos dois módulos
- Frequência de teste: **915 MHz** (compatível Brasil/ANATEL)

A pinagem do RFM95W foi mapeada para o SPI0 do RP2040.  
Confirmamos que os módulos estavam vivos através da leitura do registrador **VERSION (0x42)**, que retornou **0x12** em ambos os rádios (padrão do SX1276). Isto garantiu que o SPI estava funcional antes dos testes de RF.

### 3. Firmware e Operação
Foi desenvolvido firmware MicroPython baseado no driver uLora.  
Por opção de simplificação operacional, o firmware identifica o modo de operação através dos botões da BitDogLab:

| Botão | Função |
|-------|--------|
| A (GPIO5) | força modo TX |
| B (GPIO6) | força modo RX |

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
