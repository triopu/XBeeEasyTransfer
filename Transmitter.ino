#include <SoftEasyTransfer.h>     //Memasukkan library SoftEasyTransfer untuk XBee
#include <OneWire.h>              //Memasukkan library OneWire untuk Sensor Suhu
#include <DallasTemperature.h>    //Memasukkan library Sensor Suhu                  
#include <SoftwareSerial.h>       //Memasukkan library SoftSerial sebagai pendamping library SoftEasyTransfer

#define rxPin 0                   //Mendefinisikan pin 0 sebagai RX XBee
#define txPin 1                   //Mendefinisikan pin 1 sebagai TX XBee

SoftwareSerial XBee =  SoftwareSerial(rxPin, txPin);        //Mendefinisikan RX, TX pada serial XBee, dan menamai serial dengan XBee

SoftEasyTransfer ET;                                        //Membentuk Objek

struct SEND_DATA_STRUCTURE{
  //Mendefinisikan variabel-variabel yang akan dikirim 
  //Harus sama persis dengan Arduino penerima
  int Temperature;
  int Turbidity;
};

int suhu;              //Inisialisasi variabel suhu sebagai integrer
int keruh;             //Inisialisasi variabel kekeruhan sebagai integrer

#define ONE_WIRE_BUS 9                //Kabel data sensor suhu di pin 9

OneWire oneWire(ONE_WIRE_BUS);        //Mengatur oneWire untuk berkomunikasi dengan perangkat OneWire

DallasTemperature sensors(&oneWire);  //Menyambungkan oneWire dengan Dallas Temperature

//Memberikan nama pada grup data
SEND_DATA_STRUCTURE sensor;

void setup(){
  XBee.begin(38400);            //Memulai serail XBee dengan baudrate 38400
  
  sensors.begin();              //Memulai sensor
  
  pinMode(rxPin, INPUT);        //Menset pin RX sebagai INPUT
  pinMode(txPin, OUTPUT);       //Menset pin TX sebagai output
  
  //Memulai library, mengisi ddetails dengan nama grup data, dan nama serial port
  ET.begin(details(sensor), &XBee);
}

void loop(){

  //Pembacaan Suhu
  sensors.requestTemperatures();
  int bacasuhu = sensors.getTempCByIndex(0);
  suhu = sensors.getTempCByIndex(0);
  sensor.Temperature = suhu;
  
  //Menampilkan suhu pada serial monitor
  XBee.print("Suhu: ");
  XBee.print(sensor.Temperature);
  XBee.print(" ");
  
  //Pembacaan Turbidity
  int bacakeruh = analogRead(A0);
  keruh = analogRead(A0);
  sensor.Turbidity = keruh;
  
  //Menampilkan turbidity pada serial monitor
  XBee.print("Kekeruhan: ");
  XBee.print(sensor.Turbidity);
  XBee.println(" ");
  
  ET.sendData();          //Mengirim data

  delay(100);             //Memberikan delay 0,1 s
}

