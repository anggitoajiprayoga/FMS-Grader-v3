void setup_ads() {
  if (!ads.begin(ads_address)) {
    Serial.println("Failed to initialize ADS.");
  }
}

void loop_ads() {
  adc_altenator = ads.readADC_SingleEnded(0);
  adc_parking = ads.readADC_SingleEnded(1);
  adc_netral = ads.readADC_SingleEnded(2);
  adc_fuel = ads.readADC_SingleEnded(3);

  if (adc_altenator > setpoin_altenator) {
    altenator = 1;
  } else {
    altenator = 0;
  }

  if (adc_parking > setpoin_parking_break) {
    parking_break = 1;
  } else {
    parking_break = 0;
  }

  if (adc_netral > setpoin_netral) {
    netral = 1;
  } else {
    netral = 0;
  }

  fuel = adc_fuel;
}
