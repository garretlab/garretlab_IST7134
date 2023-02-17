#ifndef IST7134_CLASS_H
#define IST7134_CLASS_H

#include <Arduino.h>
#include <Wire.h>

class garretlab_IST7134 {
private:
  uint8_t commandAddress = 0x3c;      // I2C address for command
  uint8_t dataAddress = 0x3d;         // I2C address for data

  byte rstPin;   // Reset pin
  byte busyPin;  // Busy pin

  TwoWire *wire;

public:
  garretlab_IST7134(TwoWire &wire);

  // Hardware level controll command
  void hardwareReset();
  void waitForRelease();

  // Low level I2C command
  void sendCommand(uint8_t command);
  void sendCommand(uint8_t command[], uint8_t length);
  void sendData(uint8_t data[], uint8_t length);
  uint8_t receiveCommand(uint8_t command);

  void OSCOn();   // not tested
  void OSCOff();  // not tested

  void displayOn();
  void displayOff();

  void sleepModeOn();
  void sleepModeOff();

  void data1LatchOn();
  void data1LatchOff();
  void data2LatchOn();
  void data2LatchOff();

  void ramAddressSet(uint8_t address);
  void writeData(uint8_t data[], uint8_t length);

  void powerOn();
  void powerOff();

  void regulationRatio(uint8_t ratio);  // not tested
  void CTset(uint8_t ct);               // not tested
  void V0GeneratorCLK(uint8_t vck);     // not tested
  void setBoostRatio(uint8_t bt);       

  void TSONOn();                    
  void TSONOff();                   // not tested
  int8_t temperatureSensorData();  // not tested

  void frameTimeSet(uint8_t fts);                 // not tested
  void TSTandTSMTSet(uint8_t tst, uint8_t tsnt);  // not tested
  void reset();                                   // not tested
  void LNDIVSet(uint8_t lndiv);                   // not tested
  void waveFormSet(uint8_t bb, uint8_t bw, uint8_t wb, uint8_t ww, uint8_t com);
  void frameSet(uint8_t frame);

  // Utility functions
  void begin(byte rstPin = 8, byte busyPin = 7,
             uint8_t commandAddress = 0x3c,
             uint8_t dataAddress = 0x3d);
  void display(uint8_t address, uint8_t data[], size_t length);
  //void waveFormSetGC();
};

#endif  // IST7134_CLASS_H