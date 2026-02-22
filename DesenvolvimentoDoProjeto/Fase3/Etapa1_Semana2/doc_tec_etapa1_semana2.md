# Documento Técnico Resumido
## Estação Meteorológica IoT – Projeto Embarcatech

### 1. Introdução
A Estação Meteorológica IoT tem como objetivo realizar medições ambientais em tempo real e disponibilizar os dados por meio de uma infraestrutura conectada à internet. O sistema integra sensores meteorológicos, módulos de comunicação, fontes de energia autônomas e um servidor de visualização com dashboards interativos no Grafana ou thingsboard.

O projeto segue uma arquitetura modular, escalável e energeticamente autônoma, desenvolvida para suportar múltiplas estações distribuídas em campo, atendendo tanto a demandas de pesquisa e ensino quanto a aplicações produtivas, como agricultura, pecuária e monitoramento ambiental.

---

### 2. Requisitos Consolidados

#### 2.1 Requisitos Funcionais
O sistema deve:
- Medir prioritariamente: **temperatura do ar**, **umidade do ar**, **pressão atmosférica**, **luminosidade** e opcionalmente: **índice UV**, **velocidade e direção do vento**, **volume de chuva**.
- Realizar **coleta periódica** de dados a cada 5 minutos.
- **Armazenar temporariamente** os dados localmente (memória flash ou cartão SD) para que em caso de falha de conexão os dados possam ser transmitidos após o restabelecimento da rede.
- **Transmitir os dados** para um servidor remoto via **Wi-Fi** (HTTP/HTTPS) ou **LoRaWAN** com gateway.
- Disponibilizar os dados através de **dashboards no Grafana ou thingsboard**.
- Reportar **falhas de sensores** e anomalias operacionais ao servidor.
- **Autonomia energética:** operação contínua com baterias recarregáveis e painéis solares.

#### 2.2 Requisitos Não Funcionais
- **Modularidade:** cada bloco do sistema (sensores, energia, comunicação, CPU) deve ser independente e substituível.
- **Escalabilidade:** suporte a múltiplas estações simultâneas em locais distintos.
- **Robustez física:** invólucro resistente a intempéries (chuva, vento, sol e poeira).
- **Manutenibilidade:** fácil atualização de firmware e hardware em campo.
- **Segurança básica:** integridade e confidencialidade dos dados transmitidos.

---

### 3. Diagrama de Blocos e Camadas
O diagrama **DiagramaDaEstacaoEServidor.png** representa a **arquitetura hierárquica da estação meteorológica IoT**.
![DiagramaDaEstacaoEServidor.png](../Etapa1_Semana1/DiagramaDaEstacaoEServidor.png)

#### 3.1 Camada de Sensores
Compreende os módulos responsáveis pela aquisição de dados ambientais. Os sensores estão divididos em quatro grupos:
- **Digitais(pulos):** anemômetro e pluviômetro.
- **I²C:** sensores de temperatura do ar, umidade, pressão atmosférica, direção do vento e RTC.
- **Analógicos:** índice UV.
- **Serial (UART):** GPS

Esses sensores se comunicam com a CPU principal via barramentos digitais (I²C, GPIO, UART) e entradas AD.

#### 3.2 Camada de Infraestrutura
Inclui os subsistemas de energia e monitoramento:
- **Fonte primária de energia:** alimentação principal (painel solar ou fonte externa).
- **Fonte secundária de energia(no-break):** Bateria, o que garante autonomia durante falta da fonte primária energia.
- **Módulo de integridade:** monitora tensões, corrente e status da bateria.

#### 3.3 Camada de Processamento
A **CPU** (ex.: Raspberry Pi Pico W) realiza:
- Leitura e pré-processamento dos sensores;
- Armazenamento temporário em memória;
- Opcionalmente comunicação com a interface do usuário (display local, comunicação serial, wifi).

#### 3.4 Camada de Transmissão
Responsável pela comunicação com o servidor remoto. O sistema oferece três opções:
1. **Wi-Fi (HTTP)** – comunicação simples com servidor web;
2. **Wi-Fi (HTTPS/TLS)** – comunicação segura e criptografada;
3. **LoRaWAN + Gateway** – comunicação de longo alcance para áreas remotas.

#### 3.5 Camada de Servidor e Visualização
Após o envio dos dados, o servidor executa as seguintes funções:
- **Recepção e armazenamento** dos dados em um banco **PostgreSQL**;
- **Serviço de integração, visualização e análise** com o **Grafana** ou **ThingsBoard** para interface com os usuários, permitindo consultas e gráficos e históricos.

---

### 4. Interfaces e Dependências Principais

| Interface | Tipo | Função | Dependência |
|:---------:|:----:|--------|--------------|
| **I²C** | Digital serial | Comunicação com sensores de temperatura, pressão, umidade, luminosidade e RTC | CPU ↔ Sensores |
| **GPIO / Interrupções** | Digital | Leitura de pulsos do anemômetro e pluviômetro | CPU ↔ Sensores |
| **Wi-Fi / HTTPS** | Comunicação | Envio seguro de dados ao servidor | CPU ↔ Servidor |
| **LoRaWAN** | Comunicação de longo alcance | Alternativa para áreas sem Wi-Fi | CPU ↔ Gateway ↔ Servidor |
| **Energia Solar / Bateria** | Alimentação | Fonte primária e reserva de energia | Infraestrutura ↔ CPU |
| **Grafana/ThingsBoard<br>PostgreSQL** | Software | Visualização e armazenamento de dados | Servidor ↔ Usuário |

---

### 5. Conclusão
A arquitetura proposta integra sensores, energia, comunicação e software em uma estrutura modular. O projeto consolida a transição para um sistema de monitoramento ambiental funcional, com armazenamento local, comunicação segura e visualização em tempo real, apto para evoluir de TRL 4 para TRL 7 até o final da Fase 3 do Programa Embarcatech.
