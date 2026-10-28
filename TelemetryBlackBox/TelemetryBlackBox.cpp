#include "TelemetryBlackBox.h"
#include <iostream>
#include <bitset>

void TelemetryBlackBox::onRocketStateChange(RocketState state) {
	CurrentState = state;
}
void TelemetryBlackBox::onParachuteDeployed() {
	CurrentParachuteState = 1;
}
void TelemetryBlackBox::onBaroReading(bool error, uint16_t value) {
	CurrentBaroError = error;
	CurrentAltitude = value;
}
void TelemetryBlackBox::recordCurrentState() {
	uint32_t frame = 0;
	uint8_t checksum = 0;
	frame |= static_cast<uint32_t>(CurrentState); //zapis fazy lotu bity 0-2
	frame |= static_cast<uint32_t>(CurrentParachuteState) << 3; //zapis stanu wyzwolenia spadochronu bit 3
	frame |= static_cast<uint32_t>(CurrentBaroError) << 4; //zapis flagi błędu barometru bit 4
	if (CurrentBaroError == 1) frame |= static_cast<uint32_t>(0xFFFF) << 8;
	else if (CurrentState == DISARM || CurrentState == LANDED) frame |= static_cast<uint32_t>(0x0000) << 8;
	else frame |= static_cast<uint32_t>(CurrentAltitude) << 8; //zapis wysokosci [ft.] bity 8-23
	checksum = static_cast<uint8_t>(frame) ^ static_cast<uint8_t>(frame >> 8) ^ static_cast<uint8_t>(frame >> 16); //obliczanie XOR pierwszych trzech bajtów ramki
	frame |= static_cast<uint32_t>(checksum) << 24; // zapis checksum bity 24-32
	TelemetryBuffer[CurrentBufferIndex] = frame; //zapis gotowej ramki do bufora
	CurrentBufferIndex = (CurrentBufferIndex + 1) % 16;
}
uint32_t TelemetryBlackBox::getLastDataframe() {
	uint32_t lastframe;
	uint8_t checksum;
	lastframe = TelemetryBuffer[(CurrentBufferIndex + 15) % 16];
	//log na konsole
	std::cout << "Last data frame:" << std::endl;
	std::cout << std::bitset<32>(lastframe) << std::endl;
	std::cout << "state: " << (lastframe & 0x7) << std::endl;
	std::cout << "parachute: " << ((lastframe & 0x8) >> 3) << std::endl;
	std::cout << "baro_error: " << ((lastframe & 0x10) >> 4) << std::endl;
	std::cout << "reserved: " << ((lastframe & 0xE0) >> 5) << std::endl;
	std::cout << "altitude: " << ((lastframe & 0xFFFF00) >> 8) << std::endl;
	std::cout << "checksum: " << std::bitset<8>((lastframe & 0xFF000000) >> 24) << std::endl;
	std::cout << std::endl;
	//sprawdzenie checksum i return
	checksum = static_cast<uint8_t>(lastframe) ^ static_cast<uint8_t>(lastframe >> 8) ^ static_cast<uint8_t>(lastframe >> 16);
	if (checksum == static_cast<uint8_t>((lastframe & 0xFF000000) >> 24)) return lastframe;
	else return 0xFFFFFFFF;

}