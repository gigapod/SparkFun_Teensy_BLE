// Run a simple BLE scan and dump anything found, over and over

// Turn on the library's internal debug logging (radio/HCI level detail)
// by defining these two macros before #include <BLE.h>.
#define DEBUG_BLETEENSY
#define DEBUG_BLETEENSY_PORT Serial

#include <BLE.h>

void setup() {
  delay(5000);
  BLE.begin(""); // Name not important, we're not going to advertise
  Serial.println("Bluetooth Low Energy started");
}

void loop() {
  // BLE.scan() pumps BLE.update() internally while it runs, so no explicit
  // call is needed in this sketch's loop() -- see README.md.
  Serial.println("Starting scan...");
  uint32_t start = millis();
  BLEScanReport *report = BLE.scan(10);
  uint32_t elapsed = millis() - start;

  Serial.printf("Scan took %lu ms, found %u device(s):\n", (unsigned long)elapsed, (unsigned)report->size());
  int idx = 0;
  for (auto item : *report) {
    Serial.printf("[%d] ", idx++);
    Serial.println(item.toString());
  }
  Serial.println("DONE");
}

