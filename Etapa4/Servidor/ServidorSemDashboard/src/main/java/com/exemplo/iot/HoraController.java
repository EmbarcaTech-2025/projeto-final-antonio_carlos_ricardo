package com.exemplo.iot;

import org.springframework.web.bind.annotation.*;
import java.time.Instant;
import java.util.Map;

@RestController
public class HoraController {

    @PostMapping("/hora")
    public Map<String, String> getHora(@RequestBody Map<String, String> payload) {
//    public Map<String, Long> getHora(@RequestBody Map<String, String> payload) {
        String id    = payload.get("id");
        String token = payload.get("token");
        System.out.println("Requisição de hora para dispositivo: " + id + " com token: " + token);
        System.out.println();
//        return Map.of("time", Instant.now().getEpochSecond());
        String s_time = Instant.now().toString();
        return Map.of("time", s_time);        
    }
}

