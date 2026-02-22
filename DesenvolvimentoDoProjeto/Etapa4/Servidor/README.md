## Objetivo
- Criar e levantar o servidor para armazenamento e apresentação dos dados
- Duas versões de servidor
    - Servidor sem definição de datasource e dashboards
    - Servidor com definição de datasource e dashboards já configurados.
    
## Uso do servidor COM Dashboard pré configurado
- Garanta que sua máquina está configurada corretamente para rodar o docker
- Garanta que a porta 8080 de sua máquina esteja aberta
- Copie para um diretório local o diretório ServidorComDashboard (e seu conteúdo)
- Entre no diretório ServidorComDashboard e execute:
    ```bash
    docker-compose build
    docker-compose up
    ```
  - $ docker-compose build
  - $ docker-compose up
- Abra o browser na maquina da instalação e acesse:
  - localhost:3000
    - login: admin
    - senha: admin
- No menu laterial selecione Dashboards e click em um dos dois dashboard para a visualização:
  - Dash3Items
  - Dash4Items

## Uso do servidor SEM Dashboard pré configurado
- Garanta que sua máquina está configurada corretamente para rodar o docker
- Garanta que a porta 8080 de sua máquina esteja aberta
- Copie para um diretório local o diretório ServidorComDashboard (e seu conteudo)
- Entre no diretório ServidorComDashboard e execute:
  - $ docker-compose build
  - $ docker-compose up
- Abra o browser na maquina da instalação e acesse:
  - localhost:3000
    - login: admin
    - senha: admin
- no menu lateral selecione SourceDate
  - Crie uma conexão:
	  + postgres
	  + nome: grafana-postgresql-datasource
  - Parâmetros:
	  + DBname: iotdb
	  + user: iot
	  + psw: iospass
	  + TLS/SSL: disabled
- no menu lateral selecione Dashboards e importe através do botão [NEW] o dashboard desejado:
  - Dash3Items
  - Dash4Items

