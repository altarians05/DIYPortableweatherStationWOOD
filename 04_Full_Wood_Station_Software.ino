/* |-----------------------------------------------------|
 * |--------- Dispositivo de Monitoreo Ambiental --------|
 * |---------- Environmental Monitoring Device ----------|
 * |-----------------------------------------------------|
 * |------ Codificado por: Sergio Pinilla Valencia ------|
 * |---------- Fecha de Creacion: 01 junio 2024 ---------|
 * |--------- Ultima Modificacion: 10 agosto 2024 -------|
 * |---------- Lugar: TecnoAcademia - Manizales ---------|
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------- Descripcion ---------------------|
 * |-----------------------------------------------------|
 * | Este código implementa una estación meteorológica   |
 * | portátil y open source para el monitoreo ambiental  |
 * | en tiempo real, diseñada para estudiar los procesos |
 * | de formación en la Tecnoacademia de Manizales. Este |
 * | dispositivo permite registrar y monitorear diversas |
 * | variables ambientales, proporcionando datos         |
 * | precisos y confiables para su posterior uso en      |
 * | estudios de analisis de los procesos de aprendizaje.|
 * |                                                     |
 * | Caracteristicas Principales:                        |
 * |                                                     |
 * | - Variables a medir: Temperatura, Humedad, Calidad  |
 * |   del aire.                                         |
 * |                                                     |
 * | - Sensor DHT11: Sensa los datos de Temperatura y    |
 * |   Humedad en un solo encapsulado.                   |
 * |                                                     |
 * | - Sensor MQ135: Sensa los datos de calidad del aire |
 * |   y deteccion de gases peligrosos.                  |
 * |                                                     |
 * | - Sensor LDR: Sensa los datos de cantidad de luz    |
 * |   en el ambiente.                                   |
 * |                                                     |
 * | - LED: Permite mostrar de forma luminica algunos    |
 * |   avisos segun la deteccion de variables.           |
 * |                                                     |
 * | - Almacenamiento de datos: Tarjeta microSD como     |
 * |   dispositivo de almacenamiento principal.          |
 * |                                                     |
 * | - Registro de tiempo real: Módulo RTC DS1302 que    |
 * |   permite registrar la hora de cada medición.       |
 * |                                                     |
 * | - Visualización de datos: Pantalla LCD 16x2 con     |
 * |   iluminacion de fondo y comunicacion I2C.          |
 * |                                                     |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------- Conexiones ----------------------|
 * |-----------------------------------------------------|
 * | Sensor DHT11:                                       |
 * |    Out -> Pin D02 (NANO)                            |
 * |    Vin -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | Sensor MQ135:                                       |
 * |    A0  -> Pin A01 (NANO)                            |
 * |    Vin -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | Sensor LDR (Fotorresistencia):                      |
 * |    Out -> Pin A02 (NANO)                            |
 * |    Vin -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | LED:                                                |
 * |    Vin -> D04 (NANO)                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | LCD 16x2 I2C:                                       |
 * |    SDA  -> Pin A04 (NANO)                           |
 * |    SCL  -> Pin A05 (NANO)                           |
 * |    Vin  -> 5v Arduino                               |
 * |    GND  -> Ground Arduino                           |
 * |                                                     |
 * | Modulo RTC TINY DS1307:                             |
 * |    SCL -> Pin A05                                   |      
 * |    SDA -> Pin A04                                   |      
 * |    +5v -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | Modulo MicroSD:                                     |
 * |    CS   -> Pin D10                                  |      
 * |    MOSI -> Pin D11                                  |      
 * |    SCK  -> Pin D13                                  |      
 * |    MISO -> Pin D12                                  |      
 * |    +5v -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------ Bibliografia ---------------------|
 * |-----------------------------------------------------|
 * | Sensor DHT11:                                       |
 * | - Libraria: n9.cl/y3my5s                            |
 * | - Guia de Funcionamiento: n9.cl/2njn                |
 * | - Tutorial: n9.cl/o05e1                             |
 * |                                                     |
 * | Sensor MQ135:                                       |
 * | - Libreria: acortar.link/z7te3A                     |
 * | - Guias: n9.cl/p2cqr                                |
 * | - Tutorial: n9.cl/zewtn                             |
 * |                                                     |
 * | Sensor LDR (Fotorresistencia):                      |
 * | - Libreria: No es necesario.                        |
 * | - Guias: n9.cl/5e9dl                                |
 * | - Tutorial: n9.cl/w15fz                             |
 * |                                                     |
 * | Pantalla LCD 16x2 I2C:                              |
 * | - Libreria: n9.cl/4ulczq                            |
 * | - Guias: n9.cl/mczco                                |
 * | - Tutorial: n9.cl/exs2ti                            |
 * |                                                     |
 * | Modulo RTC DS1307:                                  |
 * | - Libreria: n9.cl/y3air                             |
 * | - Guias: n9.cl/xj3kw                                |
 * | - Tutorial: n9.cl/nxks8                             |
 * |                                                     |
 * | Modulo MicroSD:                                     |
 * | - Libreria: n9.cl/z0hjuf                            |
 * | - Guia de Funcionamiento: n9.cl/nbez7               |
 * | - Tutorial Simple: n9.cl/s88k9                      |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------- Librerias y Constantes ----------------|
 * |-----------------------------------------------------| */
#include <LCD_I2C.h> // Libreria para pantalla LCD con modulo I2C
#include <SPI.h>
#include <SD.h> // Libreria necesaria para la comunicacion SPI entre el modulo SD y el arduino.
#include "DHT.h" // Libreria para Sensor DHT11
#include <Wire.h>
#include "RTClib.h" // Libreria que incluye las instrucciones necesarias para trabajar con el modulo RTC DS3231. 
  
#define DHT_PIN 2 // Pin de conexion del sensor DHT11 al arduino
#define DHTTYPE DHT22 // Declarar en caso de utilizar DHT 11
#define MQ_PIN A1 // Pin de conexion del sensor de gas
#define LDR_PIN A2 // Pin de conexion del sensor de Luz
#define LED_PIN 4 // Pin de conexion del LED
#define SSPIN 10 // Pin de Arduino donde va conectado el pin CS del modulo SD.


/* |-----------------------------------------------------|
 * |-------- Declaracion de Variables y Objetos ---------|
 * |-----------------------------------------------------| */
LCD_I2C lcd(0x27, 16, 2); // Crear objeto para la pantalla LCD
DHT dht(DHT_PIN, DHTTYPE); // Crear objeto para el sensor DHT
RTC_DS1307 rtc; // Crear Objeto RTC para utilizar los metodos y las clases de la libreria.

File datos; // Crear objeto Archivo que permite manipular los datos a enviar o leer a la SD.
DateTime ahora; // Variable para almacenar la informacion del modulo RTC.
// char buffer[40]; // arreglo o espacio de memoria donde se alamcena los String a imprimir con sprintf.

float hValue, tValue;
int gasValue;
int ldrValue, ldrRawData, ldrMappedValue, ldrMIN, ldrMAX;
char filename[] = "datalog.csv"; // Nombre de archivo donde se almacenan los datos.


/* |-----------------------------------------------------|
 * |---------------- Zona Configuracion -----------------|
 * |-----------------------------------------------------| */
void setup(){
  Serial.begin(9600); // Inicializar Canal Serial
  lcd.begin(); // Inicializar Pantalla LCD
  dht.begin(); // Inicializar Sensor DHT
  rtc.begin(); //Inicializamos el RTC

  while (!Serial) { // Esperar hasta que se Abra el Canal Serial.
  }

  Serial.println("¡Canal Serial Abierto!");
  Serial.println("");

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Actualizar la hora y la fecha con la del PC. Comentar despues de sincronizar por primera vez.
  
  // Funcion que se encarga de inicializar y configurar el modulo microSD:
  Serial.print("Iniciando SD...");
  
  for(int i=0; i<=10; i++){
    if(!SD.begin(SSPIN)){
      Serial.print(".");
      delay(1000);
    } else{
      Serial.println(" ¡Modulo SD Encontrado!\n");
      break;
    }

//    if(i==10){
//      Serial.println(" |Error al inicializar el modulo SD.");
//      Serial.println("1. Revisa la Conexion.");
//      Serial.println("2. Reinicia el arduino.");
//      Serial.println("3. vuelve a intentarlo.");
//      while(1){};
//    }
  }

  // Conjunto de instrucciones que se encarga de revisar si ya existe un archivo dentro de la microSD para almacenar los datos:
  if(!SD.exists(filename)){ 
    datos = SD.open(filename, FILE_WRITE);

    if(datos){
      Serial.println("archivo Nuevo Creado, Escribiendo encabezados (Fila 1)...");
      datos.println("Dia,Mes,año,Hora,Minutos,Segundos,Humedad(%),Temperatura(°C),Gas(PPM)");
      datos.close();
    } else{
      Serial.println("Error al crear el nuevo archivo");
    }
  }

  // Declarar entradas y salidas de tipo digital:
  pinMode(DHT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  lcd.backlight(); // Encender Brillo de Pantalla
  lcd.setCursor(3, 0); // establecer cursor
  lcd.print("Estacion"); // imprimir en pantalla
  lcd.setCursor(3, 1);
  lcd.print("Monitoreo");

  // Esperar 3 seg. para limpiar la pantalla
  delay(3000);
  lcd.clear();

  // Tomar lecturas iniciales de Sensor LDR para calibracion: 
  ldrMIN = analogRead(LDR_PIN);
  ldrMAX = analogRead(LDR_PIN);
}


/* |-----------------------------------------------------|
 * |------------------ Codigo Principal -----------------|
 * |-----------------------------------------------------| */
void loop(){
  hValue = dht.readHumidity(); // Leer Humedad
  tValue= dht.readTemperature(); // Leer Temperatura
  gasValue = analogRead(MQ_PIN); // leer sensor de Gas
  ldrValue = ldrReading(); // leer, calibrar y convertir datos del sensor de luz

  ahora = rtc.now(); // variable tipo fecha para almacenar los datos del modulo en tiempo REAL.

  // Comprobamos si ha habido algún error en la lectura
  if (isnan(hValue) || isnan(tValue)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    lcd.setCursor(2, 0);
    lcd.print("Error DHT11");
    return;
  }

  datos = SD.open(filename, FILE_WRITE); // Instruccion para abrir el archivo donde se almacenara la informacion recolectada.
  
  if(datos){ // si el archivo datos existe, entonces:
    datos.print(ahora.day());
    datos.print(",");
    datos.print(ahora.month());
    datos.print(",");
    datos.print(ahora.year());
    datos.print(",");
    datos.print(ahora.hour());
    datos.print(",");
    datos.print(ahora.minute());
    datos.print(",");
    datos.print(ahora.second());
    datos.print(",");
    datos.print(hValue);
    datos.print(",");
    datos.print(tValue);
    datos.print(",");
    datos.println(gasValue);
    datos.close(); // Cerrar el archivo.
  } else{ // si el archivo no existe o se pierde comunicacion, entonces:
    Serial.println("Error al abrir el archivo");
  }
  
  debugging();
}


/* |-----------------------------------------------------|
 * |----------------- Funciones Propias -----------------|
 * |-----------------------------------------------------| */
int ldrReading(){ // funcion para leer, calibrar, y convertir lectura de sensor LDR:
  ldrRawData = analogRead(LDR_PIN); // leer Sensor de Luz
  ldrMappedValue = map(ldrRawData, ldrMIN, ldrMAX, 100, 0);
  
  if(ldrRawData > ldrMAX){
    ldrMAX = ldrRawData;
  }

  if(ldrRawData < ldrMIN){
    ldrMIN = ldrRawData;
  }

  return ldrMappedValue;
}


void debugging(){ // Funcion para imprimir los valores de los sensores para verificar que funcionen:
  // Imprimir valores en monitor Serial: 
  // Serial.print(myRTC.dayofmonth);
  // Serial.print("/");
  // Serial.print(myRTC.month);
  // Serial.print("/");
  // Serial.print(myRTC.year);
  // Serial.print(" ");
  // Serial.print(myRTC.hours);
  // Serial.print(":");
  // Serial.print(myRTC.minutes);
  // Serial.print(":");
  // Serial.print(myRTC.seconds);
  // Serial.print("       Humidity: ");
  // Serial.print(hValue);
  // Serial.print("%       Temperature: ");
  // Serial.print(tValue);
  // Serial.print("°C        Gas: ");
  // Serial.print(gasValue);
  // Serial.print("%        Ldr: ");
  // Serial.println(ldrValue);

  // Imprimir Valores en LCD:
  lcd.setCursor(0, 0);
  lcd.print("Hum:");
  lcd.print(int(hValue));
  
  lcd.setCursor(9, 0);
  lcd.print("Tem:");
  lcd.print(int(tValue));

  lcd.setCursor(0, 1);
  lcd.print("Gas:");
  lcd.print(int(gasValue));

  lcd.setCursor(9, 1);
  lcd.print("Ldr:");
  lcd.print(int(ldrValue));

  delay(1000);
  lcd.clear();
}
