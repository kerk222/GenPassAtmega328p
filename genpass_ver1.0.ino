#include <MD5.h>
#include <LiquidCrystal_I2C.h>
#define buttons 11
LiquidCrystal_I2C lcd(0x27, 16, 2);
char password[8]; // тут будет лежать рандомный пароль 
int index = 0; //  работает как указатель для считывания пароля 
int starts = 0;
void setup() {
  Serial.begin(9600);
  //while (!Serial);
  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку
  pinMode (buttons, INPUT_PULLUP); // 
}
void loop() {
  // Чтение аналоговых значений
  int analogValue0 = analogRead(A0);
  int analogValue1 = analogRead(A1);
  int analogValue2 = analogRead(A2);
  int analogValue3 = analogRead(A3);
  String input = "A0:" + String(analogValue0) + " A1:" + String(analogValue1)+" A2:" + String(analogValue2) + " A3:" + String(analogValue3);
  // Генерируем MD5 хеш из этой строки
  unsigned char* hash = MD5::make_hash(input.c_str());
  char* md5str = MD5::make_digest(hash, 16);
  size_t input_len = strlen(md5str);
  uint8_t byte_count = input_len / 2;
  uint8_t bytes[byte_count]; // Массив для хранения байтов
  // Преобразование hex-строки в байты
  hexStringToBytes(md5str, bytes, input_len);
  // Вывод в Serial Monitor
  Serial.println("Исходная строка: " + String(input));
  Serial.println("Преобразованная последовательность:");
  // Вывод в HEX, и ASCII (если печатаемый)
  for (uint8_t i = 0; i < byte_count; i++) 
  {
    if (isPrintable(bytes[i])) {
      Serial.print("HEX: 0x"); Serial.print(bytes[i], HEX);
      if(bytes[i] != 0x20 ){// проверка на пробел (мне не нравиться пароли с пробелами не нужны они тут )
      Serial.print(" | ASCII: "); Serial.print((char)bytes[i]);
      Serial.println();
      genpassarray((char)bytes[i]);
      }
    } 
  }
  for (int i = 0; i < 8; i++) { 
  Serial.print(password[i]);  
   }
  free(hash);
  free(md5str);
  Serial.print(buttons);
  screen(); // вывод пароля
  delay(500);  // Задержка для стабильности
}
void screen(){
 // int cursor = 1;
  lcd.clear();
  lcd.print("PASSWORD: ");
  if(digitalRead(buttons) != 1  && starts == 1){ // проверка на нажатие кнопки и проверка на заполненность массива
    starts = 0;//  
    lcd.setCursor(1, 1);
    for (int i = 0; i < 8; i++){
      lcd.setCursor(i, 1);

      lcd.print(password[i]);
    }
    delay(10000);
  }
}
// Функция преобразования hex-символа в число
uint8_t hexCharToNum(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
  if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
  return 0; // Если символ не hex
}
// Функция преобразования hex-строки в массив байтов
void hexStringToBytes(char* input, uint8_t* output, size_t len) {
  for (size_t i = 0; i < len; i += 2) {
    uint8_t high = hexCharToNum(input[i]);
    uint8_t low = hexCharToNum(input[i + 1]);
    output[i / 2] = (high << 4) | low;
  }
}

void genpassarray(char bol) {
    
    password[index] = bol;
    
    index++;
    if(index == 8){
      index = 0;
      starts = 1; // вывод пароля доступен только когда полностью заполнин массив 
    }
  }