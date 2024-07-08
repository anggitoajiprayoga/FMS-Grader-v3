void serial_monitor() {
  static unsigned long prev_sermon = 0;
  if ((millis() - prev_sermon) > 1000) {
    Serial.println("cn: " + cn + " | dateTime: " + dateTime + " | timeZone: " + timeZone + " | shift: " + String(shift) + " | board_temp: " + board_temp);
    Serial.println("lat: " + String(latitude) + " | lng: " + String(longitude) + " | alt: " + String(altitude) + " | spd: " + String(speed) + " | hdg: " + String(heading));
    Serial.println("broker: " + broker + " | username: " + username + " | password: " + password  + " | interval_publish: " + interval_mqtt  + " | en_lte: " + en_lte  + " | en_mqtt: " + en_mqtt);
    Serial.println("adc_altenator: " + String(altenator) + " | adc_netral: " + String(netral) + " | adc_parking_break: " + String(parking_break) + " | fuel: " + String(fuel));
    Serial.println("engine_running: " + String(engine_running) + " | engine_idle: " + String(engine_idle) + " | engine_off: " + String(engine_off));
    Serial.println("engine_state: " + engine_state + " | duration: " + String(time_update) + " | hm_unit: " + String(hm_unit) + " | hm_realtime: " + String(hm_realtime));
    Serial.println("datetime start running: " + String(datetime_start_running) + " | datetime stop running: " + String(datetime_stop_running) + " | duration running: " + duration_running);
    Serial.println("datetime start idle: " + String(datetime_start_idle) + " | datetime stop idle: " + String(datetime_stop_idle) + " | duration idle: " + duration_idle);
    Serial.println("datetime start off: " + String(datetime_start_off) + " | datetime stop off: " + String(datetime_stop_off) + " | duration off: " + duration_off);
    Serial.println();
    prev_sermon = millis();
  }
}
