import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.time.Instant;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Locale;

public class IoT_Dummy_GetTime {

    public static void main(String[] args) throws Exception {
        if (args.length < 3) {
            System.out.println("Uso: java IoT_Dummy_GetTime <ip> <porta> <id_dispositivo>");
            return;
        }

        String ip = args[0];
        String porta = args[1];
        String id = args[2];

        String jsonReq = String.format(Locale.US, "{\"id\":\"%s\"}", id);
        String jsonResp = enviarPost(ip, porta, jsonReq);

        if (jsonResp != null) {
            System.out.println("JSON recebido: " + jsonResp);
        }
    }

    private static String enviarPost(String ip, String porta, String json) {
        try {
            URL url = new URL("http://" + ip + ":" + porta + "/hora");
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Content-Type", "application/json; charset=UTF-8");
            conn.setDoOutput(true);

            try (OutputStream os = conn.getOutputStream()) {
                byte[] input = json.getBytes(StandardCharsets.UTF_8);
                os.write(input, 0, input.length);
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream(), StandardCharsets.UTF_8));
            StringBuilder response = new StringBuilder();
            String line;
            while ((line = br.readLine()) != null) {
                response.append(line.trim());
            }

            conn.disconnect();
            return response.toString();

        } catch (Exception e) {
            System.err.println("Erro ao enviar: " + e.getMessage());
            return null;
        }
    }
}

