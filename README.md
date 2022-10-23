# Ruuvi Measurement Parser library

[![Build Status](https://app.travis-ci.com/iotmaestro/RuuviParserLib.svg?branch=main)](https://app.travis-ci.com/iotmaestro/RuuviParserLib)

A simple library for parsing [Bluetooth advertisements from Ruuvi tags](https://docs.ruuvi.com/communication/bluetooth-advertisements).
Primarily focused on Arduino, but is platform independent.

## Example usage (Arduino)
```cpp
class RuuviTagAdvertisementCallback: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      RuuviMeasurement measurement;

      if (parser.parseRuuviMeasurement(advertisedDevice.getManufacturerData(), &measurement)) {
        if ((measurement.macAddress & RUUVI_MAC_SUFFIX) == RUUVI_MAC_SUFFIX) {
          Serial.println("Temperature is " + String(measurement.temperature));
        }                         
      }
    }
};

```

## License

MIT license.

## Contributions

Just send a PR!

