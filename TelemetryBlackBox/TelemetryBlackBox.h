#pragma once
#include <cstdint>
enum RocketState : uint8_t {
	DISARM = 0,
	ARM = 1,
	FLIGHT = 2,
	PARACHUTE = 3,
	LANDED = 4
};
class TelemetryBlackBox {
public:
	void onRocketStateChange(RocketState state);
	void onParachuteDeployed();
	void onBaroReading(bool error, uint16_t value);
	void recordCurrentState();
	uint32_t getLastDataframe();
private:
	uint8_t CurrentState = DISARM;
	bool CurrentParachuteState = 0;
	bool CurrentBaroError = 0;
	uint16_t CurrentAltitude = 0;
	uint32_t TelemetryBuffer[16] = {};
	uint8_t CurrentBufferIndex = 0;
};


