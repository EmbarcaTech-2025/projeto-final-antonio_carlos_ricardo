package com.exemplo.iot;

import com.exemplo.iot.model.SensorData;
import com.exemplo.iot.repository.SensorDataRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.time.Instant;
import java.util.Map;

@RestController
public class IoTServer {

    @Autowired
    private SensorDataRepository repository;

    @PostMapping("/dados")
    public String receiveData(@RequestBody Map<String, Object> payload) {
        // Print raw JSON
        System.out.println("📩 JSON recebido (raw): " + payload);

        try {
            String deviceId = (String) payload.get("id");
            String timeStr = (String) payload.get("time");
            Map<String, Object> sensors = (Map<String, Object>) payload.get("sensors");
            Double  temp  = Double.valueOf(sensors.get("temp").toString());
            Double  press = Double.valueOf(sensors.get("press").toString());
            Integer humi  = Integer.valueOf(sensors.get("humi").toString());

            SensorData data = new SensorData();
            data.setDeviceId(deviceId);
            data.setTimestamp(Instant.parse(timeStr));
            data.setTemp(temp);
            data.setPress(press);

            repository.save(data);

        } catch (Exception e) {
            e.printStackTrace();
            return "Erro ao processar dados";
        }

        return "OK";
    }
}

