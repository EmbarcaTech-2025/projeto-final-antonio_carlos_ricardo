

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