//memasukkan library esp8266 serta firebase
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Firebase.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

//mendefinisikan host dan database(rahasia database) firebase kedalam sistem
#define FIREBASE_HOST "kelembaban-aya.firebaseio.com"
#define FIREBASE_AUTH "vqV7NSXgWQKuse71EMHAW0TLfpl4bgZ6WtEIfXW7"

//pemberian nilai konstanta hotspot wifi dan gpio pin
const char* ssid =  "warkop etan stain";
const char* password = "yahyasn06";
const int redPin = 5; //  ~D1
const int greenPin = 4; // ~D2
const int bluePin = 2; // ~D4
int WiFiStrength = 0; //kekuatan sinyal

//merubah nilai sensor menjadi persen
unsigned int getAnalog(){
  const int sampling = 100;
  unsigned int analog = 0;
  for(int i = 0; i < sampling; i++){
    int nilaiSensor = analogRead(A0); //sebagai inputan dari sensor
    analog += nilaiSensor;
  }
  analog = analog/sampling;
  return analog;
}
uint8_t getSoil(){
  const int val_max = 900;
  const int val_min = 500;
  const int sampling = 100;
  unsigned long analog = 0;
  for(int i = 0; i < sampling; i++){
    int nilaiSensor = analogRead(A0); //sebagai inputan dari sensor
    analog += nilaiSensor;
  }
  analog = analog/sampling;
  if(analog > val_max){
    analog = val_max;
  } else if(analog < val_min) {
    analog = val_min;
  }
  uint8_t persen = map(analog,val_max,val_min,0,100);
  return persen;
}

//pengaturan nilai awal pada nodemcu
void setup() {
  Serial.begin(9600);
  //penentuan pin keluaran pada gpio 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  digitalWrite(redPin, 0);
  digitalWrite(greenPin, 0);
  digitalWrite(bluePin, 0);

  //mengkoneksikan ke hotspot wifi dan menampilkan pada serial monitor
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);\
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("wifi connected");
  Serial.print("Use IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  //penentuan nilai awal pada Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("indikator", 0);
  Firebase.setFloat("Kalibrasi", 0);
  Firebase.setFloat("persenKelembapan", 0);
  Firebase.setFloat("sinyal", 0);
}


//perulangan untuk sistem di nodemcu
void loop() {
  WiFiStrength = WiFi.RSSI(); //mengambil data kekuatan sinyal
  //kondisi untuk sensor
  if (getSoil() <= 20) //jika nilai kurang dari 20 nyala merah
  {
    digitalWrite(redPin, 1);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 0);
    Serial.println("Kering");

    //mengirim data ke firebase pada child
    Firebase.setInt("indikator", 1);
    
  }
  else if (getSoil() > 20 && getSoil() <= 75) //jika nilai 26-65 nyala hijau
  {
    digitalWrite(greenPin, 1);
    digitalWrite(redPin, 0);
    digitalWrite(bluePin, 0);
    Serial.println("Setengah kering");

    //mengirim data ke firebase pada child
    Firebase.setInt("indikator", 2);

  }
  else if (getSoil() > 75) //jika nilai lebih dari 65 nyala biru
  {
    Serial.println("Basah");
    digitalWrite(bluePin, 1);
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 0);

    //mengirim data ke firebase pada child
    Firebase.setInt("indikator", 3);
  }

  //mengirim data ke firebase pada child
  Firebase.setFloat("Kalibrasi", getAnalog());
  Firebase.setFloat("persenKelembapan", getSoil() );
  Firebase.setFloat("sinyal", WiFiStrength);

  delay(500);
  
  //monitoring pada serial data
  Serial.print("WiFi Strength: ");
  Serial.print(WiFiStrength); Serial.println("dBm");
  Serial.print("Kalib: ");
  Serial.print(getAnalog());
  Serial.print(" - ");
  Serial.print("Persen: ");
  Serial.print(getSoil());
  Serial.println('%');
}
