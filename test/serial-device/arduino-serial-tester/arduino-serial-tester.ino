#include "sdtproto.hpp"

#define SAMPLES_COUNT 10

sdt::frame frame;
sdt::complex_uint16_t samples[SAMPLES_COUNT];

void setup() {
  Serial.begin(9600);

  frame.header = sdt::SDT_HEADER;
  frame.length = SAMPLES_COUNT;
  frame.samples = samples;
}

void loop() {
  for (int i = 0; i < SAMPLES_COUNT; i++) {
    samples[i].real = analogRead(A0);
    samples[i].imag = 0;
  }

  Serial.write(reinterpret_cast<uint8_t *>(&frame.header), sizeof(uint16_t));
  Serial.write(reinterpret_cast<uint8_t *>(&frame.length), sizeof(uint16_t));
  Serial.write(reinterpret_cast<uint8_t *>(frame.samples), sizeof(sdt::complex_uint16_t) * SAMPLES_COUNT);

  for (int i = 0; i < 16; i++)
    Serial.write(0xFF);

  delay(100);
}
