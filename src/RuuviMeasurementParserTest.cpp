#include <gtest/gtest.h>

#include <vector>

#include "RuuviMeasurementParser.h"

/**
 * SO stolen dirty translator of hex strings to bytes.
 */
std::string toBytes(const std::string& hexString) {
  std::vector<char> bytes;

  for (unsigned int i = 0; i < hexString.length(); i += 2) {
    std::string byteString = hexString.substr(i, 2);
    char byte = (char)strtol(byteString.c_str(), NULL, 16);
    bytes.push_back(byte);
  }

  return std::string(bytes.begin(), bytes.end());
}

TEST(RuuviMeasurementParserTest, v5_validData) {
  std::string rawData =
      toBytes("99040512FC5394C37C0004FFFC040CAC364200CDCBB8334C884F");
  RuuviMeasurement measurement;
  RuuviMeasurementParser parser;
  parser.parseRuuviMeasurement(rawData, &measurement);

  ASSERT_EQ(measurement.dataFormat, 5);
  ASSERT_FLOAT_EQ(measurement.temperature, 24.3);
  ASSERT_FLOAT_EQ(measurement.pressure, 100044);
  ASSERT_FLOAT_EQ(measurement.humidity, 53.49);
  ASSERT_FLOAT_EQ(measurement.accelerationX, 0.004);
  ASSERT_FLOAT_EQ(measurement.accelerationY, -0.004);
  ASSERT_FLOAT_EQ(measurement.accelerationZ, 1.036);
  ASSERT_EQ(measurement.txPower, 4);
  ASSERT_EQ(measurement.batteryVoltage, 2977);
  ASSERT_EQ(measurement.movementCounter, 66);
  ASSERT_EQ(measurement.sequenceNumber, 205);
  ASSERT_EQ(measurement.macAddress, 0xCBB8334C884F);
}

TEST(RuuviMeasurementParserTest, v5_maximumValues) {
  std::string rawData =
      toBytes("9904057FFFFFFEFFFE7FFF7FFF7FFFFFDEFEFFFECBB8334C884F");
  RuuviMeasurement measurement;
  RuuviMeasurementParser parser;
  parser.parseRuuviMeasurement(rawData, &measurement);

  ASSERT_EQ(measurement.dataFormat, 5);
  ASSERT_FLOAT_EQ(measurement.temperature, 163.835);
  ASSERT_FLOAT_EQ(measurement.pressure, 115534);
  ASSERT_FLOAT_EQ(measurement.humidity, 163.835);
  ASSERT_FLOAT_EQ(measurement.accelerationX, 32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationY, 32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationZ, 32.767);
  ASSERT_EQ(measurement.txPower, 20);
  ASSERT_EQ(measurement.batteryVoltage, 3646);
  ASSERT_EQ(measurement.movementCounter, 254);
  ASSERT_EQ(measurement.sequenceNumber, 65534);
  ASSERT_EQ(measurement.macAddress, 0xCBB8334C884F);
}

TEST(RuuviMeasurementParserTest, v5_minimumValues) {
  std::string rawData =
      toBytes("9904058001000000008001800180010000000000CBB8334C884F");
  RuuviMeasurement measurement;
  RuuviMeasurementParser parser;
  parser.parseRuuviMeasurement(rawData, &measurement);

  ASSERT_EQ(measurement.dataFormat, 5);
  ASSERT_FLOAT_EQ(measurement.temperature, -163.835);
  ASSERT_FLOAT_EQ(measurement.pressure, 50000);
  ASSERT_FLOAT_EQ(measurement.humidity, 0);
  ASSERT_FLOAT_EQ(measurement.accelerationX, -32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationY, -32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationZ, -32.767);
  ASSERT_EQ(measurement.txPower, -40);
  ASSERT_EQ(measurement.batteryVoltage, 1600);
  ASSERT_EQ(measurement.movementCounter, 0);
  ASSERT_EQ(measurement.sequenceNumber, 0);
  ASSERT_EQ(measurement.macAddress, 0xCBB8334C884F);
}

TEST(RuuviMeasurementParserTest, v5_zeroBytes) {
  std::string rawData =
      toBytes("99040511785e00c7560008000803e8a6960500aec2dff2f640c6");
  RuuviMeasurement measurement;
  RuuviMeasurementParser parser;
  parser.parseRuuviMeasurement(rawData, &measurement);

  EXPECT_EQ(measurement.dataFormat, 5);
  EXPECT_FLOAT_EQ(measurement.temperature, 22.36);
  EXPECT_FLOAT_EQ(measurement.pressure, 101030);
  EXPECT_FLOAT_EQ(measurement.humidity, 60.16);
  EXPECT_FLOAT_EQ(measurement.accelerationX, 0.008);
  EXPECT_FLOAT_EQ(measurement.accelerationY, 0.008);
  EXPECT_FLOAT_EQ(measurement.accelerationZ, 1);
  EXPECT_EQ(measurement.txPower, 4);
  EXPECT_EQ(measurement.batteryVoltage, 2932);
  EXPECT_EQ(measurement.movementCounter, 5);
  EXPECT_EQ(measurement.sequenceNumber, 174);
  EXPECT_EQ(measurement.macAddress, 0xC2DFF2F640C6);
}

TEST(RuuviMeasurementParserTest, v3_validData) {
  std::string rawData = toBytes("990403291A1ECE1EFC18F94202CA0B53");
  RuuviMeasurement measurement;
  RuuviMeasurementParser parser;
  parser.parseRuuviMeasurement(rawData, &measurement);

  ASSERT_EQ(measurement.dataFormat, 3);
  ASSERT_FLOAT_EQ(measurement.temperature, 26.3);
  ASSERT_FLOAT_EQ(measurement.pressure, 102766);
  ASSERT_FLOAT_EQ(measurement.humidity, 20.5);
  ASSERT_FLOAT_EQ(measurement.accelerationX, -1.0);
  ASSERT_FLOAT_EQ(measurement.accelerationY, -1.726);
  ASSERT_FLOAT_EQ(measurement.accelerationZ, 0.714);
  ASSERT_EQ(measurement.batteryVoltage, 2899);
}

TEST(RuuviMeasurementParserTest, v3_maximumValues) {
  std::string rawData = toBytes("990403FF7F63FFFF7FFF7FFF7FFFFFFF");
  RuuviMeasurement measurement;
  RuuviMeasurementParser parser;
  parser.parseRuuviMeasurement(rawData, &measurement);

  ASSERT_EQ(measurement.dataFormat, 3);
  ASSERT_FLOAT_EQ(measurement.temperature, 127.99);
  ASSERT_FLOAT_EQ(measurement.pressure, 115535);
  ASSERT_FLOAT_EQ(measurement.humidity, 127.5);
  ASSERT_FLOAT_EQ(measurement.accelerationX, 32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationY, 32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationZ, 32.767);
  ASSERT_EQ(measurement.batteryVoltage, 65535);
}



TEST(RuuviMeasurementParserTest, v3_minimumValues) {
  std::string rawData = toBytes("99040300FF6300008001800180010000");
  RuuviMeasurement measurement;
  RuuviMeasurementParser parser;
  parser.parseRuuviMeasurement(rawData, &measurement);

  ASSERT_EQ(measurement.dataFormat, 3);
  ASSERT_FLOAT_EQ(measurement.temperature, -127.99);
  ASSERT_FLOAT_EQ(measurement.pressure, 50000);
  ASSERT_FLOAT_EQ(measurement.humidity, 0);
  ASSERT_FLOAT_EQ(measurement.accelerationX, -32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationY, -32.767);
  ASSERT_FLOAT_EQ(measurement.accelerationZ, -32.767);
  ASSERT_EQ(measurement.batteryVoltage, 0);
}
