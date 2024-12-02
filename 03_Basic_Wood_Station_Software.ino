/* |-----------------------------------------------------|
 * |--------- Dispositivo de Monitoreo Ambiental --------|
 * |---------- Environmental Monitoring Device ----------|
 * |-----------------------------------------------------|
 * |------ Codificado por: Sergio Pinilla Valencia ------|
 * |---------- Fecha de Creacion: 01 junio 2024 ---------|
 * |------- Ultima Modificacion: 19 noviembre 2024 ------|
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
 * | LCD 16x2 I2C:                                       |
 * |    SDA  -> Pin A04 (NANO)                           |
 * |    SCL  -> Pin A05 (NANO)                           |
 * |    Vin  -> 5v Arduino                               |
 * |    GND  -> Ground Arduino                           |
 * |                                                     |
 * | Modulo RTC DS1302:                                     |
 * |    CLK  -> Pin D09                                  |      
 * |    DATA -> Pin D08                                  |      
 * |    RST  -> Pin D07                                  |          
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
 * | Pantalla LCD 16x2 I2C:                              |
 * | - Libreria: n9.cl/4ulczq                            |
 * | - Guias: n9.cl/mczco                                |
 * | - Tutorial: n9.cl/exs2ti                            |
 * |                                                     |
 * | Modulo RTC DS1302:                                  |
 * | - Libreria: n9.cl/y3air                             |
 * | - Guias: n9.cl/e5cul                                |
 * | - Tutorial: n9.cl/pstsdr                            |
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
#include "virtuabotixRTC.h"

#define DHT_PIN 2 // Pin de conexion del sensor DHT11 al arduino
#define DHTTYPE DHT22 // Declarar en caso de utilizar DHT 11
#define MQ_PIN A1 // Pin de conexion del sensor de gas
#define SSPIN 10 // Pin de Arduino donde va conectado el pin CS del modulo SD.


/* |-----------------------------------------------------|
 * |-------- Declaracion de Variables y Objetos ---------|
 * |-----------------------------------------------------| */
LCD_I2C lcd(0x27, 16, 2); // Crear objeto para la pantalla LCD
DHT dht(DHT_PIN, DHTTYPE); // Crear objeto para el sensor DHT
File datos; // Crear objeto Archivo que permite manipular los datos a enviar o leer a la SD.
virtuabotixRTC myRTC(9, 8, 7);

float hValue, tValue;
int gasValue;
char filename[] = "datalog.csv"; // Nombre de archivo donde se almacenan los datos.


/* |-----------------------------------------------------|
 * |---------------- Zona Configuracion -----------------|
 * |-----------------------------------------------------| */
void setup(){
  Serial.begin(9600); // Inicializar Canal Serial
  lcd.begin(); // Inicializar Pantalla LCD
  dht.begin(); // Inicializar Sensor DHT

  while (!Serial) { // Esperar hasta que se Abra el Canal Serial.
  }

  Serial.println("¡Canal Serial Abierto!");
  Serial.println("");

  myRTC.setDS1302Time(0, 39, 23, 4, 17, 11, 2024); // Configuración del tiempo actual. segundos, minutos, horas, día de la semana, día del mes, mes, año 

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
  
  lcd.backlight(); // Encender Brillo de Pantalla
  lcd.setCursor(3, 0); // establecer cursor
  lcd.print("Estacion"); // imprimir en pantalla
  lcd.setCursor(3, 1);
  lcd.print("Monitoreo");

  // Esperar 3 seg. para limpiar la pantalla
  delay(3000);
  lcd.clear();

}


/* |-----------------------------------------------------|
 * |------------------ Codigo Principal -----------------|
 * |-----------------------------------------------------| */
void loop(){
  hValue = dht.readHumidity(); // Leer Humedad
  tValue= dht.readTemperature(); // Leer Temperatura
  gasValue = analogRead(MQ_PIN); // leer sensor de Gas

  myRTC.updateTime(); // Actualizar Tiempo

  // Comprobamos si ha habido algún error en la lectura
  if (isnan(hValue) || isnan(tValue)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    lcd.setCursor(2, 0);
    lcd.print("Error DHT11");
    return;
  }

  datos = SD.open(filename, FILE_WRITE); // Instruccion para abrir el archivo donde se almacenara la informacion recolectada.
  
  if(datos){ // si el archivo datos existe, entonces:
    datos.print(myRTC.dayofmonth);
    datos.print(",");
    datos.print(myRTC.month);
    datos.print(",");
    datos.print(myRTC.year);
    datos.print(",");
    datos.print(myRTC.hours);
    datos.print(",");
    datos.print(myRTC.minutes);
    datos.print(",");
    datos.print(myRTC.seconds);
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

  lcd.setCursor(4, 1);
  lcd.print("Gas:");
  lcd.print(int(gasValue));

  delay(1000);
  lcd.clear();
}
