public class Decode {

/*
    Version: 0.00.00 - 04/01/2026

    $ javac Decode.java
    $ java Decode BCHEX


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

*/

    public static void main(String[] args) {

        if (args.length != 1) {
            System.out.println("Uso: java Decode <hex_string>");
            return;
        }

        byte[] data = hexToBytes(args[0]);
        int index = 0;

        int bc = data[index++] & 0xFF;

        System.out.println("Byte de Controle (BC): 0x" + String.format("%02X", bc));

        /* Bit 0 - Battery */
        if ((bc & 0x01) != 0) {
            int bat = data[index++] & 0xFF;
            if (bat == 0xFF) {
                System.out.println("Battery: ERRO");
            } else {
                System.out.println("Battery: " + (bat * 0.5) + " %");
            }
        }

        /* Bit 1 - BME280 */
        if ((bc & 0x02) != 0) {

            // Umidade
            int hum = data[index++] & 0xFF;
            if (hum == 0xFF) {
                System.out.println("Umidade: ERRO");
            } else {
                System.out.println("Umidade: " + (hum * 0.5) + " %");
            }

            // Temperatura (2 bytes signed)
            int tempRaw = readInt16(data, index);
            index += 2;
            if (tempRaw == 0x7FFF) {
                System.out.println("Temperatura: ERRO");
            } else {
                System.out.println("Temperatura: " + (tempRaw / 100.0) + " °C");
            }

            // Pressão (2 bytes unsigned)
            //int presRaw = readUInt16(data, index);
            int presRaw = readInt16(data, index);
            index += 2;
            if (presRaw == 0xFFFF) {
                System.out.println("Pressão: ERRO");
            } else {
                double pressao = (60000 - presRaw) * 0.02;
                System.out.println("Pressão: " + pressao + " hPa");
            }
        }

        /* Bit 2 - GPS */
        if ((bc & 0x04) != 0) {

            int latRaw = readInt32(data, index);
            index += 4;
            if (latRaw == 0x7FFFFFFFL) {
                System.out.println("Latitude: ERRO");
            } else {
                //float lat = latRaw * 512.0 / Math.pow(2, 32);
                float lat = (float)(latRaw / 8388608.0);
                System.out.println("Latitude: " + lat + " graus");
            }

            int lonRaw = readInt32(data, index);
            index += 4;
            if (lonRaw == 0x7FFFFFFFL) {
                System.out.println("Longitude: ERRO");
            } else {
                //float lon = lonRaw * 512.0 / Math.pow(2, 32);
                float lon = (float)(lonRaw / 8388608.0);
                System.out.println("Longitude: " + lon + " graus");
            }

            int altRaw = readInt16(data, index);
            index += 2;
            if (altRaw == 0x7FFF) {
                System.out.println("Altitude: ERRO");
            } else {
                System.out.println("Altitude: " + (altRaw * 0.1) + " m");
            }
        }
    }

    /* ===================== UTILITÁRIOS ===================== */

    private static byte[] hexToBytes(String hex) {
        int len = hex.length();
        byte[] out = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            out[i / 2] = (byte) Integer.parseInt(hex.substring(i, i + 2), 16);
        }
        return out;
    }

    private static int readInt16(byte[] data, int idx) {
        return (short) ((data[idx] << 8) | (data[idx + 1] & 0xFF));
    }

    private static int readUInt16(byte[] data, int idx) {
        return ((data[idx] & 0xFF) << 8) | (data[idx + 1] & 0xFF);
    }

    private static int readInt32(byte[] data, int idx) {
        return ((int)(data[idx] & 0xFF) << 24) |
               ((int)(data[idx + 1] & 0xFF) << 16) |
               ((int)(data[idx + 2] & 0xFF) << 8) |
               ((int)(data[idx + 3] & 0xFF));
    }
}

