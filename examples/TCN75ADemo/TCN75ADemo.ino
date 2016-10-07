#include <DTWI.h>
#include <TCN75A.h>

DTWI0 twi;
TCN75A temp(twi);

void setup() {
    Serial.begin(115200);
    temp.begin();
}

void loop() {
    Serial.printf("The temperature is: %6.2f C\r\n", temp.readTemperature());
    delay(1000);
}
