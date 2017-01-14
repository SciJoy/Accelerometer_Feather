#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SFE_LSM9DS0.h>

//IMU
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
#define PRINT_CALCULATED


//SD
#define cardSelect 4
File logfile;

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}


void setup() {
 
 Serial.begin(115200);
  Serial.println("\r\nAnalog logger test");
  pinMode(13, OUTPUT);
  
  // put your setup code here, to run once:
// see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  char filename[15];
  strcpy(filename, "ACCLOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

   logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
}

//IMU
  uint16_t status = dof.begin();
 //Serial.print("LSM9DS0 WHO_AM_I's returned: 0x");
  //Serial.println(status, HEX);
  //Serial.println("Should be 0x49D4");

  
void loop() {
  // put your main code here, to run repeatedly:


digitalWrite(8, HIGH);

   logfile.print(millis());
   logfile.print(", "); logfile.print(dof.calcAccel(dof.ax), 2);
   logfile.print(", "); logfile.print(dof.calcAccel(dof.ay), 2);
   logfile.print(", "); logfile.println(dof.calcAccel(dof.az), 2);

   Serial.print(millis());
   Serial.print(", "); Serial.print(dof.calcAccel(dof.ax), 2);
   Serial.print(", "); Serial.print(dof.calcAccel(dof.ay), 2);
  Serial.print(", "); Serial.println(dof.calcAccel(dof.az), 2);
  digitalWrite(8, LOW);

  logfile.flush();
 

}
