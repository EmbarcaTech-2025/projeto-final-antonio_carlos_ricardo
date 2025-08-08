# EMBARCATECH - FASE 2

## Projeto Final - Etapa 1 (16/07/2025)
Revisão 1.1: 08/08/2025

---

# Estação Meteorológica para o Agronegócio e a Agricultura Familiar

## Autores:                     Carlos Martinez Perez e Ricardo Jorge Furlan

---

### 1. INTRODUÇÃO

O agronegócio e a agricultura familiar, pilares fundamentais da economia global e brasileira, têm vivenciado uma transformação sem precedentes impulsionada pela adoção de novas tecnologias. A **"Agricultura 4.0"** ou **"Agricultura de Precisão"** representa a convergência de diversas inovações, como a Internet das Coisas (IoT), inteligência artificial, big data e automação, para otimizar processos, aumentar a produtividade e promover a sustentabilidade no campo. Nesse cenário, a capacidade de coletar e analisar dados em tempo real sobre as condições climáticas e do solo torna-se um diferencial competitivo para todos os produtores rurais.

#### 1.1 Objetivo do Projeto:

Este projeto visa o desenvolvimento de uma estação meteorológica inteligente, autônoma e modular, projetada especificamente para atender às demandas tanto do **agronegócio** quanto da **agricultura familiar**. O principal objetivo é fornecer aos produtores rurais dados precisos e localizados de temperatura, umidade do ar e do solo, pressão atmosférica, luminosidade (visível e UV), velocidade e direção do vento, em tempo real.  

#### 1.2 Justificativa:

A tomada de decisões no planejamento e na gestão das lavouras é diretamente influenciada pelas condições climáticas e do solo. A falta de informações detalhadas e específicas para cada microclima da propriedade, muitas vezes, leva a práticas ineficientes e/ou ineficazes. Ao disponibilizar dados climáticos e de solo com alta granularidade e de forma contínua, este projeto busca promover o conhecimento e a educação no campo, permitindo ao produtor rural:

* **Compreensão e Análise da Irrigação:** Decisões de irrigação baseadas na real necessidade hídrica das plantas, usando dados concretos para evitar o desperdício de água e o estresse hídrico, com foco em uma gestão mais consciente.
* **Aprendizagem sobre o Manejo de Culturas:** Melhor planejamento de atividades agrícolas como plantio e colheita, com base no conhecimento das condições que favorecem o desenvolvimento das culturas e na identificação de padrões favoráveis ou desfavoráveis.
* **Racionalização de Insumos e Consciência Ambiental:** Incentivar o uso mais eficiente da água, fertilizantes e outros insumos a partir do conhecimento de seu impacto nas condições locais, minimizando a pegada ambiental e os custos.
* **Capacitação e Resiliência:** Capacitar o produtor com informações para tomar decisões mais ágeis e fundamentadas, o que aumenta a resiliência da propriedade frente a eventos climáticos e promove uma gestão mais autônoma.

### 2. IDENTIFICAÇÃO DO PROBLEMA

Produtores rurais, tanto no agronegócio quanto na agricultura familiar, enfrentam desafios na tomada de decisões precisas sobre o manejo de suas culturas devido à carência de dados microclimáticos e de solo localizados e em tempo real. Essa lacuna de informação resulta em um deficit de conhecimento que pode levar a:

* **Manejo Hídrico Subjetivo:** Uso inadequado da água, seja por excesso (desperdício de recursos e lixiviação de nutrientes) ou por escassez (estresse hídrico e perdas de produtividade), pois não há dados para orientar as práticas.
* **Vulnerabilidade e Perdas:** Dificuldade em compreender as condições climáticas adversas específicas da propriedade (geadas, veranicos, chuvas intensas), bem como o ambiente ideal para o surgimento de pragas e doenças, aumentando os riscos para a produção.
* **Uso de Insumos sem Base de Conhecimento:** Aplicação de fertilizantes e defensivos de forma subótima, sem o amparo de dados reais do solo e do clima, o que leva a custos desnecessários e impactos ambientais evitáveis.
* **Planejamento Agrícola sem Fundamento:** Tomada de decisão sobre plantio e colheita baseada em estimativas ou dados regionais, que nem sempre refletem as condições pontuais da propriedade, limitando o potencial de conhecimento e aprendizado sobre a própria lavoura.

### 3.  REQUISITOS

#### 3.1 Requisitos Funcionais:

O sistema deverá ser capaz de executar as seguintes Medições de
Parâmetros Ambientais:
- Temperatura do Ar: Medição em graus Celsius (°C).
- Temperatura do solo: Medição em graus Celsius (°C).
- Umidade Relativa do Ar: Medição em porcentagem (%).
- Pressão Atmosférica: Medição em hecto-Pascal (hPa).
- Luminosidade: Medição em Lux.
- Intensidade UV: Medição em UV Index.
- Velocidade do Vento: Medição em metros por segundo (m/s) ou quilômetros por hora (km/h).
- Direção do Vento: Medição em graus magnéticos.
- Volume de Chuva (Precipitação): Medição em milímetros (mm).
- Umidade do Solo: Medição em porcentagem (%).
- Qualidade do Ar: Medida de parâmetros como: CO2, CO, O3, NH3, NO2, VOCs.
- Detecção de Descargas Elétricas: número de descargas ocorridas.

Coleta e Armazenamento de Dados:
- Realizar a coleta de dados de todos os sensores em intervalos definidos (ex: a cada 10 segundos para dados brutos, com resumos a cada 10 minutos, 12 horas e 24 horas para armazenamento histórico).
- Armazenar temporariamente os dados no Raspberry Pi Pico em caso de perda de conexão, utilizando um módulo de cartão de memória removível.  

Transmissão de Dados:
- Transmitir os dados coletados para um servidor remoto. A comunicação primária será via Wi-Fi e como opção LoRA, que farão a transmissão até um roteador (WiFi ou LoRa) para envio via internet até um servidor.

Visualização e Análise de Dados:
- Os dados serão acessíveis e visualizáveis através de uma plataforma como o Grafana, com dashboards personalizáveis e específicos para o agronegócio.
- Disponibilizar os dados via API REST e Web Socket para integração com outras plataformas.

Gerenciamento de Erros:
- O sistema deve reportar, ao servidor, erros dos sensores quando forem detectados.

#### 3.2 Requisitos Não Funcionais:
- Modularidade: O design do sistema será modular, permitindo a fácil adição ou substituição de sensores e componentes para satisfazer as necessidades especificas de cada usuário.
- Escalabilidade: O sistema deve ser capaz de suportar a implantação de múltiplas estações em diferentes locais, de forma que os dados possam ser facilmente agrupados.
- Autonomia Energética: Operação autônoma com baixo consumo de energia.
  Ainda, como opções, se vislumbram:
    - Módulo de bateria recarregável com substituição periódica.
    - Módulo de bateria recarregável com recarga por energia solar.
    - Módulo de bateria recarregável com recarga por célula de combustível movida a etanol.
- Robustez Física e Resistência à Intempéries: O encapsulamento eletromecânico será resistente a condições climáticas adversas (chuva, vento, sol, poeira, umidade).
- Manutenibilidade: Facilidade de manutenção e atualização de software e hardware em campo.
- Segurança (Básica): Proteção básica dos dados transmitidos para o servidor, principalmente relativos à: integridade, disponibilidade e confidencialidade.

### 4. LISTA INICIAL DE MATERIAIS
A lista seguinte contempla os principais componentes para uma implementação completa da estação. Os componentes para o MVP (Minimum Valuable Product - Produto Mínimo Viável), aparecem indicados.  

Sensores:
| Quantidade | MVP | Componente | Utilidade |
| :-: | :-: | :--- | :--- |
| 1 |   | BME680                                                   | Pressão |
|   |   |                                                          | Temperatura |
|   |   |                                                          | Umidade |
|   |   |                                                          | Qualidade do Ar |
| 1 |   | BME280                                                   | Pressão |
|   |   |                                                          | Temperatura |
|   |   |                                                          | Umidade |
| 1 | * | BMP280                                                   | Pressão |
|   |   |                                                          | Temperatura |
| 1 | * | ATH10                                                    | Umidade |
|   |   |                                                          | Temperatura |
| 1 |   | Anemômetro com magneto + HMC5883 (ou sensor hall switch) | Velocidade do Vento |
| 1 |   | Biruta com magneto + HMC5883                             | Direção do Vento |
| 1 |   | BH1750                                                   | Luminosidade |
| 1 |   | GY33                                                     | Luminosidade "espectral" |
| 1 |   | GUVA-S12SD                                               | Índice UV |
| 3 |   | LM35                                                     | Temperatura do solo |
| 3 |   | SEN0193                                                  | Umidade do solo |
| 1 |   | Pluviômetro de báscula                                   | Pluviômetro |
| 1 |   | MQ-131                                                   | Qualidade do Ar - ozônio |
| 1 |   | MICS-6814                                                | Qualidade do Ar - CO, NH3, NO2 |
| 1 |   | NHZ-14A                                                  | Qualidade do Ar - CO2 |

Obs.: Nem todos os sensores serão implementados num primeiro momento do desenvolvimento, sendo apenas uma previsão para a estruturação do projeto.

Diversos:
| Quantidade | MVP | Componente | Utilidade |
| :-: | :-: | :--- | :--- |
| 1 | * | BitDogLab V6.3                   | CPU |
| 1 |   | RTC DS3231 ou DS1302 ou DS1307   | RTC (Real Time Clock) |
| 1 | * | Cartão de memória com módulo IDC | Armazenamento temporário |
| 1 | * | Cartão de expansão I2C com cabos | Módulo de expansão I2C |
| 1 |   | Cartão de expansão IDC           | Módulo de expansão IDC |
| 1 |   | Módulo Lora IDC                  | Comunicação |
| 1 |   | Módulo AD IDC                    | Conversor AD  |
| 1 | * | Fonte 5V                         | Alimentação |

A figura abaixo apresenta o diagrama estrutural da estação meteorológica, ilustrando a abordagem modular que fundamenta o projeto.
Cada bloco representa um subsistema autônomo, mas interconectado, garantindo flexibilidade, facilidade de manutenção e escalabilidade.
Essa arquitetura permite que os dados de diferentes sensores (temperatura, umidade, pressão, vento, chuva, luminosidade e umidade do solo) sejam coletados e processados de forma eficiente pelo Raspberry Pi Pico W, antes de serem transmitidos para um servidor remoto. A separação dos módulos otimiza o desenvolvimento e facilita futuras adaptações, reforçando o objetivo de criar um sistema robusto e versátil para o agronegócio.

![Estrutura Simplificada da Estação Meteorológica](../Etapa2/assets/Diagrama_Estrutural_Simplificado.png)
