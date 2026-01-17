// ***** Payload Decoder: Build: 0003 17/01/2026 *****

function toInt8(value) {
  return value & 0x80 ? value - 0x100 : value;
}


function readUInt16(bytes, idx) {
  return (bytes[idx] << 8) | bytes[idx + 1];
}

function toInt16(value) {
  return value & 0x8000 ? value - 0x10000 : value;
}

function readInt32(bytes, idx) {
  let value =
    (bytes[idx] << 24) |
    (bytes[idx + 1] << 16) |
    (bytes[idx + 2] << 8) |
    (bytes[idx + 3]);

  // força signed 32 bits
  return value | 0;
}


function decodeUplink(input) {
  const bytes = input.bytes;
  let index = 0;
  let data = {};

  // =====================
  // Byte de Controle
  // =====================
  const bc = bytes[index++] & 0xFF;
  data.BC = "0x" + bc.toString(16).padStart(2, "0").toUpperCase();

  // =====================
  // Bit 0 – Battery
  // =====================
  if (bc & 0x01) {
    const bat = bytes[index++] & 0xFF;
    if (bat === 0xFF) {
      data.Battery = "ERROR";
    } else {
      data.Battery = bat * 0.5; // %
    }
  }

  // =====================
  // Bit 1 – BME280
  // =====================
  if (bc & 0x02) {
    // Umidade
    const humidity = bytes[index++] & 0xFF;
    if (humidity === 0xFF) {
      data.Humidity = "ERROR";
    } else {
      data.Humidity = humidity * 0.5; // %
    }

    // Temperatura (signed int16)
    const tempRaw = readUInt16(bytes, index);
    index += 2;
    if (tempRaw === 0x7FFF) {
      data.Temperature = "ERROR";
    } else {
      data.Temperature = toInt16(tempRaw) / 100.0; // °C
    }

    // Pressão (signed int16, mesma lógica do Java)
    const presRaw = readUInt16(bytes, index);
    index += 2;
    if (presRaw === -1) { // 0xFFFF
      data.Pressure = "ERROR";
    } else {
      data.Pressure = (60000 - toInt16(presRaw)) * 0.02; // hPa
    }
  }

  // =====================
  // Bit 2 – GPS
  // =====================
  if (bc & 0x04) {
    // Latitude
    const latRaw = readInt32(bytes, index);
    index += 4;
    if (latRaw === 0x7FFFFFFF) {
      data.Latitude = "ERROR";
    } else {
      data.Latitude = latRaw / 8388608.0; // graus
    }

    // Longitude
    const lonRaw = readInt32(bytes, index);
    index += 4;
    if (lonRaw === 0x7FFFFFFF) {
      data.Longitude = "ERROR";
    } else {
      data.Longitude = lonRaw / 8388608.0; // graus
    }

    // Altitude
    const altRaw = readUInt16(bytes, index);
    index += 2;
    if (altRaw === 0x7FFF) {
      data.Altitude = "ERROR";
    } else {
      data.Altitude = toInt16(altRaw) * 0.1; // metros
    }
  }
  
  // =====================
  // Bit 3 – Lux Meter
  // =====================
  if (bc & 0x08) {
    const luminosity = readUInt16(bytes, index);
    index += 2;
    if (luminosity === 0xFFFF) {
      data.Luminosity = "ERROR";
    } else {
      data.Luminosity = luminosity; // lux
    }
  }
  
  // =====================
  // Bit 7 – CPU Temp
  // =====================
  if (bc & 0x80) {
    const mpu_temp = bytes[index++] & 0xFF;
    if (mpu_temp === 0x7F) {
      data.MpuTemperature = "ERROR";
    } else {
      data.MpuTemperature = toInt8(mpu_temp); // °C
    }
  }
  
  const agora = new Date();
  data.AqTimestamp_ms  = agora.getTime();         // Data/hora em milissegundos desde o Epoch (01/01/1970 UTC)
  data.AqTimestamp_iso = agora.toISOString();     // Data/hora em string no formato ISO 8601

  return {
    data: data
  };
}



// ===================== TESTE =====================
const input = {
  bytes: [
    0x87,                   // BC (Battery + BME + GPS + CPUTemp)
    0xAF,                   // Battery = 175      → 87,5%
    0x6C,                   // Umidade = 108      → 54,0%
    0x09, 0xEF,             // Temperatura = 2543 → 25,43 °C
    0x24, 0x79,             // Pressão     = 9337 → 1013,26 hPa
    0x00, 0x80, 0x00, 0x00, // Latitude
    0x00, 0x40, 0x00, 0x00, // Longitude
    0x1D, 0x08,             // Altitude  = 7432   → 743,2 m
    0XD4, 0X31,             // Luximetro = 54321
    0xE9                    // CPU Temp  = 233    → -23 °C
  ]
};

const input2 = {
  bytes: [
    0x87,                   // BC (Battery + BME + GPS + CPUTemp)
    0x99,                   // Battery     = 153                 →   76,5%
    0x45,                   // Umidade     =  69                 →   34,5%
    0x09, 0xEF,             // Temperatura = 2543                →   25,43 °C
    0x24, 0x7A,             // Pressão     = 9338                → 1013,24 hPa
    0xF4, 0x97, 0x6A, 0x7B, // Latitude    = -191403397          →  -22,817063
    0xE8, 0x77, 0x12, 0x09, // Longitude   = -394849782 E877120A →  -47,069762
    0x10, 0xE1,             // Altitude    = 7432                →  743,2 m
    0x7E                    // CPU Temp    =  126                →  126 °C
  ]
};


const input3 = {
  bytes: [
    0x87,                   // BC (Battery + BME + GPS + CPUTemp)
    0x99,                   // Battery     = 153                 →   76,5%
    0x45,                   // Umidade     =  69                 →   34,5%
    0x09, 0xEF,             // Temperatura = 2543                →   25,43 °C
    0x24, 0x7A,             // Pressão     = 9338                → 1013,24 hPa
    0xF4, 0x97, 0x6A, 0x7B, // Latitude    = -191403397          →  -22,817063
    0xE8, 0x77, 0x12, 0x09, // Longitude   = -394849782 E877120A →  -47,069762
    0x10, 0xE1,             // Altitude    = 7432                →  743,2 m
    0x80                    // CPU Temp    =  128                →  -127 °C
  ]
};

const hex = input.bytes
  .map(b => b.toString(16).padStart(2, "0"))
  .join(" ");
console.log("Input (hex):", hex);
const result = decodeUplink(input);
console.log(JSON.stringify(result, null, 2));


const hex2 = input2.bytes
  .map(b => b.toString(16).padStart(2, "0"))
  .join(" ");
console.log("Input2 (hex):", hex2);
const result2 = decodeUplink(input2);
console.log(JSON.stringify(result2, null, 2));

const hex3 = input3.bytes
  .map(b => b.toString(16).padStart(2, "0"))
  .join(" ");
console.log("Input3 (hex):", hex3);
const result3 = decodeUplink(input3);
console.log(JSON.stringify(result3, null, 2));
