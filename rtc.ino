void setup_rtc() {
  Rtc.Begin();

  if (!Rtc.IsDateTimeValid())
  {
    state_rtc = 1;
    if (Rtc.LastError() != 0)
    {
      Serial.print("RTC communications error = ");
      Serial.println(Rtc.LastError());
    }
    else
    {
      Serial.println("Baterai RTC lemah atau jam tidak sesuai");
    }
  }
  else {
    state_rtc = 0;
  }
}

void loop_rtc() {
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  Year = now.Year();
  Month = now.Month();
  Day = now.Day();
  Hour = now.Hour();
  Minute = now.Minute();
  Second = now.Second();

  RtcTemperature temp = Rtc.GetTemperature();
  board_temp = String(temp.AsFloatDegC(), 1);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt)
{
  char tanggalx[15];
  char waktux[15];
  snprintf_P(tanggalx,
             countof(tanggalx),
             PSTR("%04u-%02u-%02u"),
             dt.Year(),
             dt.Month(),
             dt.Day());

  snprintf_P(waktux,
             countof(waktux),
             PSTR("%02u:%02u:%02u"),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );

  times = String(waktux);
  dates = String(tanggalx);
  dateTime = dates + " " + times;
}
