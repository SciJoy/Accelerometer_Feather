#include <SPI.h>
#include <SD.h>

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

int acceleration [] = {0,0,0,0};

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

void loop() {
  // put your main code here, to run repeatedly:



/* for (int i =0; i<2; I++){
  acceleration [i] = analogRead(i));
}

acceleration[3]= millis(); */

digitalWrite(8, HIGH);

   logfile.print(millis());
   logfile.print(", "); logfile.print(analogRead(0));
   logfile.print(", "); logfile.print(analogRead(1));
   logfile.print(", "); logfile.println(analogRead(2));

   Serial.print(millis());
   Serial.print(", "); Serial.print(analogRead(0));
   Serial.print(", "); Serial.print(analogRead(1));
  Serial.print(", "); Serial.println(analogRead(2));
  digitalWrite(8, LOW);

  logfile.flush();
 

}
