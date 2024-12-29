#include <Wire.h>

// I2C Address for ST25DV16
#define ST25DV16_I2C_ADDRESS 0x53

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  
  // Initialize I2C communication
  Wire.begin();
}

void loop() {
  // Define the total memory size to read
  const uint16_t totalMemorySize = 16; // Adjust this value based on the actual memory size of ST25DV16

  // Buffer to store read data
  uint8_t data = 0;
  uint8_t array[16];

  // Read and print each memory cell
  uint8_t address = 0x0B;
  for (uint16_t i = 0; i < totalMemorySize; i++) {
    if (readMemoryCell(address, &data)) {
      if (isprint(data)) {
        Serial.print((char)data);
      } else {
        Serial.print("$");
      }
      array[i] = data;
      address++;
    }
  }

  bool isChanged = false;
  //if it changed, then break from the loop
  while(!isChanged)
  {
    uint8_t address = 0x0B;
    for(int i = 0; i < totalMemorySize; i++)
    {
      if(readMemoryCell(address, &data))
      {
        if(array[i] != data)
        {
          isChanged = true;
          break; //break from "for" loop
        }
      }
      address++;
    }
    delay(2000); // wait 2 seconds until next iteration of "while" loop
  }



  delay(2000); // Wait before the next read
  Serial.flush();
  delay(2000);

}

bool readMemoryCell(uint16_t address, uint8_t *data) {
  Wire.beginTransmission(ST25DV16_I2C_ADDRESS);
  Wire.write((uint8_t)(address >> 8)); // MSB of the address
  Wire.write((uint8_t)(address & 0xFF)); // LSB of the address
  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  Wire.requestFrom(ST25DV16_I2C_ADDRESS, (uint8_t)1);
  if (Wire.available() != 1) {
    return false;
  }

  *data = Wire.read();
  return true;
}
