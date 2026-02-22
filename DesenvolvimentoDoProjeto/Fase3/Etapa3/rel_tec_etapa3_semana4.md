# **Relatório Técnico – Etapa 3 – Semana 4**
## **Validação em Campo, Planejamento de Escala e Consolidação da Versão Beta**

**Projeto:** Estação Meteorológica IoT  
**Período:** 16 a 22 de dezembro de 2025  
**Autores:** Antonio Crepaldi – Carlos Perez – Ricardo Furlan  

---

## **1. Introdução**

A Semana 4 da Etapa 3 teve como objetivo consolidar os avanços técnicos obtidos nas semanas anteriores, com foco na **preparação da versão beta da Estação Meteorológica IoT para implantação em campo**.

Considerando a meta do projeto de instalar **10 estações no campus da Unicamp**, conectadas via **LoRaWAN ao gateway da FEEC**, utilizando **The Things Network (TTN)** e **ThingsBoard** para visualização, esta semana foi dedicada a:

- replicação do processo de compilação do programa original do módulo WCM;
- desenvolbimento de uma versão básica de estudo da transmissão LoRaWAN (ABP e OTA) utilizando a biblioteca LMIC;
- desenvolvimento de uma nova proposta do código do WCM visando simplificação da comunicação e habilitação do sleep mode para LoRaWAN ABP;
- definição do fluxo de dados ponta a ponta;
- consolidação das decisões técnicas tomadas na Etapa 3;
- preparação da documentação e da apresentação solicitadas.

Embora a integração física completa ainda esteja em fase final, os elementos críticos para a operação real do sistema foram analisados, validados conceitualmente e documentados.

---

## **2. Arquitetura Final do Sistema (Versão Beta)**

### **2.1 Visão Geral**

A arquitetura proposta para a versão beta da estação é composta pelos seguintes blocos:

- **Estação remota (nó sensor):**
  - BitDogLab v 7 (RP2040);
  - Sensores meteorológicos BME280, sensor de monitoramento da bateria (INA226); 
  - GPS (UART);
  - Módulo WCM (ESP32-C3 e RFM95W);
  - Sistema de alimentação por bateria com recarga por placa solar. 

- **Infraestrutura de comunicação:**
  - Gateway LoRaWAN da FEEC;
  - Configuração de novos end devices no TTN, inclusive com ABP mode;
  - The Things Network (TTN).

- **Backend e visualização:**
  - Integração via MQTT/HTTP;
  - ThingsBoard para dashboards e análise.

Essa arquitetura foi escolhida por equilibrar **baixo consumo**, **alcance**, **escalabilidade** e **uso de infraestrutura já existente no campus**.

---

## **3. Planejamento da Implantação no Campus da Unicamp**

### **3.1 Estratégia de Escala (10 Estações)**

Para a instalação de múltiplas estações, foram considerados os seguintes aspectos:

- cobertura do gateway LoRaWAN da FEEC;
- espaçamento entre nós sensores;
- limitação de duty-cycle imposta pelo LoRaWAN;
- organização lógica das estações no backend.

Cada estação possuirá:

- Device address único e respctivas chaves.

Isso permitirá:

- diferenciação clara das fontes de dados;
- criação de dashboards individuais e agregados.

---

## **4. Fluxo de Dados Ponta a Ponta**

O fluxo de dados definido para a versão beta é:

1. Leitura periódica dos sensores pela BitDogLab;
2. Montagem do payload binário otimizado;
3. Transmissão via LoRaWAN (uplink);
4. Recepção pelo gateway da FEEC;
5. Encaminhamento para o TTN (nuvem);
6. Integração com ThingsBoard;
7. Exibição em dashboards gráficos.

Esse fluxo está alinhado com as boas práticas de IoT de baixo consumo.

---

## **5. Dashboards e Visualização (ThingsBoard)**

### **5.1 Objetivos dos Dashboards**

Os dashboards a serem desenvolvidos no ThingsBoard devem:

- apresentar dados meteorológicos em tempo quase real;
- permitir análise histórica;
- diferenciar múltiplas estações, agregando seus dados;
- oferecer visualização clara para usuários não técnicos.

### **5.2 Métricas Previstas**

- Temperatura;
- Umidade relativa;
- Pressão atmosférica;
- Nível de bateria;
- Localização (quando aplicável);
- Status da estação (online/offline).

---

## **6. Consolidação das Decisões Técnicas da Etapa 3**

Ao final da Etapa 3, as seguintes decisões estão consolidadas para a versão beta:

- uso de **LoRaWAN** como tecnologia principal de comunicação;
- utilização do **gateway da FEEC** e do **TTN**;
- visualização via **ThingsBoard**;
- alimentação solar baseada em **painel Resun 10W** e regulador step-down;
- estratégia de **baixo consumo no RP2040** com deep sleep simulado;
- payload otimizado visando baixo Time on Air (ToA).

---

## **7. Avaliação do Nível de Maturidade Tecnológica (TRL)**

Com base no estado atual do projeto:

- arquitetura definida;
- subsistemas principais validados;
- integração lógica planejada;
- testes parciais de consumo e comunicação;

considera-se que o projeto atinge **TRL 4**, com forte preparação para **TRL 5** assim que a validação completa em campo for realizada.

---

## **8. Pendências e Próximos Passos**

Para a continuidade do projeto na Etapa 4, destacam-se como prioritários:

- finalização da integração física completa;
- testes LoRaWAN fim-a-fim com TTN e ThingsBoard;
- validação do alcance real no campus;
- ajustes finos de consumo;
- implantação piloto de pelo menos uma estação em campo;
- consolidação final dos dashboards.

---

## **9. Conclusão**

A Semana 4 da Etapa 3 possibilitou o aprofundamento para o domínio das questões técnicas da comunicação LoRaWAN, preparando o projeto para a futura fase de validação real em campo.

Embora nem todos os testes práticos tenham sido concluídos, o conjunto de decisões técnicas, estudos energéticos, definição de arquitetura e planejamento de implantação fornece uma base sólida para a Etapa 4, onde a estação deverá operar como um sistema IoT completo e funcional.

