#include <avr/io.h>
#include <avr/interrupt.h>

const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);

  Serial.begin(9600);

  // Start timer with a frequency of 0.5Hz (2 seconds period)
  startTimer(0.5);
}

void loop()
{
}

void startTimer(double timerFrequency)
{
  // Disable interrupts
  noInterrupts();

  // Clear Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;

  // Set Timer1 to CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << WGM12);

  // Calculate the compare match value for the given frequency
  // Calculation: OCR1A = (F_CPU / (2 * prescaler * timerFrequency)) - 1
  uint16_t compareValue = (uint16_t)(F_CPU / (2 * 1024 * timerFrequency)) - 1;

  // Set the compare match register
  OCR1A = compareValue;

  // Set prescaler to 1024 and start the timer
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Enable Timer1 compare match interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Enable interrupts
  interrupts();
}

ISR(TIMER1_COMPA_vect)
{
  // Toggle the LED pin
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
