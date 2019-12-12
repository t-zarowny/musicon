#include "Smoothing.h"

Smoothing30::Smoothing30(){
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void Smoothing30::add(int input){
  total = total - readings[readIndex];
  readings[readIndex] = input;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings; 
}

int Smoothing30::getVal(){
    return average;
}

Smoothing50::Smoothing50(){
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void Smoothing50::add(int input){
  total = total - readings[readIndex];
  readings[readIndex] = input;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings; 
}

int Smoothing50::getVal(){
    return average;
}

Smoothing200::Smoothing200(){
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void Smoothing200::add(int input){
  total = total - readings[readIndex];
  readings[readIndex] = input;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings; 
}

int Smoothing200::getVal(){
    return average;
}