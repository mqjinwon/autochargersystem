#define STARTBIT '#'
#define ENDBIT '@'


enum{
  example
}typedef data_mode;

unsigned char c;  // variable to store the received character

//data 규약 #'id''data_mode''data_mode에 따른 데이터들'@
int id; //linetracer id



void dataParsing(char* buf);

void setup() {
  Serial.begin(9600,SERIAL_8N1);   // setup serial
}
  
void loop() {

  //수신 소스(에코)
  if(Serial.available() > 0){
      c = Serial.read();   // read the character
      Serial.write(c);     // echo
    }
}

void dataParsing(char* buf){
  
}
