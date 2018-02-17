#include <Process.h>

Process date;                 // process used to get the date
int hours, minutes, seconds, days, years;  // for the results
void setup() {
  Bridge.begin();        // initialize Bridge
  Serial.begin(9600);    // initialize serial  

  while(!Serial);               // wait for Serial Monitor to open

  Serial.println("Time Check");  // Title of sketch
}

void loop() {
  date.begin("/bin/date");
  date.addParameter("+%d/%m/%Y %T");
  date.run();
  
  //if there's a result from the date process, get it.
  while (date.available()>0) {
    String timeString = date.readString();    
    // print the results we got
     int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");
    int firstSpace = timeString.indexOf(" ");
    int firstSlash = timeString.indexOf("/");
    int secondSlash = timeString.lastIndexOf("/");

    // get the substrings for hour, minute second:
    String dayString = timeString.substring(0, firstSlash);
    String monthString = timeString.substring(firstSlash + 1, secondSlash);
    String yearString = timeString.substring(secondSlash + 1, firstSpace);
    String hourString = timeString.substring(firstSpace+1, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

   
     Serial.println(timeString);
     Serial.println(dayString);
     Serial.println(monthString);
     Serial.println(yearString);
     Serial.println(hourString);
     Serial.println(minString);
     Serial.println(secString);

  
//Serial.println(days);
//     Serial.println(%T);

  }
  
  delay(5000); // wait 5000ms
}
