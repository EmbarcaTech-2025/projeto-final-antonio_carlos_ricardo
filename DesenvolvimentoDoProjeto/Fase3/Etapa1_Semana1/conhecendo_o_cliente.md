# Conhecendo o Cliente

### Projeto: Estação Meteorológica IoT


------------------------------------------------------------------------

## 1. Perfil do Cliente

O cliente típico atua na área de produção agrícola. Outros interessados em dados climáticos para a tomada de decisões, como:  
* promotores de eventos ao ar livre  
* esportistas  
* pescadores  
* silvicultores e reflorestadores  
* pecuaristas  
* apicultores  
* gestores de recursos hídricos  
* empresas concessionárias de rodovias e abastecimento  
* defesa civil
* escolas e instituros de pesquisa  
* outras atividades que tenham interesse em acompanhamento climático remoto

------------------------------------------------------------------------

## 2. Necessidades e Expectativas

As informações consideradas mais importantes para monitoramento são
**temperatura, umidade do ar, pressão atmosférica, pluviosidade e
luminosidade**. No caso específico do entrevistado, há o desejo de coletar dados a cada **5
minutos**, com atualização **horária** dos valores exibidos,
equilibrando detalhamento e eficiência energética. A estação deve prover flexibilidade ^na definição desses parâmetros de modo a atender variados caso de uso.

Ainda segundo o entrevistado, a visualização dos dados deveria ocorrer via **aplicativo móvel e site
web**, com suporte adicional a **relatórios automáticos** enviados por
**WhatsApp** e **Telegram**. Essa escolha revela a necessidade de acesso
rápido e móvel, sem depender de interfaces locais. Estabelece-se aí, um ponto de atenção para desenvolvimento futuro.

O entrevistado deseja que o sistema ofereça **alertas automáticos** para
eventos críticos, como chuva forte, temperaturas extremas ou falhas de
sensores, preferencialmente via notificações em tempo real. Além disso,
considera essencial o **armazenamento histórico de dados por 30 dias**,
com possibilidade de **exportação em CSV ou Excel**, o que facilita
análises externas e integração com outras ferramentas. O grupo de desenvolvimento deve aprofundar a discussão em relação a esses pontos.

------------------------------------------------------------------------

## 3. Requisitos Técnicos e Operacionais

A conectividade deve ser feita principalmente por **Wi-Fi** e **LoRa**,
atendendo tanto a ambientes urbanos quanto rurais e remotos. Quanto à
alimentação, o entrevistado prioriza **bateria e energia solar**,
ressaltando a importância da **autonomia e resiliência energética**.

O equipamento deve ser **resistente às intempéries**, suportando chuva,
ventos fortes e variações climáticas. Em caso de falha de conexão com o
servidor, é essencial que os dados sejam **armazenados localmente por
até 3 dias**, com posterior envio automático quando o link for
restabelecido.

------------------------------------------------------------------------

## 4. Interface e Usabilidade

O cliente valoriza interfaces **claras e intuitivas**, preferindo
configurar o sistema **via navegador web ou aplicativo móvel**. No
display local, caso exista, devem aparecer informações sobre **status da
rede** e **alertas**, evitando sobrecarga visual. A navegação do
protótipo atual foi considerada adequada.

Essa preferência reforça a visão de um sistema **acessível, moderno e
centralizado na experiência do usuário**, com baixo esforço de
configuração e manutenção.

Abre-se aqui um ponto de discussão sobre a interface atual, principalmente no que se refere à falta de conhecimento do usuário médio com programas como o monitor serial Putty. Alternativa mais populares como uma página web ou um aplicativo móvel devem der levadas em conta para implementação futura.

------------------------------------------------------------------------

## 5. Casos de Uso e Aplicações

O entrevistado imagina a estação sendo usada para **monitorar condições
climáticas em propriedades agrícolas** e também em **locais remotos ou
de difícil acesso**, como construções desocupadas.

Os dados serviriam para **analisar tendências climáticas de curto e
longo prazo**, correlacionando variáveis ambientais com o **crescimento
das plantações**, ajudando a decidir **momentos ideais de plantio,
colheita e manejo**. Além disso, menciona o interesse em **acompanhar o
nível de bateria dos módulos** e **receber notificações urgentes em caso
de eventos extremos**.

------------------------------------------------------------------------

## 6. Prioridades do Cliente

Para o sucesso do projeto, o cliente considera essencial:\
- **Confiabilidade e consistência** no funcionamento da estação, com
mínima necessidade de manutenção;\
- **Facilidade de acesso aos dados**, tanto local quanto remoto;\
- **Qualidade na transmissão e atualização das informações**;\
- **Ferramentas nativas de análise estatística**, incluindo médias e
desvios padrão entre múltiplas estações;\
- **Possibilidade de exportação dos dados** de forma prática e
compatível com softwares externos.

------------------------------------------------------------------------

### 📍 Síntese

O cliente valoriza **autonomia, precisão e acessibilidade**. Deseja um
sistema robusto, automatizado e interativo, com forte integração entre
hardware e plataforma digital. A combinação de **energia solar,
comunicação LoRa/Wi-Fi e interface via app/web** reflete uma visão
moderna e sustentável de monitoramento meteorológico, alinhada às
tendências da IoT aplicada ao agronegócio e à educação tecnológica.
