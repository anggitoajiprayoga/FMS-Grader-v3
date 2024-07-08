void setup_main() {
  if (en_lte == "true") {
    setup_sim7600ce();
  }

  if (en_mqtt == "true") {
    setup_mqtt();
  }
}

void loop_main() {
  if (en_mqtt == "true") {
    loop_mqtt();
  }

  static String serial_altenator, serial_parking, serial_netral;
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    StaticJsonDocument<1023> doc;
    Serial.println(data);

    deserializeJson(doc, data);
    JsonObject obj = doc.as<JsonObject>();

    String xserial_altenator = obj["altenator"].as<String>();
    String xserial_parking   = obj["parking"].as<String>();
    String xserial_netral    = obj["netral"].as<String>();

    if (xserial_altenator != "" and xserial_altenator != "null") {
      serial_altenator = xserial_altenator;
    }
    if (xserial_parking != "" and xserial_parking != "null") {
      serial_parking = xserial_parking;
    }
    if (xserial_netral != "" and xserial_netral != "null") {
      serial_netral = xserial_netral;
    }
    Serial.println("serial_altenator: " + serial_altenator);
    Serial.println("serial_parking: " + serial_parking);
    Serial.println("serial_netral: " + serial_netral);

    String output;
    serializeJson(doc, output);
    Serial.println(output);
  }

  if (engine_running == 1 and engine_idle == 1 and engine_off == 1) {
    engine_running = 0;
    engine_idle = 0;
    engine_off = 0;
    Serial.println();
    Serial.println("========= Eror Detect =========");
    Serial.println("Special Case - Eror Conflict Condition");
    Serial.println("Auto Restart and Reset Default Device");
    Serial.println();
    delay(2000);
    ESP.restart();
  }

  if (altenator == 1 or serial_altenator == "1") {
    if (engine_running == 0) {
      hm_start_running = hm_realtime;
      get_start("running");
      en_write_running = 1;
      engine_running = 1;
      engine_off = 0;
      writeFile(SPIFFS, "/hm_start_running.txt", String(hm_start_running).c_str());
      writeFile(SPIFFS, "/en_write_running.txt", String(en_write_running).c_str());
      writeFile(SPIFFS, "/engine_running.txt", String(engine_running).c_str());
      writeFile(SPIFFS, "/engine_off.txt", String(engine_off).c_str());
    }

    if ((netral == 1 or parking_break == 1 or serial_parking == "1" or serial_netral == "1")) {
      if (engine_idle == 0) {
        hm_start_idle = hm_realtime;
        get_start("idle");
        en_write_idle = 1;
        engine_running = 1;
        engine_idle = 1;
        engine_off = 0;
        writeFile(SPIFFS, "/hm_start_idle.txt", String(hm_start_idle).c_str());
        writeFile(SPIFFS, "/en_write_idle.txt", String(en_write_idle).c_str());
        writeFile(SPIFFS, "/engine_running.txt", String(engine_running).c_str());
        writeFile(SPIFFS, "/engine_idle.txt", String(engine_idle).c_str());
        writeFile(SPIFFS, "/engine_off.txt", String(engine_off).c_str());
        Serial.println("Engine Idle");
      }
    }
    else {
      engine_idle = 0;
      engine_off = 0;
      if (engine_running == 0) {
        hm_start_running = hm_realtime;
        engine_running = 1;
        engine_idle = 0;
        engine_off = 0;
        get_start("running");
        writeFile(SPIFFS, "/hm_start_running.txt", String(hm_start_running).c_str());
        writeFile(SPIFFS, "/engine_running.txt", String(engine_running).c_str());
        writeFile(SPIFFS, "/engine_idle.txt", String(engine_idle).c_str());
        writeFile(SPIFFS, "/engine_off.txt", String(engine_off).c_str());
        Serial.println("Engine Running");
      }
    }
  }
  else {
    if (engine_off == 0) {
      hm_start_off = hm_realtime;
      get_start("off");
      en_write_off = 1;
      engine_running = 0;
      engine_idle = 0;
      engine_off = 1;
      writeFile(SPIFFS, "/hm_start_off.txt", String(hm_start_off).c_str());
      writeFile(SPIFFS, "/en_write_off.txt", String(en_write_off).c_str());
      writeFile(SPIFFS, "/engine_running.txt", String(engine_running).c_str());
      writeFile(SPIFFS, "/engine_idle.txt", String(engine_idle).c_str());
      writeFile(SPIFFS, "/engine_off.txt", String(engine_off).c_str());
      Serial.println("Engine Off");
    }
  }

  if (engine_running == 0 and en_write_running == 1) {
    hm_stop_running = hm_realtime;
    get_stop("running");
    String get_production = payload_production("Engine Running", shift, String(datetime_start_running), String(datetime_stop_running), duration_running, String(hm_start_running), String(hm_stop_running));
    running_historical = get_production;
    logging("running", shift, true);
    pub("fms/gd/production/" + cn, get_production);
    en_write_running = 0;
    writeFile(SPIFFS, "/en_write_running.txt", String(en_write_running).c_str());
  }
  if (engine_idle == 0 and en_write_idle == 1) {
    hm_stop_idle = hm_realtime;
    get_stop("idle");
    String get_production = payload_production("Engine Idle", shift, String(datetime_start_idle), String(datetime_stop_idle), duration_idle, String(hm_start_idle), String(hm_stop_idle));
    idle_historical = get_production;
    logging("idle", shift, true);
    pub("fms/gd/production/" + cn, get_production);
    en_write_idle = 0;
    writeFile(SPIFFS, "/en_write_idle.txt", String(en_write_idle).c_str());
  }
  if (engine_off == 0 and en_write_off == 1) {
    hm_stop_off = hm_realtime;
    get_stop("off");
    String get_production = payload_production("Engine Off", shift, String(datetime_start_off), String(datetime_stop_off), duration_off, String(hm_start_off), String(hm_stop_off));
    off_historical = get_production;
    logging("off", shift, true);
    pub("fms/gd/production/" + cn, get_production);
    en_write_off = 0;
    writeFile(SPIFFS, "/en_write_off.txt", String(en_write_off).c_str());
  }

  if (engine_running == 1) {
    get_update("engine running", start_jam_running, start_menit_running, start_detik_running, start_tanggal_running, start_bulan_running, start_tahun_running);
    engine_state = "Engine Running";
  }
  if (engine_idle == 1) {
    get_update("engine idle", start_jam_idle, start_menit_idle, start_detik_idle, start_tanggal_idle, start_bulan_idle, start_tahun_idle);
    engine_state = "Engine Idle";
  }
  if (engine_off == 1) {
    get_update("engine off", start_jam_off, start_menit_off, start_detik_off, start_tanggal_off, start_bulan_off, start_tahun_off);
    engine_state = "Engine OFF";
  }

  static unsigned long prev_update;
  if (en_update == true) {
    if ((millis() - prev_update) > 300000) {
      pub("fms/gd/device/callback/" + cn, "retry update OTA proccess");
      Serial.println("Update Firmware by OTA");
      delay(2000);
      ota();
      prev_update = millis();
    }
  }

  if (firm_ver != xfirm_ver) {
    pub("fms/gd/device/callback/" + cn, "update OTA done");
    writeFile(SPIFFS, "/xfirm_ver.txt", firm_ver.c_str());
    xfirm_ver = readFile(SPIFFS, "/xfirm_ver.txt");
  }

  static unsigned long prev_rst = 0;
  static unsigned int count_reset = 0;
  if ((millis() - prev_rst) > 1000) {
    count_reset++;
    prev_rst = millis();
  }

  if (count_reset > 10800) {
    Serial.println("schedule device restart");
    delay(2000);
    ESP.restart();
  }
}

void last_update() {
  if (en_write_running == 0) {
    if (running_historical != "") {
      pub("fms/gd/production/" + cn, running_historical);
      delay(150);
    }
  }

  if (en_write_idle == 0) {
    if (idle_historical != "") {
      pub("fms/gd/production/" + cn, idle_historical);
      delay(150);
    }
  }

  if (en_write_off == 0) {
    if (off_historical != "") {
      pub("fms/gd/production/" + cn, off_historical);
      delay(150);
    }
  }
}

void websocket_parse(String input) {
  Serial.println();
  Serial.println("-----MQTT Payload Parse-----");
  Serial.println();
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  String xsn  = obj["sn"].as<String>();
  String xcn  = obj["cn"].as<String>();
  String xhm_hour  = obj["hm_hour"].as<String>();
  String xhm_minute  = obj["hm_minute"].as<String>();
  String xhm_second  = obj["hm_second"].as<String>();
  String xset_date  = obj["date"].as<String>();
  String xset_time  = obj["time"].as<String>();
  String xen_lte  = obj["en_lte"].as<String>();
  String xno_simcard  = obj["no_simcard"].as<String>();
  String xen_mqtt  = obj["en_mqtt"].as<String>();
  String xbroker  = obj["broker"].as<String>();
  String xusername  = obj["username"].as<String>();
  String xpassword  = obj["password"].as<String>();
  String xinterval_mqtt  = obj["interval_mqtt"].as<String>();
  String xserver_ota  = obj["server_ota"].as<String>();
  String xresource_ota  = obj["resource_ota"].as<String>();

  if (xsn != "null" and xsn != "") {
    sn = xsn;
    writeFile(SPIFFS, "/sn.txt", xsn.c_str());
  }
  if (xcn != "null" and xcn != "") {
    cn = xcn;
    writeFile(SPIFFS, "/cn.txt", xcn.c_str());
  }
  if (xhm_hour != "null" and xhm_hour != "") {
    writeFile(SPIFFS, "/hour_meter.txt", xhm_hour.c_str());
    hour_meter(0, 0, 0);
    hm_update(0, 0, 0);
  }
  if (xhm_minute != "null" and xhm_minute != "") {
    writeFile(SPIFFS, "/minute_meter.txt", xhm_minute.c_str());
    hour_meter(0, 0, 0);
    hm_update(0, 0, 0);
  }
  if (xhm_second != "null" and xhm_second != "") {
    writeFile(SPIFFS, "/second_meter.txt", xhm_second.c_str());
    hour_meter(0, 0, 0);
    hm_update(0, 0, 0);
  }
  if (xset_date != "null" and xset_date != "") {
    writeFile(SPIFFS, "/set_date.txt", xset_date.c_str());
  }
  if (xset_time != "null" and xset_time != "") {
    writeFile(SPIFFS, "/set_time.txt", xset_time.c_str());

    delay(500);
    String set_date = readFile(SPIFFS, "/set_date.txt");
    String set_time = readFile(SPIFFS, "/set_time.txt") + ":0";
    parsing_setDateTime(set_date, set_time);
    delay(500);
    //    rtc.adjust(DateTime(value_year.toInt(), value_month.toInt(), value_day.toInt(), value_hour.toInt(), value_minute.toInt(), value_second.toInt()));
    Rtc.SetDateTime(RtcDateTime(value_year.toInt(), value_month.toInt(), value_day.toInt(), value_hour.toInt(), value_minute.toInt(), value_second.toInt())); //tahun, bulan, tanggal, jam, menit, detik
    Serial.println("/n===============================");
    Serial.println("Set Timw Success");
    Serial.println("data_set: " + set_date + " " + set_time);
    Serial.println("===============================/n");
    delay(1000);
  }
  if (xen_lte != "null" and xen_lte != "") {
    en_lte = xen_lte;
    writeFile(SPIFFS, "/en_lte.txt", xen_lte.c_str());
  }
  if (xno_simcard != "null" and xno_simcard != "") {
    no_simcard = xno_simcard;
    writeFile(SPIFFS, "/no_simcard.txt", xno_simcard.c_str());
  }
  if (xen_mqtt != "null" and xen_mqtt != "") {
    en_mqtt = xen_mqtt;
    writeFile(SPIFFS, "/en_mqtt.txt", xen_mqtt.c_str());
  }
  if (xbroker != "null" and xbroker != "") {
    broker = xbroker;
    writeFile(SPIFFS, "/broker.txt", xbroker.c_str());
  }
  if (xusername != "null" and xusername != "") {
    username = xusername;
    writeFile(SPIFFS, "/username.txt", xusername.c_str());
  }
  if (xpassword != "null" and xpassword != "") {
    password = xpassword;
    writeFile(SPIFFS, "/password.txt", xpassword.c_str());
  }
  if (xinterval_mqtt != "null" and xinterval_mqtt != "") {
    interval_mqtt = xinterval_mqtt;
    writeFile(SPIFFS, "/interval_mqtt.txt", xinterval_mqtt.c_str());
  }
  if (xserver_ota != "null" and xserver_ota != "") {
    server_ota = xserver_ota;
    writeFile(SPIFFS, "/server_ota.txt", xserver_ota.c_str());
  }
  if (xresource_ota != "null" and xresource_ota != "") {
    resource_ota = xresource_ota;
    writeFile(SPIFFS, "/resource_ota.txt", xresource_ota.c_str());
  }
  if (xno_simcard != "null" and xno_simcard != "") {
    no_simcard = xno_simcard;
    writeFile(SPIFFS, "/no_simcard.txt", xno_simcard.c_str());
  }
}

void parsing_setDateTime(String date, String time) {
  //2000-4-1
  String value_date = "-" + date;

  int batasdate1 = value_date.indexOf('-');
  int batasdate2 = value_date.indexOf('-', batasdate1 + 1);
  int batasdate3 = value_date.indexOf('-', batasdate2 + 1);
  int batasdate4 = value_date.indexOf('-', batasdate3 + 1);

  value_year = value_date.substring(batasdate1 + 1, batasdate2);
  value_month = value_date.substring(batasdate2 + 1, batasdate3);
  value_day = value_date.substring(batasdate3 + 1, batasdate4);

  //12:59:59
  String value_time = ":" + time;

  int batastime1 = value_time.indexOf(':');
  int batastime2 = value_time.indexOf(':', batastime1 + 1);
  int batastime3 = value_time.indexOf(':', batastime2 + 1);
  int batastime4 = value_time.indexOf(':', batastime3 + 1);

  value_hour = value_time.substring(batastime1 + 1, batastime2);
  value_minute = value_time.substring(batastime2 + 1, batastime3);
  value_second = value_time.substring(batastime3 + 1, batastime4);
}

float roundToDecimalPlaces(float value, int decimalPlaces) {
  float multiplier = pow(10.0, decimalPlaces);
  return round(value * multiplier) / multiplier;
}
