#include <ArduinoJson.h>
#include <RadioAPI.h>
#include <RadioINIT.h>
#include <Tracking.h>
#include <Venus638.h>

Tracking * tracker;

void setup() {
    tracker = new Tracking();
}

void loop() {
    tracker->Loop();
}
