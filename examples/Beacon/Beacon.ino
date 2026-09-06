// Turn on the library's internal debug logging (radio/HCI level detail)
// by defining these two macros before #include <BLE.h>.
#define DEBUG_BLETEENSY
#define DEBUG_BLETEENSY_PORT Serial

#include <BLE.h>

// Starts a BLE Beacon in the background, no further app work required after begin()

BLEBeacon beacon;
BLEUUID beaconUUID;

void setup() {
  delay(5000);
  Serial.println("Bluetooth Low Energy Beacon starting...");

  BLE.begin();
  Serial.println("BLE.begin() done");

  // Mash up Teensy's factory-programmed 64-bit unique ID and a common header
  // for our "UUID"
  uint8_t uuid[16];
  memcpy(uuid, "TEENSYBCON", 10);
  Serial.println("Copied header into uuid[]");

  uint32_t id[2] = { HW_OCOTP_CFG0, HW_OCOTP_CFG1 };
  Serial.printf("Read chip unique ID: %08lX%08lX\n", (unsigned long)id[0], (unsigned long)id[1]);

  memcpy(uuid + 10, id, sizeof(id));
  Serial.println("Copied chip ID into uuid[]");

  memset(uuid + 14, 0, 2);
  Serial.println("Zeroed final 2 bytes of uuid[]");

  beaconUUID = BLEUUID(uuid);
  Serial.printf("Built beacon UUID: %s\n", beaconUUID.toString().c_str());

  // Define the beacon and start it
  const uint16_t major = 6, minor = 7;
  const int8_t txpower = -44; // -44dbm @ 1M

  beacon.setUUID(beaconUUID);
  Serial.println("beacon.setUUID() done");

  beacon.setMajorMinor(major, minor);
  Serial.printf("beacon.setMajorMinor(%u, %u) done\n", major, minor);

  beacon.setTXPower(txpower);
  Serial.printf("beacon.setTXPower(%d) done\n", txpower);

  bool ok = beacon.begin();
  Serial.printf("beacon.begin() %s\n", ok ? "succeeded" : "FAILED");

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("pinMode(LED_BUILTIN, OUTPUT) done");

  Serial.println("setup() complete");
}

void loop() {
  // BLE.update() must run every loop() iteration to service the BTstack
  // HCI UART and run loop -- see README.md.
  BLE.update();

  // Otherwise, we can do whatever app we want, the beacon is handled automatically.
  digitalWrite(LED_BUILTIN, LOW);
  Serial.printf("[%lu ms] LED off, beacon advertising\n", millis());
  delay(500);
  BLE.update();
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.printf("[%lu ms] LED on, beacon advertising\n", millis());
  delay(500);
}
