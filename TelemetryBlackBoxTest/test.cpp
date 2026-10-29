#include "pch.h"
#include "TelemetryBlackBox.h"

TEST(TelemetryBlackBoxTests, FrameCodingTest) {
	TelemetryBlackBox blackBox;
	blackBox.onRocketStateChange(ARM);
	blackBox.onBaroReading(0, 100);
	blackBox.recordCurrentState();
	EXPECT_EQ(blackBox.getLastDataframe(), 0x65006401u);
}

TEST(TelemetryBlackBoxTests, BaroErrorCheck) {
	TelemetryBlackBox blackBox;
	blackBox.onRocketStateChange(ARM);
	blackBox.onBaroReading(1, 6767);
	blackBox.recordCurrentState();
	EXPECT_EQ(blackBox.getLastDataframe(), 0x11FFFF11u);
}

TEST(TelemetryBlackBoxTests, DisarmCheck) {
	TelemetryBlackBox blackBox;
	blackBox.onRocketStateChange(DISARM);
	blackBox.onBaroReading(0, 6767);
	blackBox.recordCurrentState();
	EXPECT_EQ(blackBox.getLastDataframe(), 0x00000000u);
}

TEST(TelemetryBlackBoxTests, BaroErrorAndDisarmCheck) {
	TelemetryBlackBox blackBox;
	blackBox.onRocketStateChange(DISARM);
	blackBox.onBaroReading(1, 6767);
	blackBox.recordCurrentState();
	EXPECT_EQ(blackBox.getLastDataframe(), 0x10FFFF10u);
}

TEST(TelemetryBlackBoxTests, BaroErrorAndLandedCheck) {
	TelemetryBlackBox blackBox;
	blackBox.onRocketStateChange(LANDED);
	blackBox.onBaroReading(1, 6767);
	blackBox.recordCurrentState();
	EXPECT_EQ(blackBox.getLastDataframe(), 0x14FFFF14u);
}

TEST(TelemetryBlackBoxTests, ParachuteDeployedTest) {
	TelemetryBlackBox blackBox;
	blackBox.onRocketStateChange(FLIGHT);
	blackBox.onParachuteDeployed();
	blackBox.onBaroReading(0, 12333);
	blackBox.recordCurrentState();
	EXPECT_EQ(blackBox.getLastDataframe(), 0x17302D0Au);
}

TEST(TelemetryBlackBoxTests, LandedNoBaroErrorTest) {
	TelemetryBlackBox blackBox;
	blackBox.onRocketStateChange(LANDED);
	blackBox.onParachuteDeployed();
	blackBox.onBaroReading(0, 500);
	blackBox.recordCurrentState();
	EXPECT_EQ(blackBox.getLastDataframe(), 0x0C00000Cu);
}

TEST(TelemetryBlackBoxTests, RingBufferTest) {
	TelemetryBlackBox blackBox;
	for (int i = 0; i < 18; i++) {
		blackBox.onRocketStateChange(FLIGHT);
		blackBox.onBaroReading(0, 500+(i*10));
		blackBox.recordCurrentState();
	}
	EXPECT_EQ(blackBox.getLastDataframe(), 0x9E029E02u);
}


