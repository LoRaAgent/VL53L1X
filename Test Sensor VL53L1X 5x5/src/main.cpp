#include "Adafruit_VL53L1X.h"

#define IRQ_PIN 2
#define XSHUT_PIN 3

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("Adafruit VL53L1X sensor demo"));

  Wire.begin();
  if (!vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (!vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }
  Serial.println(F("Ranging started"));

  // Valid timing budgets: 15, 20, 33, 50, 100, 200, and 500ms!
  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());

  // ตั้งค่า ROI Center ที่พิกัด (8, 8) และ ROI Size เป็น 16x16 โดยใช้ VL53L1X_SetROI
  if (vl53.VL53L1X_SetROI(5, 5) == 0) {
    Serial.println(F("ตั้งค่า ROI สำเร็จ"));
  } else {
    Serial.println(F("ตั้งค่า ROI ล้มเหลว"));
  }

  /*
  vl53.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl53.VL53L1X_SetInterruptPolarity(0);
  */
}

void loop() {
  int16_t distance;

  if (vl53.dataReady()) {
    // มีข้อมูลใหม่ที่สามารถอ่านได้!
    distance = vl53.distance();
    if (distance == -1) {
      // มีบางอย่างผิดพลาด!
      Serial.print(F("Couldn't get distance: "));
      Serial.println(vl53.vl_status);
      return;
    }
    Serial.print(F("Distance: "));
    Serial.print(distance / 10);
    Serial.println(" cm");
    

    // อ่านข้อมูลเสร็จแล้ว พร้อมสำหรับการอ่านครั้งต่อไป!
    vl53.clearInterrupt();
  }
  
}