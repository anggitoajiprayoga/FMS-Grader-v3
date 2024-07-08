void get_start(String condition) {
  if (condition == "running") {
    start_tanggal_running = Day;
    start_bulan_running = Month;
    start_tahun_running = Year;
    start_jam_running = Hour;
    start_menit_running = Minute;
    start_detik_running = Second;
    sprintf(datetime_start_running, "%04d-%02d-%02d %02d:%02d:%02d", start_tahun_running, start_bulan_running, start_tanggal_running, start_jam_running, start_menit_running, start_detik_running);
    writeFile(SPIFFS, "/datetime_start_running.txt", String(datetime_start_running).c_str());
    writeFile(SPIFFS, "/start_tanggal_running.txt", String(start_tanggal_running).c_str());
    writeFile(SPIFFS, "/start_bulan_running.txt", String(start_bulan_running).c_str());
    writeFile(SPIFFS, "/start_tahun_running.txt", String(start_tahun_running).c_str());
    writeFile(SPIFFS, "/start_jam_running.txt", String(start_jam_running).c_str());
    writeFile(SPIFFS, "/start_menit_running.txt", String(start_menit_running).c_str());
    writeFile(SPIFFS, "/start_detik_running.txt", String(start_detik_running).c_str());
  }
  else if (condition == "idle") {
    start_tanggal_idle = Day;
    start_bulan_idle = Month;
    start_tahun_idle = Year;
    start_jam_idle = Hour;
    start_menit_idle = Minute;
    start_detik_idle = Second;
    sprintf(datetime_start_idle, "%04d-%02d-%02d %02d:%02d:%02d", start_tahun_idle, start_bulan_idle, start_tanggal_idle, start_jam_idle, start_menit_idle, start_detik_idle);
    writeFile(SPIFFS, "/datetime_start_idle.txt", String(datetime_start_idle).c_str());
    writeFile(SPIFFS, "/start_tanggal_idle.txt", String(start_tanggal_idle).c_str());
    writeFile(SPIFFS, "/start_bulan_idle.txt", String(start_bulan_idle).c_str());
    writeFile(SPIFFS, "/start_tahun_idle.txt", String(start_tahun_idle).c_str());
    writeFile(SPIFFS, "/start_jam_idle.txt", String(start_jam_idle).c_str());
    writeFile(SPIFFS, "/start_menit_idle.txt", String(start_menit_idle).c_str());
    writeFile(SPIFFS, "/start_detik_idle.txt", String(start_detik_idle).c_str());
  }
  else if (condition == "off") {
    start_tanggal_off = Day;
    start_bulan_off = Month;
    start_tahun_off = Year;
    start_jam_off = Hour;
    start_menit_off = Minute;
    start_detik_off = Second;
    sprintf(datetime_start_off, "%04d-%02d-%02d %02d:%02d:%02d", start_tahun_off, start_bulan_off, start_tanggal_off, start_jam_off, start_menit_off, start_detik_off);
    writeFile(SPIFFS, "/datetime_start_off.txt", String(datetime_start_off).c_str());
    writeFile(SPIFFS, "/start_tanggal_off.txt", String(start_tanggal_off).c_str());
    writeFile(SPIFFS, "/start_bulan_off.txt", String(start_bulan_off).c_str());
    writeFile(SPIFFS, "/start_tahun_off.txt", String(start_tahun_off).c_str());
    writeFile(SPIFFS, "/start_jam_off.txt", String(start_jam_off).c_str());
    writeFile(SPIFFS, "/start_menit_off.txt", String(start_menit_off).c_str());
    writeFile(SPIFFS, "/start_detik_off.txt", String(start_detik_off).c_str());
  }
}

void get_stop(String condition) {
  if (condition == "running") {
    stop_tanggal_running = Day;
    stop_bulan_running = Month;
    stop_tahun_running = Year;
    stop_jam_running = Hour;
    stop_menit_running = Minute;
    stop_detik_running = Second;
    sprintf(datetime_stop_running, "%04d-%02d-%02d %02d:%02d:%02d", stop_tahun_running, stop_bulan_running, stop_tanggal_running, stop_jam_running, stop_menit_running, stop_detik_running);
    calculation("engine running", start_jam_running, start_menit_running, start_detik_running, stop_jam_running, stop_menit_running, stop_detik_running, start_tanggal_running, start_bulan_running, start_tahun_running, stop_tanggal_running, stop_bulan_running, stop_tahun_running);
  }
  else if (condition == "idle") {
    stop_tanggal_idle = Day;
    stop_bulan_idle = Month;
    stop_tahun_idle = Year;
    stop_jam_idle = Hour;
    stop_menit_idle = Minute;
    stop_detik_idle = Second;
    sprintf(datetime_stop_idle, "%04d-%02d-%02d %02d:%02d:%02d", stop_tahun_idle, stop_bulan_idle, stop_tanggal_idle, stop_jam_idle, stop_menit_idle, stop_detik_idle);
    calculation("engine idle", start_jam_idle, start_menit_idle, start_detik_idle, stop_jam_idle, stop_menit_idle, stop_detik_idle, start_tanggal_idle, start_bulan_idle, start_tahun_idle, stop_tanggal_idle, stop_bulan_idle, stop_tahun_idle);
  }
  else if (condition == "off") {
    stop_tanggal_off = Day;
    stop_bulan_off = Month;
    stop_tahun_off = Year;
    stop_jam_off = Hour;
    stop_menit_off = Minute;
    stop_detik_off = Second;
    sprintf(datetime_stop_off, "%04d-%02d-%02d %02d:%02d:%02d", stop_tahun_off, stop_bulan_off, stop_tanggal_off, stop_jam_off, stop_menit_off, stop_detik_off);
    calculation("engine off", start_jam_off, start_menit_off, start_detik_off, stop_jam_off, stop_menit_off, stop_detik_off, start_tanggal_off, start_bulan_off, start_tahun_off, stop_tanggal_off, stop_bulan_off, stop_tahun_off);
  }
}

void calculation(String state, int jam_awal, int menit_awal, int detik_awal, int jam_akhir, int menit_akhir, int detik_akhir, int tanggal_awal, int bulan_awal, int tahun_awal, int tanggal_akhir, int bulan_akhir, int tahun_akhir) {
  //dtk,mnt,jam,hari,tgl,bln,tahun
  tmElements_t tanggalPertama = {detik_awal, menit_awal, jam_awal, 0, tanggal_awal, bulan_awal, CalendarYrToTm(tahun_awal)};
  tmElements_t tanggalKedua = {detik_akhir, menit_akhir, jam_akhir, 0, tanggal_akhir, bulan_akhir, CalendarYrToTm(tahun_akhir)};
  tmElements_t selisih;
  breakTime(makeTime(tanggalKedua) - makeTime(tanggalPertama), selisih);

  if (state == "engine running") {
    jam_running = selisih.Hour;
    menit_running = selisih.Minute;
    detik_running = selisih.Second;
    sprintf (duration_running, "%02d:%02d:%02d", jam_running, menit_running, detik_running);
    hour_meter(jam_running, menit_running, detik_running);
    Serial.println();
    Serial.println("====== New Event ======");
    Serial.println("condition: " + state);
    Serial.println("datetime start: " + String(datetime_start_running));
    Serial.println("datetime stop: " + String(datetime_stop_running));
    Serial.println("duration running: " + String(duration_running));
    Serial.println();
  }
  else if (state == "engine idle") {
    jam_idle = selisih.Hour;
    menit_idle = selisih.Minute;
    detik_idle = selisih.Second;
    sprintf (duration_idle, "%02d:%02d:%02d", jam_idle, menit_idle, detik_idle);
    Serial.println();
    Serial.println("====== New Event ======");
    Serial.println("condition: " + state);
    Serial.println("datetime start: " + String(datetime_start_idle));
    Serial.println("datetime stop: " + String(datetime_stop_idle));
    Serial.println("duration idle: " + String(duration_idle));
    Serial.println();
  }
  else if (state == "engine off") {
    jam_off = selisih.Hour;
    menit_off = selisih.Minute;
    detik_off = selisih.Second;
    sprintf (duration_off, "%02d:%02d:%02d", jam_off, menit_off, detik_off);
    Serial.println();
    Serial.println("====== New Event ======");
    Serial.println("condition: " + state);
    Serial.println("datetime start: " + String(datetime_start_off));
    Serial.println("datetime stop: " + String(datetime_stop_off));
    Serial.println("duration off: " + String(duration_off));
    Serial.println();
  }
}

void hour_meter(int get_jam, int get_minute, int get_second) {
  int hour_meter = readFile(SPIFFS, "/hour_meter.txt").toInt();
  hour_meter = hour_meter + get_jam;
  byte minute_meter = readFile(SPIFFS, "/minute_meter.txt").toInt();
  minute_meter = minute_meter + get_minute;
  byte second_meter = readFile(SPIFFS, "/second_meter.txt").toInt();
  second_meter = second_meter + get_second;

  while (minute_meter > 60) {
    minute_meter = minute_meter - 60;
    hour_meter = hour_meter + 1;
  }
  while (second_meter > 60) {
    second_meter = second_meter - 60;
    minute_meter = minute_meter + 1;
  }

  sprintf(hm_unit, "%02d:%02d:%02d", hour_meter, minute_meter, second_meter);
  writeFile(SPIFFS, "/hm_unit.txt", String(hm_unit).c_str());
  writeFile(SPIFFS, "/hour_meter.txt", String(hour_meter).c_str());
  writeFile(SPIFFS, "/minute_meter.txt", String(minute_meter).c_str());
  writeFile(SPIFFS, "/second_meter.txt", String(second_meter).c_str());
}

void hm_update(int hm_jam, int hm_menit, int hm_detik) {
  hour_realtime = readFile(SPIFFS, "/hour_meter.txt").toInt();
  minute_realtime = readFile(SPIFFS, "/minute_meter.txt").toInt();
  second_realtime = readFile(SPIFFS, "/second_meter.txt").toInt();

  hour_realtime = hour_realtime + hm_jam;
  minute_realtime = minute_realtime + hm_menit;
  second_realtime = second_realtime + hm_detik;

  while (minute_realtime > 60) {
    minute_realtime = minute_realtime - 60;
    hour_realtime = hour_realtime + 1;
  }
  while (second_realtime > 60) {
    second_realtime = second_realtime - 60;
    minute_realtime = minute_realtime + 1;
  }

  sprintf(hm_realtime, "%02d:%02d:%02d", hour_realtime, minute_realtime, second_realtime);
}

void update(String condition, int jam_awal, int menit_awal, int detik_awal, int jam_akhir, int menit_akhir, int detik_akhir, int tanggal_awal, int bulan_awal, int tahun_awal, int tanggal_akhir, int bulan_akhir, int tahun_akhir) {
  //dtk,mnt,jam,hari,tgl,bln,tahun
  tmElements_t tanggalPertama = {detik_awal, menit_awal, jam_awal, 0, tanggal_awal, bulan_awal, CalendarYrToTm(tahun_awal)};
  tmElements_t tanggalKedua = {detik_akhir, menit_akhir, jam_akhir, 0, tanggal_akhir, bulan_akhir, CalendarYrToTm(tahun_akhir)};
  tmElements_t selisih;
  breakTime(makeTime(tanggalKedua) - makeTime(tanggalPertama), selisih);

  if (condition == "engine running") {
    sprintf (running_realtime, "%02d:%02d:%02d", selisih.Hour, selisih.Minute, selisih.Second);
    hm_update(selisih.Hour, selisih.Minute, selisih.Second);
    time_update = running_realtime;
  }
  if (condition == "engine idle") {
    sprintf (idle_realtime, "%02d:%02d:%02d", selisih.Hour, selisih.Minute, selisih.Second);
    time_update = idle_realtime;
  }
  if (condition == "engine off") {
    sprintf (off_realtime, "%02d:%02d:%02d", selisih.Hour, selisih.Minute, selisih.Second);
    time_update = off_realtime;
  }
}

void get_update(String condition, int jam_on, int menit_on, int detik_on, int tanggal_on, int bulan_on, int tahun_on) {
  int jam_update = Hour;
  int menit_update = Minute;
  int detik_update = Second;
  int tanggal_update = Day;
  int bulan_update = Month;
  int tahun_update = Year;
  update(condition, jam_on, menit_on, detik_on, jam_update, menit_update, detik_update, tanggal_on, bulan_on, tahun_on, tanggal_update, bulan_update, tahun_update);
}

void changeShift() {
  if (Hour >= 7 and Hour < 19) {       // Change shift siang
    shift = 1;
  }
  else if (Hour >= 19 and Hour < 24) {  // Change shift malam
    shift = 2;
  }
  else if (Hour > -1 and Hour < 7) {  // Change shift malam
    shift = 2;
  }

  if (Hour == 18) {
    xshift = shift;
  }

  if (Hour == 6) {
    xshift = shift;
  }

  if ((xshift != shift) or (Hour == 18 and Minute == 59 and Second == 59) or (Hour == 6 and Minute == 59 and Second == 59)) {
    if (engine_running == 1) {
      hm_stop_running = hm_realtime;
      get_stop("running");
      String get_production = payload_production("Engine Running", shift, String(datetime_start_running), String(datetime_stop_running), duration_running, String(hm_start_running), String(hm_stop_running));
      running_historical = get_production;
      logging("running", shift, true);
      pub("fms/gd/production/" + cn, get_production);
      en_write_running = 0;
      writeFile(SPIFFS, "/en_write_running.txt", String(en_write_running).c_str());
      writeFile(SPIFFS, "/datetime_start_running.txt", "");
      writeFile(SPIFFS, "/start_tanggal_running.txt", "");
      writeFile(SPIFFS, "/start_bulan_running.txt", "");
      writeFile(SPIFFS, "/start_tahun_running.txt", "");
      writeFile(SPIFFS, "/start_jam_running.txt", "");
      writeFile(SPIFFS, "/start_menit_running.txt", "");
      writeFile(SPIFFS, "/start_detik_running.txt", "");
      writeFile(SPIFFS, "/hm_start_running.txt", "");
      writeFile(SPIFFS, "/hm_stop_running.txt", "");
      writeFile(SPIFFS, "/engine_running.txt", "");
    }
    if (engine_idle == 1) {
      hm_stop_idle = hm_realtime;
      get_stop("idle");
      String get_production = payload_production("Engine Idle", shift, String(datetime_start_idle), String(datetime_stop_idle), duration_idle, String(hm_start_idle), String(hm_stop_idle));
      idle_historical = get_production;
      logging("idle", shift, true);
      pub("fms/gd/production/" + cn, get_production);
      en_write_idle = 0;
      writeFile(SPIFFS, "/en_write_idle.txt", String(en_write_idle).c_str());
      writeFile(SPIFFS, "/datetime_start_idle.txt", "");
      writeFile(SPIFFS, "/start_tanggal_idle.txt", "");
      writeFile(SPIFFS, "/start_bulan_idle.txt", "");
      writeFile(SPIFFS, "/start_tahun_idle.txt", "");
      writeFile(SPIFFS, "/start_jam_idle.txt", "");
      writeFile(SPIFFS, "/start_menit_idle.txt", "");
      writeFile(SPIFFS, "/start_detik_idle.txt", "");
      writeFile(SPIFFS, "/hm_start_idle.txt", "");
      writeFile(SPIFFS, "/hm_stop_idle.txt", "");
      writeFile(SPIFFS, "/engine_idle.txt", "");
    }
    if (engine_off == 1) {
      hm_stop_off = hm_realtime;
      get_stop("off");
      String get_production = payload_production("Engine Off", shift, String(datetime_start_off), String(datetime_stop_off), duration_off, String(hm_start_off), String(hm_stop_off));
      off_historical = get_production;
      logging("off", shift, true);
      pub("fms/gd/production/" + cn, get_production);
      en_write_off = 0;
      writeFile(SPIFFS, "/en_write_off.txt", String(en_write_off).c_str());
      writeFile(SPIFFS, "/datetime_start_off.txt", "");
      writeFile(SPIFFS, "/start_tanggal_off.txt", "");
      writeFile(SPIFFS, "/start_bulan_off.txt", "");
      writeFile(SPIFFS, "/start_tahun_off.txt", "");
      writeFile(SPIFFS, "/start_jam_off.txt", "");
      writeFile(SPIFFS, "/start_menit_off.txt", "");
      writeFile(SPIFFS, "/start_detik_off.txt", "");
      writeFile(SPIFFS, "/hm_start_off.txt", "");
      writeFile(SPIFFS, "/hm_stop_off.txt", "");
      writeFile(SPIFFS, "/engine_off.txt", "");
    }

    if (Hour == 6 and Minute == 59 and Second == 59) {
      shift = 1;
    }
    if (Hour == 18 and Minute == 59 and Second == 59) {
      shift = 2;
    }

    xshift = shift;
    writeFile(SPIFFS, "/xshift.txt", String(shift).c_str());
    Serial.println("Restart Device Change Shift");
    ESP.restart();
  }
}
