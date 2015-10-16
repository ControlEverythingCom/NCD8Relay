/* Includes ------------------------------------------------------------------*/
#include "NCD8Relay.h"

SYSTEM_MODE(MANUAL);
NCD8Relay relayController;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Serial.begin(115200);
	relayController.setAddress(0,0,0);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
//Test relays
for(int i = 1; i < 9; i++){
	relayController.turnOnRelay(i);
	int rOnStatus = relayController.readRelayStatus(i);
	Serial.print("Relay status after on: ");
	Serial.println(rOnStatus);
	delay(500);
}
for(int i = 1; i < 9; i++){
	relayController.turnOffRelay(i);
	int rOffStatus = relayController.readRelayStatus(i);
		Serial.print("Relay status after off: ");
		Serial.println(rOffStatus);
	delay(500);
}

relayController.turnOnAllRelays();
delay(500);
relayController.turnOffAllRelays();
delay(500);

for(int i = 1; i < 9; i++){
	relayController.toggleRelay(i);
	delay(500);
	relayController.toggleRelay(i);
	delay(500);
}
relayController.setBankStatus(85);
int rStatus85 = relayController.readRelayBankStatus();
Serial.print("Status after setting relay bank status to 85: ");
Serial.println(rStatus85);
delay(500);
relayController.setBankStatus(170);
delay(500);
relayController.turnOffAllRelays();
delay(500);
}
