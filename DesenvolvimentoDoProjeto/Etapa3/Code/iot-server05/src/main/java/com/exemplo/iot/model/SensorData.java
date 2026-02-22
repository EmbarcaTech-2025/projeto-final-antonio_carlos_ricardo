package com.exemplo.iot.model;

import jakarta.persistence.*;
import java.time.Instant;

@Entity
public class SensorData {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String  deviceId;
    private Instant timestamp;
    private Double  temp;
    private Double  press;
    private Integer humi;

    // getters e setters
    public Long getId() { return id; }
    public void setId(Long id) { this.id = id; }
    public String getDeviceId() { return deviceId; }
    public void setDeviceId(String deviceId) { this.deviceId = deviceId; }
    public Instant getTimestamp() { return timestamp; }
    public void setTimestamp(Instant timestamp) { this.timestamp = timestamp; }
    public Double getTemp() { return temp; }
    public void setTemp(Double temp) { this.temp = temp; }
    public Double getPress() { return press; }
    public void setPress(Double press) { this.press = press; }
    public Integer getHumi() { return humi; }
    public void setHumi(Integer humi) { this.humi = humi; }
}

