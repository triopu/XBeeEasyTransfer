#include <SoftEasyTransfer.h>     //Memasukkan library SoftEasyTransfer untuk XBee
#include <LiquidCrystal.h>        //Memasukkan libaray LCD
#include <SoftwareSerial.h>       //Memasukkan library SoftSerial sebagai pendamping library SoftEasyTransfer
#include <DS1307RTC.h>            //Memasukkan library RTC
#include <Time.h>                 //Memasukkan library Time sebagai pewaktu
#include <Wire.h>                 //Memasukkan library Wire sebagai pendamping library Time


#define rxPin 0                   //Mendefinisikan pin 0 sebagai RX XBee
#define txPin 1                   //Mendefinisikan pin 1 sebagai TX XBee

LiquidCrystal lcd(2,3,4,5,6,7);   //Inisialisasi pin LCD

SoftwareSerial XBee =  SoftwareSerial(rxPin, txPin);   //Mendefinisikan RX, TX pada serial XBee, dan menamai serial dengan XBee

SoftEasyTransfer ET;                                   //Membentuk Objek


int suhu;              //Inisialisasi variabel suhu sebagai integrer
int keruh;             //Inisialisasi variabel kekeruhan sebagai integrer

struct RECEIVE_DATA_STRUCTURE{
  //Mendefinisikan variabel-variabel yang akan diterima
  //Harus sama persis dengan Arduino pengirim
  int Temperature;
  int Turbidity;
};

//Memberikan nama pada grup data
RECEIVE_DATA_STRUCTURE sensor;

void setup(){
  XBee.begin(38400);            //Memulai serail XBee dengan baudrate 38400
  
  //Memulai library, mengisi ddetails dengan nama grup data, dan nama serial port
  ET.begin(details(sensor), &XBee);
  
  //Aktifkan untuk menset waktu
  //setTime(14,00,50,26,1,2016);  //Format setTime(H,M,S,D,M,Y);    
  //RTC.set(now());
  
  //Memulai LCD
  lcd.begin(16,2);
  
  //Tampilan awal LCD
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("FISIKA FMIPA UNS");
  lcd.setCursor(0,1);
  lcd.print("SENSOR KEKERUHAN");
  delay(3000);
  lcd.clear();
}

void loop(){ 
  if(ET.receiveData()){
    
    //Mengambil data yang telah dikirim dari XBee Transmitter
    int suhu = sensor.Temperature;
    int adckeruh = sensor.Turbidity;
    
    //Merubah nilai ADC menjadi NTU
    int keruh = (0.1618*adckeruh) - 26.043;
    
    //Menampilkan pada serial monitor
    XBee.print("T = ");
    XBee.print(suhu);
    XBee.print(" ");
    XBee.print("NTU = ");
    XBee.print(keruh);
    XBee.println(" ");
    
    //Menampilkan pada LCD
    lcd.setCursor(9,1);
    lcd.print("T: ");
    lcd.setCursor(12,1);
    lcd.print(suhu);
    lcd.setCursor(14,1);
    lcd.print((char)223);        //Menampilkan karakter drajat
    lcd.setCursor(15,1);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("NTU: ");
    lcd.setCursor(5,1);
    lcd.print(keruh);
  }
  delay(25);
  
  //Menampilkan waktu
  waktu();
}

void waktu()
{
  tmElements_t tm;
  RTC.read(tm);
  if (RTC.read(tm)) {
    lcd.setCursor(11,0);
    print2digits(tm.Hour);
    lcd.write(':');
    print2digits(tm.Minute);
    
    //Diaktifkan jika membutuhkan detik
    //lcd.write(':');
    //print2digits(tm.Second);
    
    lcd.setCursor(0,0);
    lcd.print(tm.Day);
    lcd.write('/');
    lcd.print(tm.Month);
    lcd.write('/');
    lcd.print(tmYearToCalendar(tm.Year));
    //lcd.println();
  } else {
    if (RTC.chipPresent()) {
      lcd.println("DS1307 berhenti. Aktifkan setTime");
      lcd.println("setTime harus tepat.");
      lcd.println();
    } else {
      lcd.println("DS1307 error, periksa rangkaian.");
      lcd.println();
    }
    delay(9000);
  }
  delay(1000);
  //lcd.clear();
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}
