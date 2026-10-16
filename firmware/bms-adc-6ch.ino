constexpr uint8_t NUM_CH = 6;

const uint8_t CH_PIN[NUM_CH] = { A0, A1, A2, A3, A4, A5 };

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  for (uint8_t i = 0; i < NUM_CH; i++) {
    pinMode(CH_PIN[i], INPUT_ANALOG);
  }
  Serial.println("BMS ADC test: 6 channels");
}

void loop() {
  int v[NUM_CH];
  for (uint8_t i = 0; i < NUM_CH; i++) v[i] = analogRead(CH_PIN[i]);

  char buf[80];
  snprintf(buf, sizeof(buf), "C1=%d C2=%d C3=%d C4=%d T=%d I=%d",
           v[0], v[1], v[2], v[3], v[4], v[5]);
  Serial.println(buf);
  delay(200);
}