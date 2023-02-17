#include <garretlab_IST7134.h>

// Constructor.
garretlab_IST7134::garretlab_IST7134(TwoWire &wire) {
  this->wire = &wire;
}

// Hardware Reset.
void garretlab_IST7134::hardwareReset(void) {
  digitalWrite(rstPin, 1);
  delay(200);
  digitalWrite(rstPin, 0);
  delay(20);
  digitalWrite(rstPin, 1);
  delay(200);
}

void garretlab_IST7134::waitForRelease() {
  while (1) {
    if (digitalRead(busyPin) == HIGH) {
      break;
    }
    delay(1);
  }
}

void garretlab_IST7134::sendCommand(uint8_t command) {
  wire->beginTransmission(commandAddress);
  wire->write(command);
  wire->endTransmission();
}

void garretlab_IST7134::sendCommand(uint8_t command[], uint8_t length) {
  wire->beginTransmission(commandAddress);
  for (int i = 0; i < length; i++) {
    wire->write(command[i]);
  }
  wire->endTransmission();
}

void garretlab_IST7134::sendData(uint8_t data[], uint8_t length) {
  wire->beginTransmission(dataAddress);
  for (int i = 0; i < length; i++) {
    wire->write(data[i]);
  }
  wire->endTransmission();
}

#if 0
uint8_t garretlab_IST7134::receiveCommandX(uint8_t command) {
  uint8_t data;

  wire->beginTransmission(commandAddress);
  delay(10);
  wire->write(command);
  data = wire->read();
  wire->endTransmission();
  return data;
}
#endif

uint8_t garretlab_IST7134::receiveCommand(uint8_t command) {
  uint8_t data;

  sendCommand(command);
  wire->requestFrom(commandAddress, (uint8_t)1);
  while (wire->available() == 0)
    ;
  data = wire->read();
  return data;
}

// Device specific commands
void garretlab_IST7134::OSCOn() {
  sendCommand(0x03);
}

void garretlab_IST7134::OSCOff() {
  sendCommand(0x02);
}

void garretlab_IST7134::displayOn() {
  sendCommand(0xaf);
}

void garretlab_IST7134::displayOff() {
  sendCommand(0xae);
}

void garretlab_IST7134::sleepModeOn() {
  sendCommand(0xad);
}

void garretlab_IST7134::sleepModeOff() {
  sendCommand(0xac);
}

void garretlab_IST7134::data1LatchOn() {
  sendCommand(0xa9);
}

void garretlab_IST7134::data1LatchOff() {
  sendCommand(0xa8);
}

void garretlab_IST7134::data2LatchOn() {
  sendCommand(0xab);
}

void garretlab_IST7134::data2LatchOff() {
  sendCommand(0xaa);
}

void garretlab_IST7134::ramAddressSet(uint8_t address) {
  sendCommand(0x40 | address);
}

void garretlab_IST7134::writeData(uint8_t data[], uint8_t length) {
  sendData(data, length);
}

void garretlab_IST7134::powerOn() {
  sendCommand(0x2b);
}

void garretlab_IST7134::powerOff() {
  sendCommand(0x28);
}

void garretlab_IST7134::regulationRatio(uint8_t ratio) {
  sendCommand(0x70 | (ratio & 0xf));
}

void garretlab_IST7134::CTset(uint8_t ct) {
  sendCommand(0x81);
  sendCommand(ct);
}

void garretlab_IST7134::V0GeneratorCLK(uint8_t vck) {
  sendCommand(0xa0 | (vck & 0x03));
}

void garretlab_IST7134::setBoostRatio(uint8_t bt) {
  sendCommand(0xa4 | (bt & 0x03));
}

void garretlab_IST7134::TSONOn() {
  sendCommand(0xe1);
}

void garretlab_IST7134::TSONOff() {
  sendCommand(0xe0);
}

int8_t garretlab_IST7134::temperatureSensorData() {
  TSONOn();
  return (receiveCommand(0xe4) / 2 - 40);
}

void garretlab_IST7134::frameTimeSet(uint8_t fts) {
  sendCommand(0xb4 | (fts & 0x03));
}

void garretlab_IST7134::TSTandTSMTSet(uint8_t tst, uint8_t tsnt) {
  sendCommand(0xe5);
  sendCommand(((tst & 0x07) << 4) | (tsnt & 0x07));
}

void garretlab_IST7134::reset() {
  sendCommand(0xe2);
}

void garretlab_IST7134::LNDIVSet(uint8_t lndiv) {
  sendCommand(0xe7);
  sendCommand(lndiv);
}

void garretlab_IST7134::waveFormSet(uint8_t bb, uint8_t bw, uint8_t wb, uint8_t ww, uint8_t com) {
  sendCommand(0x82);
  sendCommand(bb);
  sendCommand(bw);
  sendCommand(wb);
  sendCommand(ww);
  sendCommand(com);
}

void garretlab_IST7134::frameSet(uint8_t frame) {
  sendCommand(0x60 | (frame & 0x0f));
}

// Initialize system.
void garretlab_IST7134::begin(byte rstPin, byte busyPin,
                         uint8_t commandAddress,
                         uint8_t dataAddress) {
  this->rstPin = rstPin;
  this->busyPin = busyPin;
  this->commandAddress = commandAddress;
  this->dataAddress = dataAddress;

  pinMode(rstPin, OUTPUT);
  pinMode(busyPin, INPUT);
  hardwareReset();
  wire->begin();
}

void garretlab_IST7134::display(uint8_t address, uint8_t *data, size_t size) {
  sleepModeOff();
  powerOn();
  data1LatchOn();
  data1LatchOff();

  ramAddressSet(address);
  writeData(data, size);

  data2LatchOn();
  data2LatchOff();
  displayOn();

  waitForRelease();

  displayOff();
  powerOff();
  sleepModeOn();
}

