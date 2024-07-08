void setup_gps() {
  serial_gps.begin(GPSBaud);
}

void loop_gps() {
  if (millis() > 5000 and gps.charsProcessed() < 10) {
    Serial.println("No GPS data received: check wiring");
    state_gps = 1;
  }
  else {
    state_gps = 0;
  }

  if (gps.location.isValid()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    speed = gps.speed.kmph();
    altitude = gps.altitude.meters();
    heading = gps.course.deg();
    satelite = gps.satellites.value();
  }

  smartDelay(200);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (serial_gps.available())
      gps.encode(serial_gps.read());
  } while (millis() - start < ms);
}
