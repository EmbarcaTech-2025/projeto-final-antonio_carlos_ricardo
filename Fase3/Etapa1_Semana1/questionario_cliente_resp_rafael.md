# Questionário de Levantamento de Requisitos  
### Projeto: Estação Meteorológica IoT  
**Fase 3 – Entrevista com o Usuário/Cliente**

---

## 1. Identificação do Entrevistado
- **Nome:**  Rafitcha
- **Cargo/Função:**  Monitor do Projeto/Cliente
- **Área de atuação:**  Monitoria Embarcatech
- **Data da entrevista:** 09/10/25
- **Entrevistador:** Entrevistador  

---

## 2. Uso Atual e Expectativas

1. Você já utiliza algum tipo de estação meteorológica ou sensor ambiental?  
   - (X) Não  
   - ( ) Sim
     - quais modelos tipos: ______________________________________________
     - quais limitações você encontrou no sistema que utilizou:___________

1. Quais são as **principais informações meteorológicas** que você considera mais importantes monitorar?  
   - (X) Temperatura  
   - (X) Umidade do ar  
   - (X) Pressão atmosférica  
   - ( ) Velocidade do vento  
   - ( ) Direção do vento  
   - (X) Pluviosidade  
   - (X) Luminosidade  
   - ( ) Outras: ___________________________________________________

1. Com que **frequência** você gostaria que fosse feita a **coleta dos dados**?  
   - A cada *5 minutos* (Segundos, minutos , horas)
   - ( ) Outro intervalo: ____________________________
   
1. Com que **frequência** você gostaria que as medições fossem **atualizadas**?  
   - ( ) Em tempo real  
   - ( ) A cada minuto  
   - (X) A cada hora  
   - ( ) Outro intervalo: ____________________________

1. Qual seria a **forma ideal de visualizar os dados**?  
   - ( ) Display local na estação  
   - ( ) Display remoto dedicado a estação
   - (X) Aplicativo Celular
   - (X) Site Web 
   - (X) Relatórios automáticos enviados via:
     - (X) Whatsapp
     - (X) Telegrama
     - ( ) e-mail
     - ( ) Outro: _____________________________
   - ( ) Outro: ____________________________

---

## 3. Funcionalidades Desejadas

1. Deseja que a estação **envie alertas automáticos** (ex.: chuva forte, temperatura extrema, falha no sensor)? 
   - ( ) Não  
   - (X) Sim, como gostaria de recebê-los? (ex.: e-mail, app, WhatsApp): *Notificação do app/whatsapp*

1. Acha importante **armazenar o histórico dos dados** para consultas futuras e gráficos de tendência?  
   - ( ) Não 
   - (X) Sim, por quanto tempo: *1 Mês*

1. Você considera útil a possibilidade de **exportar os dados** (CSV, Excel, etc.)? 
   - ( ) Não  
   - (X) Sim, qual formato: CSV/Excel

1. Há interesse em fazer análise comparativa entre estações ou que fosse feito a média entre **várias estações** (por exemplo, em diferentes áreas da propriedade)?
   - ( ) Não  
   - (X) Sim, descrever: Média e desvio padrão de cada tipo de medição

---

## 4. Requisitos Técnicos e Operacionais

1. Em relação à **conectividade**, quais opções são mais viáveis no seu ambiente?  
   - ( ) Cabeada(Ethernet, USB, RS232 etc)
   - ( ) Bluetooth
   - (X) Wi-Fi  
   - ( ) 4G/5G  
   - (X) LoRa  
   - ( ) Zibgee
   - ( ) Outro: ____________________________

1. Qual (ou quais) deve ser a **fonte energética primária** da estação?
   - ( ) Alimentação elétrica contínua  
   - (X) Bateria  
   - (X) Energia solar  
   - ( ) Outro: ____________________________

1. Deve haver fonte auxiliar, no-brake/bateria?
   - ( ) Não
   - ( ) Sim, qual o tempo de duração mínimo: __________________________
 
1. Há restrições quanto à **instalação física** (local, tamanho, tipo de fixação)?
   - ( ) Não
   - (X) Sim, qual/quais: *Ser resistente às intempéries (Chuva, neve, ventos fortes).*

1. Em caso de falha de conexão como o servidor, é importante que os dados sejam **armazenados localmente** e enviados depois?  
   - ( ) Não
   - (X) Sim, qual o intervalo mínimo de armazenamento desses dados: *3 Dias*

---

## 5. Interface e Usabilidade

1. Quais informações você gostaria que aparecessem **no display local** (caso exista)?  
   - ( ) Temperatura e umidade  
   - ( ) Data e hora  
   - (X) Status da rede  
   - (X) Alertas e avisos  
   - ( ) Outro: ____________________________

1. Você prefere uma interface de configuração:  
   - ( ) Por menu no display  
   - (X) Via navegador web  
   - (X) Via aplicativo móvel
   - ( ) Outro: ____________________________  

1. A interface do protótipo atual (menu e configurações) é clara e intuitiva?
   - (X) Sim
   - ( ) Parcialmente
   - ( ) Não  
   - Sugestões de melhoria: ____________________________

---

## 6. Casos de Uso

1. Em quais **situações práticas** você imagina usar a estação meteorológica? 
    :*Monitorar as condições climáticas de um agronegócio, ou então de construções vazias em locais remotos*

1. Descreva um **exemplo real** de como as informações coletadas ajudariam no seu trabalho ou decisão.  
    :*No caso do agronegócio, monitorar as tendências do clima a curto (ao longo do dia) e longo prazo (ao longo do mês), analisando também como as alterações climáticas afetaram o crescimento e desenvolvimento da plantação, eu poderia tomar decisões estratégicas relacionadas a tempo e local de colheita e plantio das plantas e também tentar prever qualidade das colheitas futuras baseado nos dados colhidos*

1. Há algum cenário onde o sistema **deveria ter um funcionamento diferenciado ou comportamento especial** (ex.: modo de economia, tempestades, ausência de rede)?  
    : *Na ausência de rede, armazenar os dados localmente e enviar em um momento futuro quando houver conexão novamente*
    *Em casos de condições extremas, notificar o usuário com urgência das condições (tempestades, ventos fortes, etc)*
    *Seria interessante poder monitorar o nível de bateria de cada módulo caso funcionem com baterias*

---

## 7. Considerações Finais

1. O que você considera **mais importante** para o sucesso desse projeto?  
    :*Consistência do funcionamento do módulo, minimizando ao máximo a necessidade de manutenção e qualidade do recebimento das informações, incluindo a facilidade com que o cliente pode acessar esses dados, e também as funcionalidades para análise estatística nativas do projeto, podendo incluir análises nativas e também formas facilitadas de exportação dos dados*

1. Deseja sugerir **novos sensores ou recursos** para futuras versões?  
    :___________________________________________

