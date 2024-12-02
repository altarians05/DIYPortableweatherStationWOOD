/* |-----------------------------------------------------|
 * |------------- prueba de funcionamiento --------------|
 * |----------------- Modulo RTC DS1302 -----------------|
 * |-----------------------------------------------------|
 * |------ Codificado por: Sergio Pinilla Valencia ------|
 * |---------- Fecha de Creacion: 20 mayo 2024 ----------|
 * |--------- Ultima Modificacion: 21 mayo 2024 ---------|
 * |---------- Lugar: TecnoAcademia - Manizales ---------|
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------- Descripcion ---------------------|
 * |-----------------------------------------------------|
 * | El presente codigo muestra la implementacion        | 
 * | del modulo de reloj DS1302 que permite obtener la   | 
 * | fecha y la hora de forma muy precisa, con la        |
 * | intencion de registrar en tiempo real en que        |
 * | momento se toman los datos desde los sensores.      |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------- Conexiones ----------------------|
 * |-----------------------------------------------------|
 * | Modulo RTC DS1302:                                         |
 * |    CLK  -> Pin D09                                  |      
 * |    DATA -> Pin D08                                  |      
 * |    RST  -> Pin D07                                  |          
 * |    +5v -> 5v Arduino                                |
 * |    GND -> Ground Arduino                            |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------------ Bibliografia ---------------------|
 * |-----------------------------------------------------|
 * | Libreria: n9.cl/y3air                               |
 * | Guias: n9.cl/e5cul                                  |
 * | Tutorial: n9.cl/pstsdr                              |
 * |-----------------------------------------------------|
 * |-----------------------------------------------------|*/

/* |-----------------------------------------------------|
 * |------------- Librerias y Constantes ----------------|
 * |-----------------------------------------------------| */
#include "virtuabotixRTC.h"

virtuabotixRTC myRTC(9, 8, 7); // CLK -> 9 , DAT -> 8, Reset -> 7

void setup() {
 Serial.begin(9600);
 // Formato para la fecha:
 // segundos, minutos, horas, día de la semana, día del mes, mes, año
 myRTC.setDS1302Time(0, 6, 9, 3, 9, 11, 2024); //Configuración del tiempo actual 
 // Recuerda comentar la línea anterior, una vez configurado
}

void loop() {
 myRTC.updateTime();

 Serial.print("Fecha / Hora: ");
 Serial.print(myRTC.dayofmonth);
 Serial.print("/");
 Serial.print(myRTC.month);
 Serial.print("/");
 Serial.print(myRTC.year);
 Serial.print(" ");
 Serial.print(myRTC.hours);
 Serial.print(":");
 Serial.print(myRTC.minutes);
 Serial.print(":");
 Serial.println(myRTC.seconds);

 delay(1000);
}
