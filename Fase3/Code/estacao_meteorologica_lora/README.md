# Estação Meteorológica LoRa/LoRaWAN

## Build:0004- 04/01/2026
- Menu parcialmente funcional para LoRaWAN ABPVariaveis do lorawan
- Rotinas com sensores de:
    - Bateria
    - Pressão, temperatura (BMP280)
    - GPS
- Sleep mode
- Comunicação com WCM para envio LoRaWAN ABP


## Build:0002- 08/12/2025
- Add simple Sleep mode
- Add simple Bat    Driver
- Add simple Bmp280 Driver
- Add simple GPS    Driver
- Add print do que deve ser enviado pelo loRa
- Add Debug Mode

## Build:0001- 05/12/2025
- inicio do menu
- estrutura do main e todos os arquivos necessários
- Estrutura básica da configuração
- Estrutura básica dos dados
- Definição do payload físico:
[Byte de controle] [dados....] [Byte de controle] [dados....] [Byte de controle] [dados....]

BC[0] ==> Byte de controle 1 define o que está presente e o tamanho de cada item

Bit 0 ==> Battery Level, Add 1 Byte no Payload
    Add 1 Byte: Batery Level 0-100 %, 0xFF ==> erro no sensor

Bit 1 ==> Bme280, Add 5 Bytes no Payload,
    Add 1 Byte:  Umidade:       0 -  100 %,       Resolução  0.5 %
        0xFF ==> erro no sensor
        0x00 ==>   0.0 %
        0x01 ==>   0.5 %
        0x02 ==>   1.0 %
        0xC8 ==> 100.0 %
        Umidade(em %) = Valor * 0.5;

    Add 2 Bytes: Temperatura: -40 -  +85 Celsius, Resolução 0.01 C
        0x7FFF ==> erro no sensor
        0xD8F0
        -10000 ==> -100.00 C
        0xFFFF ==>   -0.01 C
        0x0000 ==>    0.00 C
        0x0001 ==>    0.01 C
        0x2710
         10000 ==> +100 0.00 C
        Temperatura (em Celsius) = Valor * 0.01
        T = V / 100

    Add 2 Bytes: Pressão:     300 - 1100 hPa,     Resolução 0.02 hPa
        0xFFFF ==> erro no sensor
        0x0000 ==> 1200.00 hPa
        0x0001 ==> 1199.98 hPa
        0xAFC8
         45000 ==>  300.00 hPa
        Pressão(em hPa) = (60000 - Valor) * 0.02
    
Bit 2 ==> GPS, Add 10 Bytes
    Add x 4 Bytes Latitude
        0x7FFF FFFF ==> erro no sensor
        latitude em graus = valor * 512 /(2^32)
    Add x 4 Bytes Longitude
        0x7FFF FFFF ==> erro no sensor
        latitude em graus = valor * 512 /(2^32)
    Add x 2 Bytes Altitude
        0x7FFF ==> erro no sensor
        atitude em metros = valor * 0.1


A serem definidos:
Bit 3 ==> Luxímetro (2 bytes)
Bit 4 ==> UV índex (1 Byte)
Bit 5 ==> vento(direção (1byte, 2 graus de resolução) e velocidade (1byte, resolução de 0,2 m/s para vMax  51m/2 ou 0,72km/h com Vmax 183 km/h)) 
Bit 6 ==> Pluviômetro
Bit 7 ==> 


BC[1] ==> Byte de controle 2, pode ou não existir para definir outros campos com o byte de controle 1


## Build:0000-Base 05/12/2025
Wizard Raspberry Pi Pico para Pico 
