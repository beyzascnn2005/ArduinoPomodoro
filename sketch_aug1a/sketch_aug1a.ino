#define RGB_k 3   // Kırmızı LED
#define RGB_m 4   // mavi LED
#define RGB_y 5   // Yeşil LED
#define echo 6
#define trig 7
#define buzzer 11
#define buton 8
//gecemodu için
bool geceModu=false;
bool oncekiButond=HIGH;
//masada olma/olmama
bool masadaydi=false;
bool calismaModunda=false;
long masadaSayac=0;
long masadaDegilSayac=0;

unsigned long calismaSure = 0;
unsigned long baslangic = 0;
unsigned long ara = 0;
int tur=1;

const unsigned long calismaSuresi = 30000; // 30 saniye (örnek)
const unsigned long araSuresi = 10000;     // 10 saniye (örnek)

void setup() {
  pinMode(RGB_k, OUTPUT);
  pinMode(RGB_m, OUTPUT);
  pinMode(RGB_y, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(buton,INPUT_PULLUP);//DAHİLİ OULLUP DİRENCİ AKTİF, BUTONA BASINCA BUTON DURUMU LOW OLUR
  Serial.begin(9600);
  Serial.println("*** PROGRAM BAŞLATILIYOR ***");
  Serial.println("Süre basladi!!!!");
}

void loop() {
  //GECE MODU AC/KAPA
  GeceModu();

bool suAnkimasaD=masaDurumu();
if(suAnkimasaD){
  masadaSayac++;
  masadaDegilSayac=0;
}
else{
  masadaDegilSayac++;
  masadaSayac=0;
}
  // MASADA OLMAMA DURUMU,masadan kaktı
  if(masadaDegilSayac>=3 && masadaydi) {
   
    masadaydi = false;
    calismaModunda = false;
    digitalWrite(RGB_y, LOW);
    Serial.println("Lütfen masaya geçin!!!");
    digitalWrite(RGB_k, HIGH);
    
      if(!geceModu){
        tone(buzzer,440);
        delay(200);
        noTone(buzzer);
  }
    
    return; // masada değilse geri dön
  }

  // MASADA OLMA DURUMU,oturdu
  if (masadaSayac>=3 && !masadaydi) { // süre henüz başlamadıysa
    masadaydi = true;
    calismaModunda = true;
    Serial.println("masaya hosgeldiniz");
    digitalWrite(RGB_k, LOW);
    digitalWrite(RGB_y, HIGH);
  
      if(!geceModu){
        tone(buzzer,440);
        delay(200);
        noTone(buzzer);
  }
    baslangic = millis(); // çalışma süresi başlangıcı
   
    
  }
  
   if(calismaModunda){
 // Sürekli çalışma süresini güncelle
  calismaSure = millis() - baslangic;

  // ÇALIŞMA SÜRESİ BİTTİ Mİ?
  if (calismaSure >= calismaSuresi) {
    calismaModunda=false;
    digitalWrite(RGB_y, LOW);
    digitalWrite(RGB_m, HIGH); // Mavi = ara zamanı
       if(!geceModu){
        tone(buzzer,440);
        delay(200);
        noTone(buzzer);
  }
    Serial.print(tur);Serial.println(".turunuz sona erdi,Ara verebilirsiniz!!!");
    
    tur++;
    
    

    // Ara başlangıç zamanını ayarla
    unsigned long araBaslangic = millis();
    while (millis() - araBaslangic < araSuresi) {
      // Ara süresince LED yanık kalsın
    }
    digitalWrite(RGB_m, LOW); 
    digitalWrite(RGB_k, HIGH);
       if(!geceModu){
        tone(buzzer,440);
        delay(200);
        noTone(buzzer);
  }
    Serial.println("Aranız sona erdi!!!");
   

    // Tekrar çalışmaya başla
    baslangic = millis();
    calismaSure = 0;
    calismaModunda=true;
    Serial.println("Süre başladİ!!!");
    digitalWrite(RGB_k, LOW);
    digitalWrite(RGB_y, HIGH);

    }
  }  
}

// Mesafe ölçüm fonksiyonu
long mesafeOlc() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long sureUs = pulseIn(echo, HIGH);
  long mesafe = sureUs * 0.034 / 2;

  return mesafe;
}

// Masada olup olmadığını kontrol et
bool masaDurumu() {
  long toplam = 0;
  for (int i = 0; i < 5; i++) {  // 5 ölçüm al, ortalamasını kullan(daha doğru bir olcum yapmak adina)
    toplam += mesafeOlc();
    delay(10);
  }
  long ortalama = toplam / 5;
  return (ortalama < 60);
}

long GeceModu(){
bool butonDurumu=digitalRead(buton);
  if(butonDurumu==LOW && oncekiButond==HIGH){
    geceModu=!geceModu;
    Serial.print("Gece Modu: ");
    Serial.println(geceModu ? "Açık" : "Kapalı");

      
      delay(200);
  }
  oncekiButond=butonDurumu;
}