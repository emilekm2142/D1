#include <ArduinoJson.h>
#include <TFT_eSPI.h>
//Serial z bt leci po UART0, czyli tym samym do debugownia


boolean isTriggerDown = false;
boolean isConnected = false;

int magazineType = 0;
int maxAmmo[] = { 30, 50, 100 };

boolean isMagazineInserted = false;
int actualAmmo = 0;

boolean isSmartTriggerOn = false;

boolean adjustRate = false;
int rate = 0;

int kills = 0;

boolean burstMode = false;
int burstAmount = 0;

boolean batterySaver = false;

boolean boost = false;


//do not touch!!!111oneone
//variables for counting shoots
int lastZeros = 0;
bool d = false;
int count = 0;

TFT_eSPI tft = TFT_eSPI();
DynamicJsonBuffer incomingDataBuffer(512);
void setup() {
	Serial.begin(11520);

	tft.init();

	pinMode(PIN_D0, OUTPUT);
	analogWrite(PIN_D0, 150);

	tft.fillScreen(TFT_WHITE);
	tft.setRotation(1);
}
void redrawDisplay() {

}
void loop() {
	if (Serial.available() > 0) {
		JsonObject& root = incomingDataBuffer.parseObject(Serial);
		if (strcmp(root["type"].as<const char*>(), "handshake") == 0) {
			isConnected = true;
		}
		else if (strcmp(root["type"].as<const char*>(), "update") == 0) {
			JsonObject& data = root["data"];
			burstMode = data["burst"];
			burstAmount = data["burstValue"];
			adjustRate = data["rate"];
			rate = data["rateValue"];
			batterySaver = data["batterySaver"];
			isSmartTriggerOn = data["smartTrigger"];
			boost = data["boost"];
			magazineType = data["magazineType"];
		}
	}
	isMagazineInserted = digitalRead(10);
	isTriggerDown = digitalRead(5);
	if (isTriggerDown) {
		//kod na szczelanie zaleznie od smartTriggerów itd... Tymczasowo prosto xd
		analogWrite(15, 255);
		//xd
	}
	//segment na badanie ilosci wystrzalow. zamienic to na przerwania?
	{
		int actual = analogRead(4);

		if (actual == 0) {
			lastZeros++;
			if (lastZeros > 5) {
				//interrupt;}
				if (d) {
					count++;
				}
				d = false;
				lastZeros = 0;
			}

		}
		else {
			d = true;
			lastZeros = 0;
			//detection
		}
	}
	//TODO: dodać badanie baterii
}