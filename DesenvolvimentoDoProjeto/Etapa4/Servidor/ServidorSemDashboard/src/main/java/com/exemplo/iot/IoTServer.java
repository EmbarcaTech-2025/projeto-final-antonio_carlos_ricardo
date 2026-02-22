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
        System.out.println("JSON recebido (raw): " + payload);
        System.out.println();

        try {
            Map<String, Object> sensors = (Map<String, Object>) payload.get("sensors");

            SensorData data = new SensorData();
            
            data.setDeviceName( (String) payload.get("name"));
            data.setToken(      (String) payload.get("token"));
            data.setDeviceGroup((String) payload.get("group"));
    
            data.setTimeAq(  payload.get("time_aq" ) != null ? Instant.parse((String) payload.get("time_aq")) : null);
            data.setAqN(     payload.get("aq_n"    ) != null ? Long.valueOf(payload.get(  "aq_n"    ).toString()) : null); 
    
            data.setLat(     payload.get("lat"     ) != null ? Double.valueOf(payload.get("lat"     ).toString()) : null);
            data.setLon(     payload.get("lon"     ) != null ? Double.valueOf(payload.get("lon"     ).toString()) : null);
            data.setElev(    payload.get("elev"    ) != null ? Double.valueOf(payload.get("elev"    ).toString()) : null);
            
            //sensors
            data.setPress(   sensors.get("press"   ) != null ? Double.valueOf(sensors.get("press"   ).toString()) : null);
            data.setTemp_p(  sensors.get("temp_p"  ) != null ? Double.valueOf(sensors.get("temp_p"  ).toString()) : null);
            data.setHumi(    sensors.get("humi"    ) != null ? Double.valueOf(sensors.get("humi"    ).toString()) : null);
            data.setTemp_h(  sensors.get("temp_h"  ) != null ? Double.valueOf(sensors.get("temp_h"  ).toString()) : null);
            data.setLumi(    sensors.get("lumi"    ) != null ? Double.valueOf(sensors.get("lumi"    ).toString()) : null);
            //lumi_c
            data.setWind_d(  sensors.get("wind_d"  ) != null ? Double.valueOf(sensors.get("wind_d"  ).toString()) : null);
            data.setWind_s(  sensors.get("wind_s"  ) != null ? Double.valueOf(sensors.get("wind_s"  ).toString()) : null);
            data.setRain(    sensors.get("rain"    ) != null ? Double.valueOf(sensors.get("rain"    ).toString()) : null);
            data.setCo(      sensors.get("co"      ) != null ? Double.valueOf(sensors.get("co"      ).toString()) : null);
            data.setCo2(     sensors.get("co2"     ) != null ? Double.valueOf(sensors.get("co2"     ).toString()) : null);
            data.setO3(      sensors.get("o3"      ) != null ? Double.valueOf(sensors.get("o3"      ).toString()) : null);
            data.setSoil_t(  sensors.get("soil_t"  ) != null ? Double.valueOf(sensors.get("soil_t"  ).toString()) : null);
            data.setSoil_h(  sensors.get("soil_h"  ) != null ? Double.valueOf(sensors.get("soil_h"  ).toString()) : null);
            data.setUv_index(sensors.get("uv_index") != null ? Double.valueOf(sensors.get("uv_index").toString()) : null);
            
            repository.save(data);
        } catch (Exception e) {
            e.printStackTrace();
            return "Erro ao processar dados:" + e.toString();
        }

        return "OK";
    }
}

