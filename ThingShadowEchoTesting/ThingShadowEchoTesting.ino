#include <aws_iot_mqtt.h>
#include <aws_iot_version.h>
#include "aws_iot_config.h"
#include <ArduinoJson.h>
#include <Process.h>

Process date;  
aws_iot_mqtt_client myClient;
char JSON_buf[50];
char update_buf[400];
int cnt = 0;
int rc = 1;
bool success_connect = false;
char deltaLoggingTemp[7], deltaSampleRateTemp[7], deltaStartDateTemp[11], deltaStartTimeTemp[6], deltaEndDateTemp[11], deltaEndTimeTemp[6], deltaActuatorTemp[7], deltaTriggerTemp[5], deltaPowerTemp[5], deltaActuatorDurationTemp[7];


bool print_log(const char* src, int code) {
  bool ret = true;
  if(code == 0) {
    #ifdef AWS_IOT_DEBUG
      Serial.print(F("[LOG] command: "));
      Serial.print(src);
      Serial.println(F(" completed."));
    #endif
    ret = true;
  }
  else {
    #ifdef AWS_IOT_DEBUG
      Serial.print(F("[ERR] command: "));
      Serial.print(src);
      Serial.print(F(" code: "));
     Serial.println(code);
    #endif
    ret = false;
  }
  Serial.flush();
  return ret;
}

void msg_callback_delta(char* src, unsigned int len, Message_status_t flag) {
  if(flag == STATUS_NORMAL) {

StaticJsonBuffer<370> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

    //Temperature Delta  
    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Logging", deltaLoggingTemp, 7));
    
    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Sample Rate", deltaSampleRateTemp, 7));

    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Start Date", deltaStartDateTemp, 11));
 
    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Start Time", deltaStartTimeTemp, 6));

    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"End Date", deltaEndDateTemp, 11));
   
    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"End Time", deltaEndTimeTemp, 6));

    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Actuator", deltaActuatorTemp, 7));
     
    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Trigger", deltaTriggerTemp, 5));
 
    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Power", deltaPowerTemp, 5));

    print_log("getDeltaKeyValue", myClient.getDeltaValueByKey(src, "temperature\"Actuator Duration", deltaActuatorDurationTemp, 7));

JsonObject &state = root.createNestedObject("state");
JsonObject &reported = state.createNestedObject("reported");
JsonObject &temperature = reported.createNestedObject("temperature");
//JsonObject &reported1 = reported.createNestedObject("temp");
temperature["Logging"] = deltaLoggingTemp;
temperature["Sample Rate"] = deltaSampleRateTemp;
temperature["Start Date"] = deltaStartDateTemp;
temperature["Start Time"] = deltaStartTimeTemp;
temperature["End Date"] = deltaEndDateTemp;
temperature["End Time"] = deltaEndTimeTemp;
temperature["Actuator"] = deltaActuatorTemp;
temperature["Trigger"] = deltaTriggerTemp;
temperature["Power"] = deltaPowerTemp;
temperature["Actuator Duration"] = deltaActuatorDurationTemp;

root.printTo(Serial);
Serial.println();
root.prettyPrintTo(Serial);
root.printTo(update_buf, 400);
print_log(update_buf, 400);
print_log("update thing shadow", myClient.shadow_update(AWS_IOT_MY_THING_NAME, update_buf, strlen(update_buf), NULL, 5));

if (strcmp(deltaLoggingTemp, "active") == 0)
        log_temperature(HIGH);


  }
}

void setup() {
  Bridge.begin(); 
  Serial.begin(115200);
  while(!Serial);

  char curr_version[80];
  snprintf_P(curr_version, 80, PSTR("AWS IoT SDK Version(dev) %d.%d.%d-%s\n"), VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TAG);
  Serial.println(curr_version);

  if(print_log("setup", myClient.setup(AWS_IOT_CLIENT_ID))) {
    if(print_log("config", myClient.config(AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT, AWS_IOT_ROOT_CA_PATH, AWS_IOT_PRIVATE_KEY_PATH, AWS_IOT_CERTIFICATE_PATH))) {
      if(print_log("connect", myClient.connect())) {
        success_connect = true;
        print_log("shadow init", myClient.shadow_init(AWS_IOT_MY_THING_NAME));
        print_log("register thing shadow delta function", myClient.shadow_register_delta_func(AWS_IOT_MY_THING_NAME, msg_callback_delta));
      }
    }
  }


  
}

void loop() {
  if(success_connect) {
    if(myClient.yield()) {
      Serial.println(F("Yield failed."));
    }
    delay(1000); // check for incoming delta per 100 ms

    //if boolean is true{
    //slotType = "actuator";
    //nullValueAfterLoggingDone(slotType);
    //set boolean to false;
    //}
  }
}

void log_temperature(uint8_t state)
{
  date.begin("/bin/date");
  date.addParameter("+%d/%m/%Y %T");
  date.run();
  
  //if there's a result from the date process, get it.
  while (date.available()>0) {
    String timeString = date.readString();    
    // print the results we got.
    Serial.print(timeString);
  }
    
    

}
