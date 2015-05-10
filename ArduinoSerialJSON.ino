//------------------------------------------------------SENSOR-VARIABLES
const int sensors[] = {A0, A1, A2, A3, A4, A5};         // array of sensor pins
int values[] = {0, 0, 0, 0, 0, 0};                      // values read from the sensors
boolean states[] = {true, true, true, true, true, true};// on/off states of sensors
int sensor_count = 6;                                   // number of sensors used
 
//---------------------------------------------------------LED-VARIABLES
const int status_led = 6;                               // pin
const int button = 7;                                   // button for state changes
int status_led_state = 0;                               // circular integer for state
float status_led_oscillation = 0;                       // value for the oscillation
float status_led_oscillation_variance = 0.05;           // oscillation variance
boolean status_led_oscillation_direction = true;        // oscillation direction
 
//------------------------------------------------TRANSMISSION-VARIABLES
String JSON_string;                                     // transmission string
int transmission_number;                                // transmission id
int transmissions_per_second = 5;                       // frequency
long wait = (1000/transmissions_per_second);            // wait time between
long previous_millis = 0;                               // previous transmission time
 
 
//---------------------------------------------------------------
//-----------------------------SETUP-----------------------------
//---------------------------------------------------------------
void setup(){
 
//-----------------------------------------------------------PINS
  Serial.begin(9600);                                   // starting serial
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
 
  pinMode(status_led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
 
  set_sensor_state();
  status_led_action();
}
 
 
//---------------------------------------------------------------
//-----------------------------LOOP------------------------------
//---------------------------------------------------------------
void loop()
{  
  if( wait < (millis() - previous_millis))
  {
    get_sensor_values();
    build_transmission();
    Serial.println(JSON_string);
    status_led_action();//---------------------------------ACTION
    previous_millis = millis();
  }
  status_led_action();//-----------------------------------ACTION
  status_led_button();
}
 
 
//---------------------------------------------------------------
//-----------------------SENSOR-METHODS--------------------------
//---------------------------------------------------------------
void toggle_sensor_state(int sensor)
{
    states[sensor] != states[sensor];
    set_sensor_state();
}
void set_sensor_state()
{
  for(int i = 0; i < sensor_count; i++)
  {
      if(states[i])                                               // state?
      {
          digitalWrite(i, HIGH);
      }
      else
      {
          digitalWrite(i, LOW);
      }
  }
}
void get_sensor_values()
{
  for(int i = 0; i < sensor_count; i++)
  {
    values[i] = analogRead(sensors[i]);
  }
}
 
 
 
//---------------------------------------------------------------
//---------------------LED-STATUS-METHODS------------------------
//---------------------------------------------------------------
void status_led_action()
{
    if(status_led_state == 0) //-------------------turned on mode
    {
        analogWrite(status_led, 255);
    }
    else if (status_led_state == 1) //---------------looping mode
    {
        //--------------INCREMENT OR DECREMENT BASED ON DIRECTION
        if(status_led_oscillation_direction){
            status_led_oscillation += status_led_oscillation_variance;
        }
        else
        {
            status_led_oscillation -= status_led_oscillation_variance;
        }
       
        //-----------------TOGGLE DIRECTION IF SOON OUT OF BOUNDS
        if(status_led_oscillation>254.0 || status_led_oscillation< 1.0)
        {
            status_led_oscillation_direction != status_led_oscillation_direction;
        }
       
        analogWrite(status_led, ((int) status_led_oscillation));
    }
    else if (status_led_state == 2) //----------transmission mode
    {
        if(status_led_oscillation_direction)
        {
            analogWrite(status_led, 255);
            status_led_oscillation_direction = false;
        }
        else
        {
            analogWrite(status_led, 0);
            status_led_oscillation_direction = true;
        }
    }
}
void status_led_button(){
    int val = digitalRead(button);
    if(val == 0){
        analogWrite(status_led, 255);
        turn_status_led_state();
        delay(2000);
        analogWrite(status_led, 0);
    }
}
void turn_status_led_state()
{
    if(status_led_state <2)
    {
        status_led_state++;
    }
    else
    {
        status_led_state = 0;
    }
}
 
 
 
//---------------------------------------------------------------
//--------------------TRANSMISSION-METHODS-----------------------
//---------------------------------------------------------------
void build_transmission()
{
  JSON_string = "";
 
  JSON_string += "{\"transmission\":" ;
  JSON_string += transmission_number++;
  JSON_string += "{\"transmission\":" ;
  JSON_string += transmission_number++;
  JSON_string += ",\"sensor\":[";
 
  for(int i = 0; i < sensor_count; i++)
  {
    JSON_string += "{\"sensor\":\"";
    JSON_string += sensors[i];
    JSON_string += "\",\"value\":";
    JSON_string += values[i];
    JSON_string += "\",\"state\":";
    JSON_string += states[i];
    JSON_string += "}";
    if(i < (sensor_count - 1))
    {
      JSON_string += ",";
    }
  }
  JSON_string += "]}";
}
