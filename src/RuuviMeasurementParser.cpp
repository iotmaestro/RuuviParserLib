#include "RuuviMeasurementParser.h"

bool RuuviMeasurementParser::parseRuuviMeasurement(
    const std::string& rawData, RuuviMeasurement* measurement) {
  if (rawData.length() < 2) {
    debug("Unable to get manufacturer ID.");
    return false;
  }

  uint16_t manufacturerId = getUnsignedShort(rawData, 0);
  if (manufacturerId != 0x9904) {
    debug("Not a Ruuvi tag.");
    return false;
  }

  uint8_t dataFormat = getByte(rawData, 2);
  switch (dataFormat) {
    case 3: {
      if (rawData.length() != 16) {
        debug("Unexpected length of data");
        return false;
      }
      measurement->dataFormat = dataFormat;
      measurement->humidity = getByte(rawData, 3) * 0.5;

      char encodedWholeDeg = getByte(rawData, 4);
      char magnitudeWholeDeg = (encodedWholeDeg & 0x7F);
      bool negative = (encodedWholeDeg & 0x80);
      char decimals = getByte(rawData, 5);

      measurement->temperature = magnitudeWholeDeg + decimals * 0.01;
      if (negative) {
        measurement->temperature = -measurement->temperature;
      }

      measurement->pressure = getUnsignedShort(rawData, 6) + 50000;
      measurement->accelerationX = getSignedShort(rawData, 8) * 0.001;
      measurement->accelerationY = getSignedShort(rawData, 10) * 0.001;
      measurement->accelerationZ = getSignedShort(rawData, 12) * 0.001;
      measurement->batteryVoltage = getUnsignedShort(rawData, 14);
      return true;
    }

    case 5: {
      if (rawData.length() != 26) {
        debug("Unexpected length of data");
        return false;
      }
      measurement->dataFormat = dataFormat;
      measurement->temperature = getSignedShort(rawData, 3) * 0.005;
      measurement->humidity = getUnsignedShort(rawData, 5) * 0.0025;
      measurement->pressure = getUnsignedShort(rawData, 7) + 50000;
      measurement->accelerationX = getSignedShort(rawData, 9) * 0.001;
      measurement->accelerationY = getSignedShort(rawData, 11) * 0.001;
      measurement->accelerationZ = getSignedShort(rawData, 13) * 0.001;

      uint16_t powerInfo = getUnsignedShort(rawData, 15);
      measurement->batteryVoltage = 1600 + (powerInfo >> 5);
      measurement->txPower = ((powerInfo & 0x1F) << 1) - 40;

      measurement->movementCounter = getByte(rawData, 17);
      measurement->sequenceNumber = getUnsignedShort(rawData, 18);

      measurement->macAddress = getUnsignedShort(rawData, 20);
      measurement->macAddress <<= 16;
      measurement->macAddress |= getUnsignedShort(rawData, 22);
      measurement->macAddress <<= 16;
      measurement->macAddress |= getUnsignedShort(rawData, 24);
      return true;
    }
    default:
      debug("Unknown protocol: " + dataFormat);
      return false;
  }
}

unsigned char RuuviMeasurementParser::getByte(const std::string& s, int offset) {
  return s[offset];
}

uint16_t RuuviMeasurementParser::getUnsignedShort(const std::string& s,
                                                  int offset) {
  uint16_t ret = getByte(s, offset);
  return ret << 8 | getByte(s, offset + 1);
}

int16_t RuuviMeasurementParser::getSignedShort(const std::string& s,
                                               int offset) {
  int16_t ret = getByte(s, offset);
  return ret << 8 | getByte(s, offset + 1);
}
