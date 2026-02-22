# Objetivo
O objetivo desses aplicativos em Java é simular o comportamento de uma estação meteorológica, permitindo desenvolver e aprimorar o servidor de forma independente do dispositivo IoT.

# Programas
- **IoT_dummy_get_time.java**
  - Faz uma requisição da data/hora atual ao servidor.
  - Para executar:
    ```bash
    java IoT_dummy_get_time.java
    ```

- **IoT_dummy_send_data.java**
  - Envia, a cada 5 segundos, dados fictícios de uma estação para o servidor.
  - Para executar:
    ```bash
    java IoT_dummy_send_data.java 127.0.0.1 8080 nome_da_estacao
    ```
    ou
    ```bash
    java IoT_dummy_send_data.java 127.0.0.1 8080 numero_da_estacao (0 a 12)
    ```

- **IoT_dummy_send_old_data.java**
  - Envia para o servidor dados fictícios de diversas estações, referentes a um período de 8 dias atrás até o momento.  
    Os dados simulam aquisições feitas a cada 60 segundos, com o objetivo de popular o sistema para testes.
  - Para executar:
    ```bash
    java IoT_dummy_send_old_data.java 127.0.0.1 8080
    ```

# Observação
Esses códigos não estão documentados em profundidade, pois não representam o objetivo principal do projeto, mas apenas ferramentas auxiliares para simulação e testes.
