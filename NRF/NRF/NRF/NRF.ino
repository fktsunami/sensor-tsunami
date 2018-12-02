#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;


RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
	Serial.begin(115200);

	Serial.println("Initialize MPU6050");

	while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
	{
		Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
		delay(500);
	}

	// If you want, you can set accelerometer offsets
	// mpu.setAccelOffsetX();
	// mpu.setAccelOffsetY();
	// mpu.setAccelOffsetZ();

	checkSettings();


	radio.begin();
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_MIN);
	radio.stopListening();
}
void loop() {

	Vector rawAccel = mpu.readRawAccel();
	Vector normAccel = mpu.readNormalizeAccel();

	char data_buffer[120];
	char data2[120];
	char data3[120];
	Serial.print(" Xraw = ");
	Serial.print(rawAccel.XAxis);
	Serial.print(" Yraw = ");
	Serial.print(rawAccel.YAxis);
	Serial.print(" Zraw = ");

	Serial.println(rawAccel.ZAxis);
	Serial.print(" Xnorm = ");
	Serial.print(normAccel.XAxis);
	Serial.print(" Ynorm = ");
	Serial.print(normAccel.YAxis);
	Serial.print(" Znorm = ");
	Serial.println(normAccel.ZAxis);
	char lat[] = "16.062180";
	char lng[] = "108.247865";
	char ID[] = "@ID0XFF";
	char force[] = "100";
	String x = String(normAccel.XAxis,4);
	String y = String(normAccel.YAxis,4);
	String z = String(normAccel.ZAxis,4);
	sprintf(data3, "_%s_%s_%s$", x.c_str(), y.c_str(), z.c_str());

	//char x[8], y[8], z[8];
	//dtostrf(normAccel.XAxis, 6, 2, x);
	//dtostrf(normAccel.YAxis, 6, 2, y);
	//dtostrf(normAccel.ZAxis, 6, 2, z);

	sprintf(data_buffer, "%s_%s", ID, lat); 
	sprintf(data2, "_%s_%s", lng, force);

	
	//char H [] = "===============";
	radio.write(&data_buffer, sizeof(data_buffer));
	radio.write(&data2, sizeof(data2));
	radio.write(&data3, sizeof(data3));
	
	delay(1000);
}