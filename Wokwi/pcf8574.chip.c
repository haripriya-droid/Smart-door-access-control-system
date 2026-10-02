#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  pin_t p[8];
  uint8_t latch;
} chip_state_t;


// Arduino writes to PCF8574
bool on_i2c_write(void *user_data, uint8_t data) {
  chip_state_t *chip = (chip_state_t *)user_data;

  chip->latch = data;

  for (int i = 0; i < 8; i++) {
  uint8_t bit = (data >> i) & 1;

  if (bit == 0) {
    // Write 0 → actively pull the pin LOW
    pin_mode(chip->p[i], OUTPUT);
    pin_write(chip->p[i], LOW);
  } 
  else {
    // Write 1 → release the pin so it can be pulled LOW externally
    pin_mode(chip->p[i], INPUT_PULLUP);
  }
  }

  return true;
}


// Arduino reads from PCF8574
uint8_t on_i2c_read(void *user_data) {
  chip_state_t *chip = (chip_state_t *)user_data;

  uint8_t value = 0;

  for (int i = 0; i < 8; i++) {
    if (pin_read(chip->p[i]) == HIGH) {
      value |= (1 << i);
    }
  }

  return value;
}


// Arduino addresses the PCF8574
bool on_i2c_connect(void *user_data, uint32_t address, bool read) {
  return address == 0x20;
}


void chip_init() {

  chip_state_t *chip = malloc(sizeof(chip_state_t));

  for (int i = 0; i < 8; i++) {
    char name[3];

    snprintf(name, sizeof(name), "P%d", i);

    chip->p[i] = pin_init(name, INPUT_PULLUP);
  }

  chip->latch = 0xFF;

  i2c_config_t config = {
    .address = 0x20,
    .scl = pin_init("SCL", INPUT_PULLUP),
    .sda = pin_init("SDA", INPUT_PULLUP),
    .connect = on_i2c_connect,
    .read = on_i2c_read,
    .write = on_i2c_write,
    .disconnect = NULL,
    .user_data = chip,
  };

  i2c_init(&config);

  printf("PCF8574 initialized at 0x20\n");
}