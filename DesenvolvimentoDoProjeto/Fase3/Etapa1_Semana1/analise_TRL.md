# Nível de Maturidade Tecnológica (TRL) do Projeto

### Estação Meteorológica IoT -- Justificativa e Meta de Evolução

O projeto **Estação Meteorológica IoT** encontra-se atualmente no
**Nível de Maturidade Tecnológica (TRL 4)**, de acordo com a escala
internacionalmente adotada para mensurar o grau de prontidão de uma
tecnologia. Esse nível corresponde à **validação de componentes e
funcionalidades em ambiente controlado de laboratório**, com integração
inicial entre hardware e software, mas ainda sem operação contínua em
ambiente real.

Nesta etapa, já foram implementadas e testadas as principais
funcionalidades da estação meteorológica: aquisição de dados de sensores
ambientais (temperatura, umidade e pressão), armazenamento local,
comunicação via Wi-Fi, integração com banco de dados PostgreSQL e
visualização dos dados em um dashboard Grafana. Esses resultados
comprovam a **viabilidade técnica** do conceito e o funcionamento
coerente dos módulos, caracterizando a transição entre o protótipo
experimental e um sistema funcional básico.

Entretanto, o projeto **ainda não atingiu o estágio de protótipo
validado em campo**, pois faltam etapas essenciais como a consolidação
do encapsulamento físico da estação, testes de robustez sob condições
ambientais reais (chuva, vento, radiação solar) e a avaliação de
desempenho contínuo da conectividade em diferentes cenários. Além disso,
aspectos como autonomia energética por meio de bateria e painel solar, conectividade com melhores alcance e rendimento energético, alternativas de armazenamento local com envio diferido de dados, aspectos de segurança da informação e auto recuperação de falhas ainda estão sendo avaliados para desenvolvimento atual ou futuro.

------------------------------------------------------------------------

## 🎯 Proposta de Evolução até o TRL 7

A meta definida pelo grupo para o término da **Fase 3 do Programa Embarcatech** é
alcançar o **TRL 7**, que corresponde ao **protótipo de sistema
demonstrado em ambiente operacional real**. Para atingir esse patamar, o
plano de evolução tecnológica contempla as seguintes ações:

1.  **Integração completa do sistema IoT**: unificação dos módulos de aquisição, processamento, comunicação e visualização em um sistema contínuo e autônomo.  
2.  **Implantação em ambiente real**: instalação de várias estações meteorológicas em campo, com coleta e  transmissão de dados contínuas por períodos prolongados.  
3.  **Validação funcional e de desempenho**: avaliação da estabilidade
dos sensores, confiabilidade das comunicações Wi-Fi/LoRa/Bluetooth e
consistência dos dados armazenados e exibidos no Grafana.  
4.  **Autonomia energética**: implementação do sistema de energia solar e bateria, validando o funcionamento autossuficiente da estação.  
5.  **Testes de resiliência e tolerância a falhas**: verificação do comportamento do sistema frente a interrupções de rede e energia, com armazenamento local e posterior sincronização de dados.  
6.  **Aprimoramento da interface e usabilidade**: ajustes na experiência do usuário, tanto no dashboard quanto na configuração via navegador e aplicativo móvel.

------------------------------------------------------------------------

Ao final da Fase 3, espera-se que o sistema esteja **plenamente
operacional em campo**, com capacidade de coleta, transmissão e análise
de dados em tempo real, mantendo estabilidade e confiabilidade em
diferentes condições ambientais. Essa consolidação representará a
**transição do protótipo experimental (TRL 4)** para um **protótipo
validado em ambiente real (TRL 7)**, pronto para testes ampliados, escalonamento e aplicações educacionais e produtivas no contexto do Embarcatech.
