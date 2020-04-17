#include <TCN75A_DTWI.h>

void TCN75A::begin() {
    _dtwi->beginMaster();
    writeRegister(1, 0b01100000);
}

void TCN75A::begin(uint8_t addr) {
    _i2caddr = addr;
    _dtwi->beginMaster();
    writeRegister(1, 0b01100000);
}

void TCN75A::writeRegister(uint8_t reg, uint8_t val) {
    uint8_t state = 0;
    uint32_t start = millis();
    while (1) {
        if (millis() - start >= 1000) return;

        switch (state) {
            case 0: // begin write 
                if (_dtwi->startMasterWrite(_i2caddr)) {
                    state = 1;
                }
                break; 
            case 1: // Send register address
                if (_dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }
                break; 
            case 2: // Send register address
                if (_dtwi->write(&val, 1) == 1) {
                    state = 3;
                }
                break; 
            case 3: // Stop Master
                if (_dtwi->stopMaster()) {
                    return;
                }
                break;
        }
    }
}

uint16_t TCN75A::readRegister(uint8_t reg) {
    uint8_t state = 0;
    uint8_t msb, lsb;
    uint8_t br = 0;
    uint32_t start = millis();
    while (1) {
        if (millis() - start >= 1000) {
            return 0;
        }
        switch (state) {
            case 0: // begin write
                if (_dtwi->startMasterWrite(_i2caddr)) {
                    state = 1;
                }
                break;
            case 1: // Send register address
                if (_dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }
                break;
            case 2: // Stop Master
                if (_dtwi->stopMaster()) {
                    state = 3;
                }
                break;
            case 3:
                if (_dtwi->startMasterRead(_i2caddr, 1)) {
                    state = 4;
                }
                break;
            case 4:
                if (_dtwi->available()) {
                    _dtwi->read(&msb, 1);
                    _dtwi->read(&lsb, 1);
                    state = 5;
                    break;
                }
                break;
            case 5:
                if (_dtwi->stopMaster()) {
                    return (msb << 8) | lsb;
                }
                break;
        }
    }
}

float TCN75A::readTemperature() {
    writeRegister(1, 0b11100001);
    uint8_t status = readRegister(1);
    while (status & 0b10000000) {
        status = readRegister(1);
    }

    int16_t rval = readRegister(0);
    rval /= 16;
    return (float)rval / 16.0;
}
