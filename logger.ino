void setup_sd_card() {
  if (!SD.begin(SD_CS)) {
    Serial.println(F("Card failed or not present, no SD Card data logging possible..."));
  } else {
    Serial.println(F("Card initialised... file access enabled..."));
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println(F("Tidak Ada Kartu SD"));
    state_sdcard = 1;
    return;
  }
  else {
    state_sdcard = 0;
  }
  Serial.println(F("Succes initialization SD Card"));
}

void check_dir() {
  File file = SD.open("/Live Location.csv");
  if (!file) {
    Serial.println("File /Live Location.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Live Location.csv", "sn, cn, brand, date, time, timezone, shift, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /Live Location.csv already exists");
  }
  file.close();

  File file1 = SD.open("/Engine Running Shift 1.csv");
  if (!file1) {
    Serial.println("File /Engine Running Shift 1.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Engine Running Shift 1.csv", "sn, cn, brand, date, time, timezone, shift, adc altenator, adc parking break, adc netral, adc fuel, condition, start, stop, duration, hm start, hm stop, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /Engine Running Shift 1.csv already exists");
  }
  file1.close();

  File file2 = SD.open("/Engine Idle Shift 1.csv");
  if (!file2) {
    Serial.println("File /Engine Idle Shift 1.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Engine Idle Shift 1.csv", "sn, cn, brand, date, time, timezone, shift, hm, adc altenator, adc parking break, adc netral, adc fuel, condition, start, stop, duration, hm start, hm stop, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /Engine Idle Shift 1.csv already exists");
  }
  file2.close();

  File file3 = SD.open("/Engine OFF Shift 1.csv");
  if (!file3) {
    Serial.println("File /Engine OFF Shift 1.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Engine OFF Shift 1.csv", "sn, cn, brand, date, time, timezone, shift, hm, adc altenator, adc parking break, adc netral, adc fuel, condition, start, stop, duration, hm start, hm stop, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /Engine OFF Shift 1.csv already exists");
  }
  file3.close();

  File file4 = SD.open("/Engine Running Shift 2.csv");
  if (!file4) {
    Serial.println("File /Engine Running Shift 2.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Engine Running Shift 2.csv", "sn, cn, brand, date, time, timezone, shift, hm, adc altenator, adc parking break, adc netral, adc fuel, condition, start, stop, duration, hm start, hm stop, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /Engine Running Shift 2.csv already exists");
  }
  file4.close();

  File file5 = SD.open("/Engine Idle Shift 2.csv");
  if (!file5) {
    Serial.println("File /Engine Idle Shift 2.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Engine Idle Shift 2.csv", "sn, cn, brand, date, time, timezone, shift, hm, adc altenator, adc parking break, adc netral, adc fuel, condition, start, stop, duration, hm start, hm stop, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /Engine Idle Shift 2.csv already exists");
  }
  file5.close();

  File file6 = SD.open("/Engine OFF Shift 2.csv");
  if (!file6) {
    Serial.println("File /Engine OFF Shift 2.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Engine OFF Shift 2.csv", "sn, cn, brand, date, time, timezone, shift, hm, adc altenator, adc parking break, adc netral, adc fuel, condition, start, stop, duration, hm start, hm stop, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /Engine OFF Shift 2.csv already exists");
  }
  file6.close();

  File file7 = SD.open("/All History.csv");
  if (!file7) {
    Serial.println("File /All History.csv doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/All History.csv", "sn, cn, brand, date, time, timezone, shift, hm, adc altenator, adc parking break, adc netral, adc fuel, condition, start, stop, duration, hm start, hm stop, latitude, longitude, altitude, speed, heading\n");
  }
  else {
    Serial.println("File /All History.csv already exists");
  }
  file7.close();

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void logging(String condition, int shift, bool en_log) {
  while (condition == "running" and shift == 1 and en_log == true) {
    String dataLog = sn + "," + cn + "," + brand + "," + dates + "," + times + "," + timeZone + "," + shift + "," + adc_altenator + "," + adc_parking + "," + adc_netral + "," + adc_fuel + "," + condition + "," + String(datetime_start_running) + "," +
                     String(datetime_stop_running) + "," + String(duration_running) + "," + String(hm_start_running) + "," + String(hm_stop_running) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude) + "," + String(speed) + "," + String(heading) + "\n";
    Serial.println("dataLog: " + dataLog);
    check_dir();
    appendFile(SD, "/Engine Running Shift 1.csv", dataLog.c_str());
    appendFile(SD, "/All History.csv", dataLog.c_str());
    delay(1000);
    en_log = false;
  }
  while (condition == "idle" and shift == 1 and en_log == true) {
    String dataLog = sn + "," + cn + "," + brand + "," + dates + "," + times + "," + timeZone + "," + shift + "," + adc_altenator + "," + adc_parking + "," + adc_netral + "," + adc_fuel + "," + condition + "," + String(datetime_start_idle) + "," +
                     String(datetime_stop_idle) + "," + String(duration_idle) + "," + String(hm_start_idle) + "," + String(hm_stop_idle) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude) + "," + String(speed) + "," + String(heading) + "\n";
    Serial.println("dataLog: " + dataLog);
    check_dir();
    appendFile(SD, "/Engine Idle Shift 1.csv", dataLog.c_str());
    appendFile(SD, "/All History.csv", dataLog.c_str());
    delay(1000);
    en_log = false;
  }
  while (condition == "off" and shift == 1 and en_log == true) {
    String dataLog = sn + "," + cn + "," + brand + "," + dates + "," + times + "," + timeZone + "," + shift + "," + adc_altenator + "," + adc_parking + "," + adc_netral + "," + adc_fuel + "," + condition + "," + String(datetime_start_off) + "," +
                     String(datetime_stop_off) + "," + String(duration_off) + "," + String(hm_start_off) + "," + String(hm_stop_off) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude) + "," + String(speed) + "," + String(heading) + "\n";
    Serial.println("dataLog: " + dataLog);
    check_dir();
    appendFile(SD, "/Engine OFF Shift 1.csv", dataLog.c_str());
    appendFile(SD, "/All History.csv", dataLog.c_str());
    delay(1000);
    en_log = false;
  }
  while (condition == "running" and shift == 2 and en_log == true) {
    String dataLog = sn + "," + cn + "," + brand + "," + dates + "," + times + "," + timeZone + "," + shift + "," + adc_altenator + "," + adc_parking + "," + adc_netral + "," + adc_fuel + "," + condition + "," + String(datetime_start_running) + "," +
                     String(datetime_stop_running) + "," + String(duration_running) + "," + String(hm_start_running) + "," + String(hm_stop_running) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude) + "," + String(speed) + "," + String(heading) + "\n";
    Serial.println("dataLog: " + dataLog);
    check_dir();
    appendFile(SD, "/Engine Running Shift 2.csv", dataLog.c_str());
    appendFile(SD, "/All History.csv", dataLog.c_str());
    delay(1000);
    en_log = false;
  }
  while (condition == "idle" and shift == 2 and en_log == true) {
    String dataLog = sn + "," + cn + "," + brand + "," + dates + "," + times + "," + timeZone + "," + shift + "," + adc_altenator + "," + adc_parking + "," + adc_netral + "," + adc_fuel + "," + condition + "," + String(datetime_start_idle) + "," +
                     String(datetime_stop_idle) + "," + String(duration_idle) + "," + String(hm_start_idle) + "," + String(hm_stop_idle) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude) + "," + String(speed) + "," + String(heading) + "\n";
    Serial.println("dataLog: " + dataLog);
    check_dir();
    appendFile(SD, "/Engine Idle Shift 2.csv", dataLog.c_str());
    appendFile(SD, "/All History.csv", dataLog.c_str());
    delay(1000);
    en_log = false;
  }
  while (condition == "off" and shift == 2 and en_log == true) {
    String dataLog = sn + "," + cn + "," + brand + "," + dates + "," + times + "," + timeZone + "," + shift + "," + adc_altenator + "," + adc_parking + "," + adc_netral + "," + adc_fuel + "," + condition + "," + String(datetime_start_off) + "," +
                     String(datetime_stop_off) + "," + String(duration_off) + "," + String(hm_start_off) + "," + String(hm_stop_off) + "," + String(latitude) + "," + String(longitude) + "," + String(altitude) + "," + String(speed) + "," + String(heading) + "\n";
    Serial.println("dataLog: " + dataLog);
    check_dir();
    appendFile(SD, "/Engine OFF Shift 2.csv", dataLog.c_str());
    appendFile(SD, "/All History.csv", dataLog.c_str());
    delay(1000);
    en_log = false;
  }

  while (condition == "livelocation" and en_log == true) {
    String dataLog = sn + "," + cn + "," + brand + "," + dates + "," + times + "," + timeZone + "," + shift + "," + latitude + "," + longitude + "," + altitude + "," + speed + "," + heading + "\n";
    Serial.println("dataLog: " + dataLog);
    check_dir();
    appendFile(SD, "/Live Location.csv", dataLog.c_str());
    delay(1000);
    en_log = false;
  }
}

void loop_logging() {
  static unsigned long prev_log;
  static unsigned int count_log;
  if ((millis() - prev_log) > 1000) {
    count_log++;
    prev_log = millis();
  }

  if (count_log > 300) {
    logging("livelocation", 0, true);
    count_log = 0;
  }
}
