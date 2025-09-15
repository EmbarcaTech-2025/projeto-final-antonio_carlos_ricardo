import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.time.Instant;
import java.util.Random;
import java.time.Duration;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Locale;



public class IoT_Dummy_SendOldData {
    static String ip;
    static String porta;
    static String id;

    static Object[][] lista_estacoes = {
        {-22.817078367765316, -47.069743738306656, "Unicamp",      "PracaCicloBas"},
        {-22.820935131845136, -47.06643657463327,  "Unicamp",      "FEEC"},
        {-22.815215462130617, -47.072647229552004, "Unicamp",      "FalcuEdFisica"},
        {-22.8122782745061,   -47.06618578809472,  "Unicamp",      "CasaDoProfessor"},
        {-22.81489900050847,  -47.06582368994227,  "Unicamp",      "InstEconomia"},
        {-22.81731694678724,  -47.07224758076062,  "Unicamp",      "Restaurante"},
        {-22.814501581619666, -47.05932707900954,  "UnicampLeste", "HbR"},
        {-22.813265392781766, -47.06034631842581,  "UnicampLeste", "Vertice"},
        {-22.814897159692197, -47.05762119407571,  "UnicampLeste", "MuseuCiencias"},
        {-22.826197213852517, -47.064436988183786, "OutSide",      "HospitalUnicamp"},
        {-22.816070894056224, -47.04529674482993,  "OutSide",      "CPQD"},
        {-22.807911934313054, -47.05115468893139,  "OutSide",      "Sirius"},
        {-22.85607733231256,  -47.113017157584146, "OutSide",      "Amarais"}
    };

    public static void main(String[] args) throws Exception {
        if (args.length < 2) {
            System.out.println("Uso: java IoT_Dummy_SendOldData <ip> <porta>");
            return;
        }

        ip    = args[0];
        porta = args[1];
        
        for(int i=0;i<=12;i++){
            gera(i);
        }
    }
    public static void gera(int id_n) throws Exception {       
        String  deviceName = (String)lista_estacoes[id_n][3];
        Double  lat        = (Double)lista_estacoes[id_n][0];
        Double  lon        = (Double)lista_estacoes[id_n][1];
        String  group      = (String)lista_estacoes[id_n][2];
        
        System.out.println("\n\n\n\n*************** Gerando dados para " + deviceName + " ***************");
        System.out.println("Station Name: " + deviceName);
        System.out.println("Latitude:  " + lat);
        System.out.println("Longitude: " + lon);
        System.out.println();
        
        String  token      = "station_token";
        Double  elev       = 765.4;
        long    aqN        = 0L;

        Random random   = new Random();
        Instant timeNow = Instant.now();
        Instant timeAq  =  timeNow.minus(Duration.ofDays(8));
        
        while (timeAq.isBefore(timeNow)) {
            aqN++;
          
            //sensors
            Double  press    = 1000 +       random.nextDouble() * 20;   // 1000 a 1020 hPa
            Double  temp_p   = 25   +       random.nextDouble() * 10;   // 25   a 35°C
            Double  humi     = 40   +       random.nextDouble() * 20;   // 40   a 60 %
            Double  temp_h   = 15   +       random.nextDouble() * 10;   // 15   a 25°C
            Double  lumi     = 100  +       random.nextDouble() * 1000; // 100  a 1100 lux;
            //lumi_c
            Double  wind_d   = 120  +       random.nextDouble() * 90;   // 120  a 210 graus
            Double  wind_s   = 11   +       random.nextDouble() * 20;   // 11   a 31 m/s
            Double  rain     = 0    +       random.nextDouble() * 20;   // 0    a 20 
            Double  co       = 123  +       random.nextDouble() * 100;  // 123 a 223 ppm
            Double  co2      = 500  +       random.nextDouble() * 200;  // 500 a 700 ppm
            Double  o3       = 12   +       random.nextDouble() * 100;  // 12  a 112 ppm
            Double  soil_t   = 5    +       random.nextDouble() * 10;   // 5   a 15°C
            Double  soil_h   = 70   +       random.nextDouble() * 20;   // 70  a 90 %
            Double  uv_index = 1    +       random.nextDouble() * 11;   // 1   a 12 index
            
            String json = String.format(Locale.US,
                "{" +
                "\"name\":\"%s\","    + 
                "\"token\":\"%s\","   + 
                "\"group\":\"%s\","   + 
                "\"lat\":%.6f,"       +
                "\"lon\":%.6f,"       +
                "\"elev\":%.1f,"      +
                "\"time_aq\":\"%s\"," +
                "\"aq_n\":\"%d\","    +
                "\"sensors\":{"   +
                    "\"press\":%.2f,"   +
                    "\"temp_p\":%.2f,"  +
                    "\"humi\":%.2f,"    +
                    "\"temp_h\":%.2f,"  +
                    "\"lumi\":%.2f,"    +
//                  "\"lumi_c": tbd, 
                    "\"wind_d\":%.2f,"  +
                    "\"wind_s\":%.2f,"  +
                    "\"rain\":%.2f,"    +
                    "\"co\":%.2f,"      +
                    "\"co2\":%.2f,"     +
                    "\"o3\":%.2f,"      +
                    "\"soil_t\":%.2f,"  +
                    "\"soil_h\":%.2f,"  +
                    "\"uv_index\":%.2f" +
                    "}" +
                "}",
                deviceName,
                token,
                group,
                lat,
                lon,
                elev,
                timeAq.toString(),
                aqN,
                press,
                temp_p,
                humi,
                temp_h,
                lumi,
                //lumi_c
                wind_d,
                wind_s,
                rain,
                co,
                co2,
                o3,
                soil_t,
                soil_h,
                uv_index
                );                
            enviarPost(ip, porta, json);
            timeAq  =  timeAq.plus(Duration.ofSeconds(60));
        }
    }

    private static void enviarPost(String ip, String porta, String json) {
        System.out.println("Enviando: " + json);
        try {
            URL url = new URL("http://" + ip + ":" + porta + "/dados");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Content-Type", "application/json; charset=UTF-8");
            conn.setDoOutput(true);

            try (OutputStream os = conn.getOutputStream()) {
                byte[] input = json.getBytes(StandardCharsets.UTF_8);
                os.write(input, 0, input.length);
            }

            int code = conn.getResponseCode();
            System.out.println("Enviado para " + url + " - HTTP " + code);

            conn.disconnect();
        } catch (Exception e) {
            System.err.println("Erro ao enviar: " + e.getMessage());
        }
        System.out.println();
    }
}
