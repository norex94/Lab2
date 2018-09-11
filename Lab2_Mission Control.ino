// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <cstdlib>



#define RFM95_CS  5   // "B"
#define RFM95_RST 6   // "A"
#define RFM95_INT  10   // "D"


const int MyID = 10;



// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup()
{
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);

	Serial.begin(115200);
	while (!Serial) {
		delay(1);
	}

	delay(100);

	Serial.println("Feather LoRa TX Test!");

	// manual reset
	digitalWrite(RFM95_RST, LOW);
	delay(10);
	digitalWrite(RFM95_RST, HIGH);
	delay(10);

	while (!rf95.init()) {
		Serial.println("LoRa radio init failed");
		while (1);
	}
	Serial.println("LoRa radio init OK!");

	// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
	if (!rf95.setFrequency(RF95_FREQ)) {
		Serial.println("setFrequency failed");
		while (1);
	}
	Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

	rf95.setTxPower(23, false);

	rf95.setHeaderId(MyID);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission


void sendMail(String radiopacket) {
	
	
	

	


	Serial.print("Sending: "); Serial.println(radiopacket);
	radiopacket[19] = 0;


	//Serial.println("Sending...");
	delay(10);
	rf95.send((uint8_t *)radiopacket.c_str(), 10);
	//Serial.println("Waiting for packet to complete...");
	delay(10);
	rf95.waitPacketSent(500);

}


void getMail() {

	// Now wait for a reply
	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);

	//Serial.println("Waiting for reply...");
	if (rf95.waitAvailableTimeout(800))
	{
		if (rf95.headerId() == 11) {
			Serial.println("ID Match");
		}

		// Should be a reply message for us now   
		if (rf95.recv(buf, &len))
		{
			Serial.print("id:");  Serial.print(rf95.headerId());
			Serial.print(" Message: ");
			String tmp = (char*)buf;
			Serial.println(tmp);




			Serial.print("RSSI: ");
			Serial.println(rf95.lastRssi(), DEC);
		}
		else
		{
			Serial.println("Receive failed");
		}
	}
	else
	{
		Serial.println("No reply.");
	}



}

int tmp = 0;
int thrust;
bool Engine = 0;
bool Parach = 0;



void loop()
{
	thrust = rand() % 100; 
	Serial.println(thrust);
	String mail = "Arnor"+(String)thrust;
	tmp++;
	
	

	Serial.println("-------------");
	sendMail(mail);
	getMail();
	delay(500);
	//helllo dwlsfasdf

}
