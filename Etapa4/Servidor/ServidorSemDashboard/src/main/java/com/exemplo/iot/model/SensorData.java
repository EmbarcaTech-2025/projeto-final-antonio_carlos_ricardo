package com.exemplo.iot.model;

import jakarta.persistence.*;
import java.time.Instant;

@Entity
public class SensorData {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String  deviceName;
    private String  token;
    private String  deviceGroup;
    private Double  lat;
    private Double  lon;
    private Double  elev;
    private Instant timeAq;
    private Long    aqN;
    
    //sensors
    private Double  press;
    private Double  temp_p;
    private Double  humi;
    private Double  temp_h;
    private Double  lumi;
    //lumi_c
    private Double  wind_d;
    private Double  wind_s;
    private Double  rain;
    private Double  co;
    private Double  co2;
    private Double  o3;
    private Double  soil_t;
    private Double  soil_h;
    private Double  uv_index;

    // getters e setters
    public Long    getId()                            { return id; }
    public void    setId(Long id)                     { this.id = id; }
    
    public String  getDeviceName()                    { return deviceName; }
    public void    setDeviceName(String deviceName)   { this.deviceName = deviceName; }
    public String  getToken()                         { return token; }
    public void    setToken(String token)             { this.token = token; }    
    public String  getDeviceGroup()                   { return deviceGroup; }
    public void    setDeviceGroup(String deviceGroup) { this.deviceGroup = deviceGroup; }
    public Double  getLat()                           { return lat; }
    public void    setLat(Double lat)                 { this.lat = lat; }
    public Double  getLon()                           { return lon; }
    public void    setLon(Double lon)                 { this.lon = lon; }
    public Double  getElev()                          { return elev; }
    public void    setElev(Double elev)               { this.elev = elev; }      
    public Instant getTimeAq()                        { return timeAq; }
    public void    setTimeAq(Instant timeAq)          { this.timeAq = timeAq; }
    public Long    getAqN()                           { return aqN; }
    public void    setAqN(Long aqN)                   { this.aqN = aqN; }
    
    
    // Sensors
    public Double getPress()                          { return press; }
    public void   setPress(Double press)              { this.press = press; }
    
    public Double getTemp_p()                         { return temp_p; }
    public void   setTemp_p(Double temp_p)            { this.temp_p = temp_p; }
    
    public Double getHumi()                           { return humi; }
    public void   setHumi(Double humi)                { this.humi = humi; }
    
    public Double getTemp_h()                         { return temp_h; }
    public void   setTemp_h(Double temp_h)            { this.temp_h = temp_h; }
    
    public Double getLumi()                           { return lumi; }
    public void   setLumi(Double lumi)                { this.lumi = lumi; }
       //lumi_c
    public Double getWind_d()                         { return wind_d; }
    public void   setWind_d(Double wind_d)            { this.wind_d = wind_d; }
    
    public Double getWind_s()                         { return wind_s; }
    public void   setWind_s(Double wind_s)            { this.wind_s = wind_s; }
    
    public Double getRain()                           { return rain; }
    public void   setRain(Double rain)                { this.rain = rain; }
    
    public Double getCo()                             { return co; }
    public void   setCo(Double co)                    { this.co = co; }
    
    public Double getCo2()                            { return co2; }
    public void   setCo2(Double co2)                  { this.co2 = co2; }
    
    public Double getO3()                             { return o3; }
    public void   setO3(Double o3)                    { this.o3 = o3; }
    
    public Double getSoil_t()                         { return soil_t; }
    public void   setSoil_t(Double soil_t)            { this.soil_t = soil_t; }
    
    public Double getSoil_h()                         { return soil_h; }
    public void   setSoil_h(Double soil_h)            { this.soil_h = soil_h; }
    
    public Double getUv_index()                       { return uv_index; }
    public void   setUv_index(Double uv_index)        { this.uv_index = uv_index; }
}

