# Relatório Técnico – Etapa 4 – Semana 1

## Validação Técnica Inicial do Sistema

**Projeto:** Estação Meteorológica IoT  
**Período:** 5 a 11 de janeiro de 2026  
**Autores:** Antonio Crepaldi – Carlos Perez – Ricardo Furlan  

## Checklist de requisitos × estado atual

| Requisitos Funcionais e Não Funcionais | Estado Atual |
|------------------------|-------------|
| - Medição da carga da bateria (%) |Implementado código do INA226 |
| - Medição de Temperatura do Ar (°C)<br>- Medição de Pressão Atmosférica (hPa)<br>- Medição de Umidade Relativa do Ar (%) | Implementado código do BMP280<br>Necessário atualizar para o BME280 |
| - Medição da posição (GPS) | Implentado parcialmente via UART<br>Necessario reduzir o consumo de energia |
| - Coleta periódica de dados (1 min, 2 min, ..., 60 min)<br> | Feito |
| - Transmissão dos dados via LoRaWAN | Implementado, mas não testado com o gateway |
| - Visualização de dados em dashboards The Things Board<br>- Relato de erros de sensores ao servidor | Em desenvolvimento |
| - Medição de Luminosidade (Lux) | Ainda não implementado |
| - Design modular para adição e substituição de sensores| Implementado |
| - Autonomia energética com baixo consumo de energia | Implementado |
| - Recarga por painel solar | Em testes |
| - Encapsulamento resistente a intempéries (chuva, vento, sol, poeira, umidade)| Em desenvolvimento |
| - Escalabilidade para 10 estações | Em desenvolvimento |
| - Segurança básica dos dados (integridade, disponibilidade e confidencialidade) | Payload criptografado |

## Resultados da validação técnica interna
Foram realizados, em blocos específicos do projeto, as validações necessárias mas, não houve integração completa. Os blocos validados foram:
- Aquisição de dados;
- Transmissão de dados via WCM;
- Redução de consumo de energia via:
  - Software, estratégias de "sleep mode" e redução de clock;
  - Revisão de hardware com a remoção de componentes desnecessários.

## Lista priorizada de ajustes necessários
- Implementação da arquitetura energética com painel solar e recarga direta da bateria (não via USB);
- Confirmação de dados transmitidos via LoRaWAN;
- Implementação dos Dashboards;
- Instalar 10 unidades em campo;
- Medir tempo de funcionamento com apenas a bateria;
- Medir tempo necessário para a carga completa da bateria pelo painel solar;
- Verificar perda de dados na transmissão.

## Plano de validação com cliente ou avaliador externo
Checklist a ser apresentado ao cliente para validação:
- [  ] Os parâmetros de configuração atendem as expectativas (Tela Menu de configuração)?
- [  ] Os sensores utilizados são suficientes para a atual aplicação (Bateria, bme280 e GPS)?
- [  ] A autonomia energética projetado, está dentro do esperado?

Na sequencia do projeto, o cliente deverá avaliar outros aspectos que ainda estão em desenvolvimento:
- Dashboard;
- Manual da instalação e configuração da estação;
- Manual da configuração do GateWay, TTN e TheThingsBoard.
  
## TRL revisado (pré-validação externa) 

Com base no estado atual do projeto, ou seja:
- arquitetura definida;
- subsistemas principais validados;
- integração lógica planejada;
- testes parciais de consumo e comunicação.

Considera-se que o projeto atingiu, até o momento, o **TRL 4**, com tendência para o **TRL 5**, assim que a validação completa em campo for realizada.

