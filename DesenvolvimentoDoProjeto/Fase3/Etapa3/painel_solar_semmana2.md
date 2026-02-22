# Estudo Comparativo de Viabilidade Técnica e Económica: Estações Meteorológicas IoT

**Objecto:** Comparativo entre a arquitetura baseada no "Modelo Unicamp" (Painéis DIY + MP158) versus a Solução Profissional (Painel 10W + Step-Down Síncrono).
**Aplicação:** Alimentação de 9 Estações com BitDogLab (Raspberry Pi Pico W + ESP32 LoRa).

---

## 1. Resumo Executivo

A análise demonstra que a modernização dos componentes (painel solar e regulador de tensão) resulta numa **redução de custos superior a 65%**, aumenta a precisão dos sensores devido ao menor ruído elétrico e garante uma vida útil do equipamento superior a 10 anos.

---

## 2. Análise Financeira (Por Estação)

O impacto financeiro considera a substituição dos painéis e a troca do regulador MP158 pelo modelo Mini Síncrono.

| Item | Proposta Inicial (Unicamp + MP158) | Solução Otimizada (Resun 10W + Mini Step-Down) |
| :--- | :--- | :--- |
| **Painéis Solares** | R$ 227,20 (8 un.) | R$ 76,00 (1 un.) |
| **Regulador de Tensão** | ~R$ 15,00 (MP158) | ~R$ 9,00 (Mini Síncrono) |
| **Custo por Estação** | **R$ 242,20** | **R$ 85,00** |
| **Custo Total (9 Estações)** | **~ R$ 2.179,80** | **~ R$ 765,00** |
| **Economia Total Gerada** | — | **R$ 1.414,80** |

---

## 3. Comparativo: Painéis Solares

### A. Modelo Unicamp (Artesanal)
* **Arquitetura:** 8 placas laminadas (PET/Epóxi) ligadas manualmente.
* **Vulnerabilidades:** Degradação rápida do plástico (amarelecimento em < 3 anos), alta complexidade de soldadura (72 pontos de falha) e vedação precária.

### B. Solução Profissional (Recomendada)
* **Arquitetura:** Painel único Resun 10W.
* **Construção:** Vidro Temperado e Alumínio Anodizado (Certificação INMETRO).
* **Vantagens:** Instalação "Plug & Play", garantia de 12 anos e robustez contra granizo e radiação UV.

---

## 4. Comparativo: Reguladores de Tensão

A substituição do regulador MP158 pelo novo modelo Mini Step-Down Síncrono traz vantagens críticas para aplicações de IoT e sensores de precisão.

| Parâmetro Técnico | MP158 (Modelo Antigo) | Mini Step-Down Síncrono (Novo) | Veredicto |
| :--- | :--- | :--- | :--- |
| **Tecnologia** | Retificação Assíncrona (Diodo) | Retificação Síncrona (MOSFET) | **Novo é superior** |
| **Eficiência** | ~88% a 92% | Até **97.5%** | **Novo desperdiça menos energia** |
| **Output Ripple (Ruído)** | 30mV a 50mV | **10mV** | **Novo garante leitura estável dos sensores** |
| **Tamanho** | 42 x 16 mm | **26 x 15 mm** | **Novo facilita a integração na caixa** |
| **Aquecimento** | Moderado (pode exigir dissipador) | Mínimo (frio) | **Novo é mais seguro** |

> **Nota Técnica sobre o Ruído (Ripple):**
> O novo regulador possui um *ripple* de apenas 10mV. Para sensores climáticos (pressão, humidade), isto é crucial, pois evita flutuações nas leituras e previne reinícios inesperados do microcontrolador (Brownout).

---

## 5. Dimensionamento Energético (Balanço de Energia)

Considerando o hardware alvo (**BitDogLab**), o sistema é sobredimensionado para garantir operação contínua.

* **Consumo do Sistema:** ~1.2W (média).
* **Geração Painel 10W:** ~7W a 10W.
* **Margem de Segurança:** O painel gera aproximadamente **5x a 6x mais energia** do que o consumo, permitindo recarga rápida da bateria 18650 mesmo em dias com pouca incidência solar.

---

## 6. Arquitetura Final Sugerida

Para maximizar a eficiência, a segurança e a simplicidade de montagem das 9 estações:

1.  **Fonte:** 1x Painel Solar Resun 10W (18V).
2.  **Proteção:** 1x Diodo 1N4007 (No positivo, contra inversão de polaridade na instalação).
3.  **Conversão:** 1x Módulo Mini Step-Down DC-DC (R$ 9,00).
    * *Configuração:* Entrada 18V / Saída 5V USB.
4.  **Carga/IoT:** Placa BitDogLab.
    * *Conexão:* Via porta USB ou pinos VBUS/GND. A placa gere automaticamente a carga da bateria 18650 e a telemetria LoRaWAN.

---

## 7. Conclusão

A adoção do **Painel Profissional de 10W** combinada com o **Regulador Síncrono de Baixo Ruído** representa a melhor decisão técnica e económica.
O projeto ganha em **fiabilidade de dados** (energia limpa para sensores), **durabilidade física** (vidro/alumínio) e liberta orçamento substancial (R$ 1.400,00) que pode ser realocado para outros periféricos ou reservas de contingência.
