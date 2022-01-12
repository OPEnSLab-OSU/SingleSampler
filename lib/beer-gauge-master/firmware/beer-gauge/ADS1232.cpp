/*
  ADS1232.cpp - Library for reading from a ADS1232 24-bit ADC.
  Created by Jeffrey M. Kubascik, June 28, 2016.
  Released into the public domain.
*/

#include "ADS1232.h"

ADS1232::ADS1232(int pin_cs, int pin_dout, int pin_pdwn) :
  _pin_cs(pin_cs),
  _pin_dout(pin_dout),
  _pin_pdwn(pin_pdwn),
  spi_settings(2000000, MSBFIRST, SPI_MODE1)
{
  return;
}

void ADS1232::init(Gain gain, Speed speed, Channel channel)
{
  pinMode(_pin_cs, OUTPUT);
  pinMode(_pin_pdwn, OUTPUT);

  disable();

  delay(1);
  enable();
  
  return;
}

void ADS1232::enable(void)
{
  digitalWrite(_pin_pdwn, HIGH);
  
  return;
}

void ADS1232::disable(void)
{
  digitalWrite(_pin_pdwn, LOW);
  
  return;
}

bool ADS1232::dataReady(void)
{
  return digitalRead(_pin_dout) == LOW;
}

int32_t ADS1232::read(void)
{
  int32_t data = 0;

  while(dataReady() == false);

  SPI.beginTransaction(spi_settings);
  digitalWrite(_pin_cs, LOW);
  
  data |= (uint32_t)SPI.transfer16(0) << 16;
  data |= (uint32_t)SPI.transfer16(0) <<  0;

  digitalWrite(_pin_cs, HIGH);
  SPI.endTransaction();

  return data;
}

