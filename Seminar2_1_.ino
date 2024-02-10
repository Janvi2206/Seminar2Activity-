#include <avr/io.h>
#include <avr/interrupt.h>

const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);

  Serial.begin(9600);

  startTimer();
}

void loop()
{ 
}

void startTimer(){
  // Disable interrupts
  noInterrupts();

  // Clear Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;

  // Set Timer1 to CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << WGM12);

  // Set prescaler to 1024 and start the timer
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Set compare match register for 1Hz (1 second) increments
  // Calculation: (F_CPU / (prescaler * desired frequency)) - 1
  OCR1A = 15624;

  // Enable Timer1 compare match interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Enable interrupts
  interrupts();
}

ISR(TIMER1_COMPA_vect){
  // Toggle the LED pin
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
