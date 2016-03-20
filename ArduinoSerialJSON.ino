//------------------------------------------------------STATE-VARIABLES
const int button = 9;                                   // button for state changes
int machine_state = 0;                                  // circular integer for state

//------------------------------------------------------LED-VARIABLES
const int led = 6;                                      // pin
float led_intensity = 0;                                // value for the oscillation
float led_oscillation_variance = 0.006;                 // oscillation variance
boolean led_oscillation_direction = true;               // oscillation direction

//------------------------------------------------------SENSOR-VARIABLES
const int sensors[] = {A0, A1, A2, A3, A4, A5};         // array of sensor pins
int values[] = {0, 0, 0, 0, 0, 0};                      // values read from the sensors
boolean states[] = {true, true, true, true, true, true};// on/off states of sensors
int sensor_count = 6;                                   // number of sensors used

//------------------------------------------------------TRANSMISSION-VARIABLES
String JSON_string;                                     // transmission string
int transmission_number;                                // transmission id
int transmissions_per_second = 10;                       // frequency
long previous_millis = 0;                               // previous transmission time
long wait;                                              // wait time between



//---------------------------------------------------------------
//-----------------------------SETUP-----------------------------
//---------------------------------------------------------------
void setup() {
  //-----------------------------------------------------------TIME
  wait = (1000 / transmissions_per_second);
  //-----------------------------------------------------------PINS
  Serial.begin(9600);                                   // starting serial
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  update_sensor_state();
  led_action();
}


//---------------------------------------------------------------
//-----------------------------LOOP------------------------------
//---------------------------------------------------------------
void loop()
{
  if ( wait < (millis() - previous_millis))
  {
    if (machine_state == 0 || machine_state == 1)
    {
      get_sensor_values();
      build_transmission();
      Serial.println(JSON_string);
    }
    led_action();
    previous_millis = millis();
  }
  if (machine_state == 2)
  {
    led_action();//------------------------------------LED-ACTION
  }

  machine_state_button();
}


//---------------------------------------------------------------
//-----------------------SENSOR-METHODS--------------------------
//---------------------------------------------------------------
void update_sensor_state()
{
  for (int i = 0; i < sensor_count; i++)
  {
    if (states[i])                                              // state?
    {
      digitalWrite(i, HIGH);
    }
    else
    {
      digitalWrite(i, LOW);
    }
  }
}
void toggle_sensor_state(int sensor)
{
  states[sensor] != states[sensor];
  update_sensor_state();
}
void get_sensor_values()
{
  for (int i = 0; i < sensor_count; i++)
  {
    values[i] = analogRead(sensors[i]);
  }
}



//---------------------------------------------------------------
//---------------------LED-STATUS-METHODS------------------------
//---------------------------------------------------------------
void led_action()
{
  if (machine_state == 0) //--------------------transmission mode
  {
    led_action_type_zero();
  }
  else if (machine_state == 1) //---------------short transmission mode
  {
    led_action_type_zero();
  }
  else if (machine_state == 2)
  {
    led_action_type_one();
  }

  analogWrite(led, (int) led_intensity);
}

void led_action_type_zero()
{
  if (led_oscillation_direction)
  {
    //led_intensity = 255.0;
    led_oscillation_direction = false;
  }
  else
  {
    led_intensity = 0.0;
    led_oscillation_direction = true;
  }
}

void led_action_type_one()
{
  //--------------INCREMENT OR DECREMENT BASED ON DIRECTION
  if (led_oscillation_direction) {
    led_intensity += led_oscillation_variance;
  }
  else
  {
    led_intensity -= led_oscillation_variance;
  }

  //-----------------TOGGLE DIRECTION IF SOON OUT OF BOUNDS
  if ((led_intensity > (255.0 - (led_oscillation_variance / 2))) || (led_intensity < (led_oscillation_variance / 2)))
  {
    if (led_oscillation_direction)
    {
      led_oscillation_direction = false;
    }
    else
    {
      led_oscillation_direction = true;
    }

  }
}

void machine_state_button() {
  int val = digitalRead(button);
  if (val == 1) {
    led_intensity = 255.0;
    led_oscillation_direction = true;
    analogWrite(led, (int)led_intensity);
    turn_machine_state();
    delay(5000);
    led_intensity = 0.0;
    analogWrite(led, (int)led_intensity);
  }
}

void turn_machine_state()
{
  if (machine_state < 2)
  {
    machine_state++;
  }
  else
  {
    machine_state = 0;
  }
}



//---------------------------------------------------------------
//--------------------TRANSMISSION-METHODS-----------------------
//---------------------------------------------------------------
void build_transmission()
{
  //RESET
  JSON_string = "";

  //TRANSMISSION
  JSON_string += "{\"t\":" ;
  JSON_string += String(transmission_number++);

  //STATE
  JSON_string += ",\"s\":";
  JSON_string += String((int)machine_state);
  JSON_string += ",\"l\":";
  JSON_string += String((int)led_intensity);


  if (machine_state == 0)
  {
    //SENSORS
    JSON_string += ",\"sensor\":[";
    for (int i = 0; i < sensor_count; i++)
    {
      JSON_string += values[i];
      if (i < (sensor_count - 1))
      {
        JSON_string += ",";
      }
    }
    JSON_string += "]";
  }


  //ENDING
  JSON_string += "}";
}
