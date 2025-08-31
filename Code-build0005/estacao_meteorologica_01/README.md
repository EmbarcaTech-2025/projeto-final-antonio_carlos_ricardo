# Exemplo de maior mensagem a ser enviada

405 bytes
{"name":"1234567890abcdef","token":"1234567890abcdef","group":"1234567890abcdef","lat":-123.456789,"lon":-123.456789,"elev":10000.2,"time_aq":"2025-08-29T12:53:13.298237953Z","sensors":{"press":1013.25,"temp_p":123.45,"humi":100.00,"temp_h":123.45,"lumi":30000.12,"wind_d":321.25,"wind_s":27.12,"rain":200.12,"co":100000.12,"co2":100000.12,"o3":100000.12,"soil_t":123.45,"soil_h":100.00,"uv_index":12.34}}


# TIME

timestamp Unix: número de segundos desde 1º de janeiro de 1970 00:00:00 UTC

#include <time.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    time_t now = time(NULL);  // segundos desde 01/01/1970 UTC
    printf("%ld\n", (long) now);
    return 0;
}

printf("{\"timestamp\": %ld}\n", (long) now);




long timestamp = jsonNode.get("timestamp").asLong();
Instant instant = Instant.ofEpochSecond(timestamp);
ZonedDateTime dateTime = instant.atZone(ZoneId.of("UTC"));
System.out.println(dateTime);

long now = Instant.now().getEpochSecond();
return Map.of("timestamp", now);