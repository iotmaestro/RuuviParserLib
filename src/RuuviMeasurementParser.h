#include <string>

#ifndef RUUVI_MEASUREMENT_PARSER_H
#define RUUVI_MEASUREMENT_PARSER_H

/**
 * Parsed Ruuvi measurements.
 *
 * The struct holds a union of fields known to all versions. However, not all
 * versions necessarily fill in all fields.
 *
 * See https://docs.ruuvi.com/communication/bluetooth-advertisements and
 * individual data formats for more details.
 */
struct RuuviMeasurement {
  /**
   * Data format version that was used for this measurement.
   */
  uint8_t dataFormat;

  /**
   * Reported temperature in degrees Celsius.
   */
  float temperature;

  /**
   * Relative humidity, in percents.
   */
  float humidity;

  /**
   * Atmospheric pressure, in pascals.
   */
  float pressure;

  /**
   * Acceleration in the X axis, as G multiples.
   */
  float accelerationX;

  /**
   * Acceleration in the Y axis, as G multiples.
   */
  float accelerationY;

  /**
   * Acceleration in the Z axis, as G multiples.
   */
  float accelerationZ;

  /**
   * Voltage of the tag battery, in millivolts.
   */
  uint16_t batteryVoltage;

  /**
   * TX power, in dBm.
   */
  int8_t txPower;

  /**
   * Movement counter, incremented by motion detection interrupts from
   * accelerometer.
   */
  uint8_t movementCounter;

  /**
   * Measurement sequence number (16 bit unsigned), each time a measurement is
   * taken, this is incremented by one, used for measurement de-duplication.
   * Depending on the transmit interval, multiple packets with the same
   * measurements can be sent, and there may be measurements that never were
   * sent.
   */
  uint16_t sequenceNumber;

  /**
   * The 48 bit MAC address of the tag. The least significant 48 bits are used.
   */
  uint64_t macAddress;
};

class RuuviMeasurementParser {
 public:
  /**
   * Parses a raw byte string as a Ruuvi measurement. Returns whether the
   * operation was successful.
   *
   * It's guaranteed that the measurement is only changed if the method returns
   * true.
   */
  bool parseRuuviMeasurement(const std::string& rawData,
                             RuuviMeasurement* measurement);

 protected:
  /**
   * Prints a debug statement. Not implemented by default, override for platform
   * specific behavior.
   */
  virtual void debug(const std::string& statement) {};

 private:
  static unsigned char getByte(const std::string& s, int offset);
  static uint16_t getUnsignedShort(const std::string& s, int offset);
  static int16_t getSignedShort(const std::string& s, int offset);
};

#endif
