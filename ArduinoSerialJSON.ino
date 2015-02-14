/*
{
  "transmission":12
  "sensor": [
    {
      "sensor": "A0",
      "value": 200
    },
    {
      "sensor": "A1",
      "value": 400
    },
    {
      "sensor": "A2",
      "value": 600
    }
  ]
}
*/

const int sensors[] = {A0, A1, A2, A3, A4, A5};       // array of sensor pins
int values[] = {0, 0, 0, 0, 0, 0};                    // values read from the sensors
int sensor_count = 4;                                 // number of sensors used
String JSON_string;
int transmission_number; 


void setup()
{
  Serial.begin(9600);
}


void loop()
{
  get_sensor_values();
  build_transmission();
 
  Serial.println(JSON_string);
 
  transmission_number += 1;
  JSON_string = ""; 
}

void get_sensor_values(){
 for(int i = 0; i < sensor_count; i++){
   values[i] = analogRead(sensors[i]);
 } 
}

void build_transmission(){
  JSON_string += "{\"transmission\":" ;
  JSON_string += transmission_number;
  JSON_string +=",\"sensor\":[";
 
  for(int i = 0; i < sensor_count; i++)
  {
    JSON_string += "{\"sensor\":\"";
    JSON_string += sensors[i];
    JSON_string += "\",\"value\":";
    JSON_string += values[i];
    JSON_string += "}";
    if(i < sensor_count - 1) 
  {
    JSON_string += ",";
  }
  
   } 
  JSON_string += "]}";
}
