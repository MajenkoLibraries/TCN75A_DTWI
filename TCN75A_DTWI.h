#ifndef _TCN75A_DTWI_H
#define _TCN75A_DTWI_H

#include <Arduino.h>
#include <DTWI.h>

#define TCN75A_ADDRESS 0x48

class TCN75A {
    private:
        DTWI *_dtwi;
        uint8_t _i2caddr;

    public:
        TCN75A(DTWI *d) : _dtwi(d), _i2caddr(TCN75A_ADDRESS) {}
        TCN75A(DTWI &d) : _dtwi(&d), _i2caddr(TCN75A_ADDRESS) {}
        TCN75A(DTWI *d, uint8_t addr) : _dtwi(d), _i2caddr(addr) {}
        TCN75A(DTWI &d, uint8_t addr) : _dtwi(&d), _i2caddr(addr) {}

        void begin();
        void begin(uint8_t addr);
        void writeRegister(uint8_t reg, uint8_t val);
        uint16_t readRegister(uint8_t reg);
        float readTemperature();
};

#endif
