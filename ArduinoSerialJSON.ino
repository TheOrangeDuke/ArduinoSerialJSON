//touch

const int sensors[] = {A0, A1, A2, A3, A4, A5};
int values[] = {0, 0, 0, 0, 0, 0};

setup(){
  Serial.begin(9600);
}

loop(){
  Serial.write("Hello World!");
  delay(2000);
}
