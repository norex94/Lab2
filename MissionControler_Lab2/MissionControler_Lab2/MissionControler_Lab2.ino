#include <SPI.h>
#include <RH_RF95.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std; 

#define RFM95_CS  5   // "B"
#define RFM95_RST 6   // "A"
#define RFM95_INT  10   // "D"


const int MyID = 35;
const int FriendID = 8;


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
	rf95.send((uint8_t *)radiopacket.c_str(), 20);
	//Serial.println("Waiting for packet to complete...");
	delay(10);
	rf95.waitPacketSent(500);

}


String getMail() {

	// Now wait for a reply
	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);

	//Serial.println("Waiting for reply...");
	if (rf95.waitAvailableTimeout(1000))
	{
		// Should be a reply message for us now   
		if (rf95.recv(buf, &len))
		{
			return (char*)buf;
		}
		else
		{
			Serial.println("Receive failed");
			return("error");
		}
	}
	else
	{
		Serial.println("ERROR");
		return "error";
	}



}

String MessageSplit(String mail) {

	vector<int> values; 

	for (unsigned int i; i < 30; i++) {

		

		
		

	}






}




int tmp = 0;
int thrust;
bool Engine = 0;
bool Parach = 0;

int cheackSum() {return tmp + thrust + Engine + Parach; }
String RecivedMail;

void loop()
{
	thrust = rand() % 100;
	Engine = rand() % 2; 
	Parach = rand() % 2;

	//tmp thurst Parach Cheaksum

	String MyMail = "#"+(String)tmp + "," + (String)thrust + "," + (String)Parach + "," + (String)cheackSum()+";";
	tmp++;



	Serial.println("--------------------------");
	sendMail(MyMail); // Sendi mitt skilaboð

	RecivedMail = getMail(); // Sæki skilaboð




	if (rf95.headerId() == FriendID) { // Chekka hvort þetta sé minn póstur
		Serial.print("Got mail from FRIEND: ");
		Serial.println(RecivedMail);


	}
	else { //Ekki minn póstur, prenta samt út
		Serial.print("Got random Mail from ID: ");
		Serial.print(rf95.headerId());
		Serial.print("; Message: ");
		Serial.println(RecivedMail);

	}

	Serial.print("RSSI: ");//Samband á milli síðasta sendingar
	Serial.println(rf95.lastRssi(), DEC);

	delay(500);
	
	MyMail = "";

}
