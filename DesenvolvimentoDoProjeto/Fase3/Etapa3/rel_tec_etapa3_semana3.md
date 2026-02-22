# Relatório Técnico – Etapa 3 - Semana 3  
## Otimização, Correções e Robustez  
**Projeto:** Estação Meteorológica IoT  
**Período:** 09 a 15 de dezembro de 2025  
**Autores:** Antonio Crepaldi – Carlos Perez – Ricardo Furlan  

---

## 1. Introdução

A Semana 3 da Etapa 3 teve como foco a **otimização do sistema, correção de fragilidades identificadas nas semanas anteriores e aumento da robustez geral da solução**.  

Entretanto, não foi possível consolidar um protótipo totalmente integrado nesta semana, em função de limitações de hardware e software.  Para atender aos requisitos futueos do projeto, optou-se por voltar a atenção para duas frentes:  
* avaliação energética com a nova placa solar adquirida e  
* desenvolvimento e validação de estratégias de redução de consumo no RP2040.

Essas atividades, embora menos visíveis do ponto de vista de integração fim-a-fim, são fundamentais para a confiabilidade e autonomia do sistema em campo.

---

## 2. Revisão Técnica da Semana

### 2.1 Situação do Protótipo

- Ainda **não há protótipo completamente funcional end-to-end.
- A comunicação LoRaWAN (via WCM) e backend permanecem pendentes.

Essa decisão foi intencional, visando evitar retrabalho futuro e permitir uma integração mais estável nas próximas semanas.

---

## 3. Estudos e Testes Energéticos com Placa Solar

### 3.1 Revisão da Arquitetura de Alimentação

Durante esta semana foi aprofundado o estudo iniciado anteriormente sobre a alimentação solar da estação, agora com uma nova placa sola e novo módulo de carregamento:

- Painel solar **Resun 10W**;
- Regulador **step-down síncrono**, com ripple reduzido;
- Proteção simples com diodo no positivo;
- Alimentação da BitDogLab via **VBUS**.

### 3.2 Resultados Observados

Os testes indicaram:

- geração estimada entre **6,5 Wh/dia e 21 Wh/dia**, dependendo das condições climáticas;
- média operacional compatível com ciclos de leitura e transmissão LoRaWAN;
- redução significativa de ruído elétrico em comparação com soluções anteriores.

Essa melhoria impacta diretamente:

- a estabilidade dos sensores,
- a confiabilidade da comunicação,
- a redução de resets e falhas intermitentes.

---

## 4. Desenvolvimento de Baixo Consumo no RP2040

### 4.1 Motivação

Diante do orçamento energético limitado, tornou-se prioritário reduzir o consumo médio da estação.  
Como o RP2040 não possui um modo de deep sleep nativo equivalente ao de outros microcontroladores, optou-se por estudar e implementar uma **estratégia de deep sleep simulado**, baseada em **light sleep, e redução de clock.

### 4.2 Atividades Realizadas

- Estudo detalhado do funcionamento interno do RP2040;
- Implementação de rotinas de:
  - redução de clock,  
  - uso controlado de timers;  

- Testes repetitivos de entrada e saída de estados de baixo consumo;
- Medições com osciloscópio, analisando:
  - consumo em repouso,
  - picos de corrente na transição wake-up,
  - estabilidade das linhas de alimentação.

### 4.3 Resultados Parciais

- Redução do consumo médio durante períodos ociosos;
- Comportamento previsível e repetível do sistema ao acordar;
- Base de integração futura com ciclos de transmissão LoRaWAN.

---

## 5. Testes

Os testes realizados nesta semana foram focados em veridficar o comportamento do RP2040 em ciclos repetidos de sleep/wake. Não foram realizados ainda testes completos de comunicação LoRaWAN nem validação fim-a-fim.

---

## 6. Avaliação de Robustez

Apesar da ausência de um protótipo totalmente integrado, a robustez do sistema foi fortalecida em aspectos fundamentais:

- alimentação mais estável;
- menor consumo médio;
- maior previsibilidade do comportamento do firmware;
- redução de riscos para operação em campo.

Esses ganhos são considerados pré-requisitos para a fase final do projeto.

---

## 7. Revisão do TRL

Considerando o estado atual:

- não houve avanço significativo em integração funcional;
- houve avanço relevante em **infraestrutura energética e gestão de consumo**.
- **TRL permanece em 4**, com melhorias internas que preparam o salto para TRL 5 assim que a integração completa for realizada.

---

## 8. Conclusão

A Semana 3 da Etapa 3 não resultou em um protótipo funcional completo, mas foi tecnicamente produtiva.  
O tempo investido em energia e baixo consumo proporcionará melhor autonomia.   

A próxima etapa deverá concentrar esforços em:

- integração completa dos módulos,
- validação LoRaWAN fim-a-fim,
- consolidação da versão beta funcional.
