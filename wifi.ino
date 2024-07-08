void setup_wifi() {
  String APssid2 = cn;
  WiFi.mode(WIFI_AP); 
  WiFi.softAP(APssid2.c_str(), ""); 
  server.begin(); 
  server_begin(); 
}

void loop_wifi() {
  static unsigned long lastTime = 0;
  static bool simbol = false; 
  static bool state = false;

  if ((millis() - lastTime) > 500) {
    String get_payload = payload(); 
    //    Serial.println("//web_socket_payload_send");
    //    Serial.println(get_payload);
    notifyClients(get_payload);
    state = !state; 
    lastTime = millis(); 
  }

  simbol = state;
  if (state) {
    ind = " | ";
  } else {
    ind = " - ";
  }
  ws.cleanupClients(); 
}
