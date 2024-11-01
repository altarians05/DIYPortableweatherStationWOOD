/* |-----------------------------------------------------|
 * |--------- Dispositivo de Monitoreo Ambiental --------|
 * |---------- Environmental Monitoring Device ----------|
 * |-----------------------------------------------------|
 * |------ Codificado por: Sergio Pinilla Valencia ------|
 * |---------- Fecha de Creacion: 16 julio 2024 ---------|
 * |--------- Ultima Modificacion: 16 Julio 2024 --------|
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
 * | - Variables a medir: Temperatura, Humedad, Presión, |
 * |   Calidad del aire.                                 |
 * |                                                     |
 * | - Sensor BME680 de la marca Bosch: Integra todos    |
 * |   los sensores necesarios en un solo encapsulado.   |
 * |                                                     |
 * | - Almacenamiento de datos: Tarjeta microSD como     |
 * |   dispositivo de almacenamiento principal.          |
 * |                                                     |
 * | - Registro de tiempo real: Módulo RTC DS3231 que    |
 * |   permite registrar la hora de cada medición.       |
 * |                                                     |
 * | - Visualización de datos: Pantalla OLED de 0.96''   |
 * |   para mostrar los datos al usuario final.          |
 * |                                                     |
 * | - Autonomía energética: Sistema de recolección de   |
 * |   energía mediante un panel solar.                  |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------- Conexiones ----------------------|
 * |-----------------------------------------------------|
 * | Sensor BMP280:                                      |
 * |    SCL -> Pin A05                                   |      
 * |    SDA -> Pin A04                                   | 
 * |    Vin -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | Modulo MicroSD:                                     |
 * |    CS   -> Pin D10                                  |      
 * |    MOSI -> Pin D11                                  |      
 * |    SCK  -> Pin D13                                  |      
 * |    MISO -> Pin D12                                  |      
 * |    +5v -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | Modulo RTC DS3231:                                  |
 * |    SCL -> Pin A05                                   |      
 * |    SDA -> Pin A04                                   | 
 * |    Vin -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |                                                     |
 * | Pantalla OLED 0.96'' 128x64:                        |
 * |    SCL -> Pin A05                                   |      
 * |    SDA -> Pin A04                                   | 
 * |    Vin -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------ Bibliografia ---------------------|
 * |-----------------------------------------------------|
 * | Sensor BMP280:                                      |
 * | - Libreria: https://n9.cl/2wgdx                     |
 * | - Guia de Funcionamiento: https://n9.cl/31472       |
 * | - Tutorial Simple: https://n9.cl/s88k9              |
 * |                                                     |
 * | Modulo MicroSD:                                     |
 * | - Libreria: https://n9.cl/z0hjuf                    |
 * | - Guia de Funcionamiento: https://n9.cl/nbez7       |
 * | - Tutorial Simple: https://n9.cl/s88k9              |
 * |                                                     |
 * | Modulo RTC DS3231:                                  |
 * | - Libreria: https://github.com/adafruit/RTClib      |
 * | - Guia de Funcionamiento: https://n9.cl/o9x3j       |
 * | - Tutorial Simple: https://n9.cl/v09a0              |
 * |                                                     |
 * | Pantalla OLED 0.96'' 128x64:                        |
 * | - Libraria: www.n9.cl/xlc9w - www.n9.cl/xv1dp       |
 * | - Guia de Funcionamiento: https://n9.cl/l03ufr      |
 * | - Tutorial: www.n9.cl/jzy5xj - www.n9.cl/f29yyf     |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/


/* |-----------------------------------------------------|
 * |------------- Librerias y Constantes ----------------|
 * |-----------------------------------------------------| */
#include <Wire.h> // Libreria necesaria para la comunicacion I2C entre los disferentes dispositivos y el arduino.
#include <SPI.h> // Libreria necesaria para la comunicacion SPI entre el modulo Micro SD y el arduino.
#include <Adafruit_Sensor.h> // Libreria requerida para la programacion de todos los sensores de marca Adafruit.
#include <Adafruit_BMP280.h> // Libreria que contiene las instrucciones especificas para el sensor BMP280.
#include <SD.h> // Libreria necesaria para la comunicacion SPI entre el modulo SD y el arduino.
#include <RTClib.h> // Libreria que incluye las instrucciones necesarias para trabajar con el modulo RTC DS3231.
#include <Adafruit_GFX.h> // Libreria que incluye las instrucciones necesarias para trabajar con pantallas Genericas que requieren de graficos a traves de pixeles. 
#include <Adafruit_SSD1306.h> // Libreria para las pantallas OLED basadas en el Driver SSD1306. Permite comunicacion I2C o SPI.

#define SEALEVELPRESSURE_HPA 1013.25 // Constante utilizada para determinar la presion a nivel del mar (Cartagena) que es 760 mmHg = 1013.25 hPa.   
#define SSPIN 10 // Pin de Arduino donde va conectado el pin CS del modulo SD.
#define ANCHO 128 // Constante que contiene el tamaño Horizontal en pixeles de la pantalla OLED.
#define ALTO 64 // Constante que contiene el tamaño Vertical en pixeles de la pantalla OLED.
#define OLED_RESET 4 // Pin de Reset de la pantalla OLED conectada al Arduino, aunque no se utiliza con pantallas genericas, es necesario declararlo para que se pueda crear el objeto.


/* |-----------------------------------------------------|
 * |-------- Declaracion de Variables y Objetos ---------|
 * |-----------------------------------------------------| */
Adafruit_BMP280 BMP; // Crear objeto BMP para usar los metodos y clases del sensor respectivo.
File datos; // Crear objeto Archivo que permite manipular los datos a enviar o leer a la SD.
RTC_DS3231 rtc; // Crear Objeto RTC para utilizar los metodos y las clases de la libreria.
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET); // Crear Objeto OLED-SSD1306 segun algunos parametros como el tamaño de pantalla, tipo de comunicacion, y Pin de Reset.

float temp, pres, alti, humi; // variables para almacenar los valores medidos por el sensor.
const char filename[] = "datalog.csv"; // variable que contiene el nombre del archivo que se guardara en la microSD.
DateTime ahora; // Variable para almacenar la informacion del modulo RTC.


/* |-----------------------------------------------------|
 * |---------------- Zona Configuracion -----------------|
 * |-----------------------------------------------------| */
 void setup() {
  Serial.begin(9600); // Inicializar Comunicacion Serial.
  Wire.begin(); // Inicializar comunicacion I2C.
  while (!Serial) { // Esperar hasta que se Abra el Canal Serial.
  }

  Serial.println("¡Canal Serial Abierto!");
  Serial.println("");

  // Bucle de 10 intentos para comprobar comunicacion con el sensor BMP280:
  Serial.print("Iniciando Sensor BMP280...");
  
  for(int i=0; i<=10; i++){ 
    if(!BMP.begin()){
      Serial.print(".");
      delay(1000);
    } else{
      Serial.print(" ¡Sensor BMP280 Encontrado!\n");
      break;
    }

    if(i==10){
      Serial.println(" Error al inicializar el Sensor BME280.");
      Serial.println("1. Revisa la Conexion.");
      Serial.println("2. Reinicia el arduino.");
      Serial.println("3. vuelve a intentarlo.");
      while(1){};
    }
  }

  // Bucle de 10 intentos para comprobar comunicacion con el modulo microSD:
  Serial.print("Iniciando SD...");
  for(int i=0; i<=10; i++){
    if(!SD.begin(SSPIN)){
      Serial.print(".");
      delay(1000);
    } else{
      Serial.println(" ¡Modulo SD Encontrado!\n");
      break;
    }

    if(i==10){
      Serial.println(" |Error al inicializar el modulo SD.");
      Serial.println("1. Revisa la Conexion.");
      Serial.println("2. Reinicia el arduino.");
      Serial.println("3. vuelve a intentarlo.");
      while(1){};
    }
  }

  // Bucle de 10 intentos para comprobar comunicacion con el modulo RTC DS3231:
  Serial.print("Iniciando Modulo RTC...");
  for(int i=0; i<=10; i++){
    if(!rtc.begin()){
      Serial.print(".");
      delay(1000);
    } else{
      Serial.println(" ¡Modulo RTC Encontrado!\n");
      break;
    }

    if(i==10){
      Serial.println("\nError al inicializar el modulo RTC.");
      Serial.println("1. Revisa la Conexion.");
      Serial.println("2. Reinicia el arduino.");
      Serial.println("3. vuelve a intentarlo.");
      while(1){};
    }
  }

  // Bucle de 10 intentos para comprobar comunicacion con la pantalla OLED:
  Serial.print("Iniciando pantalla OLED...");
  for(int i=0; i<=10; i++){
    if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
      Serial.print(".");
      delay(1000);
    } else{
      oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializar comunicacion con la pantalla con algunos parametros como la alimentacion base, y la direccion I2C del dispositivo.
      Serial.println(" ¡Pantalla OLED Encontrada!\n");
      break;
    }

    if(i==10){
      Serial.println("\nError al inicializar la pantalla OLED.");
      Serial.println("1. Revisa la Conexion.");
      Serial.println("2. Reinicia el arduino.");
      Serial.println("3. vuelve a intentarlo.");
      while(1){};
    }
  } 

  oled.display(); // mostrar una pantalla de inicio por defecto de adafruit.

  // Conjunto de instrucciones que se encarga de revisar si ya existe un archivo dentro de la microSD para almacenar los datos:
  if(!SD.exists(filename)){
    datos = SD.open(filename, FILE_WRITE);

    if(datos){
      Serial.println("archivo Nuevo Creado, Escribiendo encabezados (Fila 1)...");
      datos.println("Fecha, Hora, Temperatura(°C),Presion(hPa),Altitud(m)");
      datos.close();
    } else{
      Serial.println("Error al crear el nuevo archivo");
    }
  }

  // Directivas que permiten cargar en el modulo RTC la hora y la fecha actual.
  rtc.adjust(DateTime(__DATE__, __TIME__)); // comentar despues de sincronizar la hora por primera vez, ya despues el mismo modulo se actualiza con su bateria de reserva.

  delay(2000);
  
  oled.clearDisplay(); // Limpar pantalla despues de dos (2) segundos.
}


/* |-----------------------------------------------------|
 * |------------------ Codigo Principal -----------------|
 * |-----------------------------------------------------| */
void loop(){
  datos = SD.open(filename, FILE_WRITE); // Instruccion para abrir el archivo donde se almacenara la informacion recolectada.
  ahora = rtc.now(); // variable tipo fecha para almacenar los datos del modulo en tiempo REAL.

  // Registrar las Mediciones ddesde el sensor BMP:
  temp = BMP.readTemperature(); // Funcion para leer la temperatura. 
  pres = BMP.readPressure()/100; // Funcion para leer la Presion. Se divide en 100 porque el sensor lee en Pascales, y se necesitan hPa.
  alti = BMP.readAltitude(SEALEVELPRESSURE_HPA); // se pasa como parametro la presion atmosferica a nivel del mar en colombia.   

  // Imprimir datos sensados en el monitor Serial:
  printDate(ahora, 1); // funcion que imprime en el monitor serial los datos de la fecha en tiempor Real.
  Serial.print("        Temperatura: ");
  Serial.print(temp);
  Serial.print(" °C       Presion: ");
  Serial.print(pres);
  Serial.print(" hPa      Altitud: ");
  Serial.print(alti);
  Serial.println("m"); // Metros

  // Almacenar los datos en la memoria MicroSD:
  if(datos){
    printDate(ahora, 0); // funcion que imprime en el modulo microSD los datos de la fecha en tiempor Real.
    datos.print(",");
    datos.print(temp);
    datos.print(",");
    datos.print(pres);
    datos.print(",");
    datos.println(alti);

    datos.close(); // Cerrar el archivo.
  } else{
    Serial.println("Error al abrir el archivo");
  }

  delay(1000);
}


/* |-----------------------------------------------------|
 * |----------------- Funciones Propias -----------------|
 * |-----------------------------------------------------| */
void printDate(DateTime fecha, bool type){ // Funcion para Imprimir y organizar los datos de la fecha y hora respectivamente. 
  if(type){
    Serial.print(fecha.day(), DEC);
    Serial.print("/");
    Serial.print(fecha.month(), DEC);
    Serial.print("/");
    Serial.print(fecha.year(), DEC);
    Serial.print(" - ");
    Serial.print(fecha.hour(), DEC);
    Serial.print(":");
    Serial.print(fecha.minute(), DEC);
    Serial.print(":");
    Serial.print(fecha.second(), DEC);
  } else{
    datos.print(fecha.day(), DEC);
    datos.print("/");
    datos.print(fecha.month(), DEC);
    datos.print("/");
    datos.print(fecha.year(), DEC);
    datos.print(",");
    datos.print(fecha.hour(), DEC);
    datos.print(":");
    datos.print(fecha.minute(), DEC);
    datos.print(":");
    datos.print(fecha.second(), DEC);
  }
}


void Text(){ // Funcion para mostrar las medicioes del sensor y del RTC en la pantalla OLED.
  oled.clearDisplay(); // Limpiar Pantalla OLED.
  oled.setTextColor(WHITE); // Establecer el color de los pixeles a encender en color BLANCO.
  oled.setCursor(0, 0); // Mover el cursor a la posicion X0, Y0, para iniciar la escritura en pantalla.
  oled.setTextSize(1); // Establecer el tamaño de la letra, donde 1 = 6x8 px.
  oled.print("hola, Han Pasado:"); // Escribir texto de prueba segun parametros previos de configuracion.
  oled.setCursor(10, 10); // mover el cursor a una nueva posicion (X10, Y30), con la intencion que el siguiente texto a imprimir quede centrado.
  oled.setTextSize(2); // Cambiar el tamaño de la letra, donde 2 = 12x16 px.
  oled.print(millis()/1000); // Imprimir la cantidad de seg. que pasan desde que el arduino esta en funcionamiento. 
  oled.print("seg."); // Imprimir la magnitud de medicion.
  oled.drawRect(20, 30, 88, 54, WHITE); // Crear un rectangulo desde la posicion X20 Y40 y que a partir de alli mida 88x64 px.
  oled.setCursor(28,44); 
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.print("ALARMA"); // Imprimir el mensaje "alarma" dentro del recuadro. 
  oled.display(); // mostrar en pantalla lo que se acabo de configurar, ya que todos los comandos son temporales, no definitivos. 
} 
