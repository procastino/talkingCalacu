#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void printDetail(uint8_t type, int value);

//cabaza mortífera
const int rxPin = 1;
const int txPin = 0;
const int busyPin = 9;
const int pinTrig = 7;
const int pinEcho = 8;
const int pinLed = 12;
int estado;
long cronoSaudo;
long cronoDistancia;
long tempoSaudo = 30000;
int randNumber;
long tempo; //tempo que tarda en chegar o echo
int dist; //distancia en centimetros



void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);
  pinMode(pinTrig, OUTPUT); //pin como salida
  pinMode(pinEcho, INPUT);  //pin como entrada
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinTrig, LOW);//Inicializamos el pin con 0
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(10);  //Set volume value. From 0 to 30
  randomSeed(analogRead(0));
  cronoSaudo =  millis();
  cronoDistancia = millis();


}

void loop() {
  // Clears the trigPin condition
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  
  if ((millis() - cronoDistancia)>100){
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);          //este pulso fai que o modulo lance unha rafaga de 8 ciclos de 40 Khz
  digitalWrite(pinTrig, LOW);
  tempo = pulseIn(pinEcho, HIGH); // este da o tempo en milis que tarda en chegar de volta esa rafaga
  dist = tempo*0.034/2;             //escalamos el tiempo a una distancia en cm
  cronoDistancia = millis();
  }

  if (dist > 50 || dist == 0) {
    if ((millis() - cronoSaudo) > tempoSaudo) {
      Serial.println("Reproducindo pista 1");
      myDFPlayer.play(1);;
      activaLeds();
      cronoSaudo = millis();
    }
    else {
      //for stability
      delay(500);
    }
    
    }
  else {
    cronoSaudo = millis();
    randNumber = random(2,6);
    Serial.print("Reproducindo pista ");
    Serial.println(randNumber);
    myDFPlayer.play(randNumber);
    activaLeds();    
  }
  
  Serial.print("Distancia: ");
  Serial.print(dist);      //Enviamos serialmente el valor de la distancia
  Serial.println("cm");
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  

}


void printDetail(uint8_t type, int value){

  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
void activaLeds(){
  int randomTempo= 1000;
  estado = 1;
  for (int i=0; i<26 ; i++){
  randomTempo = random(50, 400);
  if (estado == 1){
    digitalWrite(pinLed, HIGH );
    delay(randomTempo);
  }
  else {
    digitalWrite(pinLed, LOW );
    delay(randomTempo);
  }
  estado = -estado;
  
  }
  
}
//
