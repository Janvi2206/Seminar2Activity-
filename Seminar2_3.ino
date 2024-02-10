#include <avr/io.h>
#include <avr/interrupt.h>

const byte LED_PIN = 13;
const byte POTENTIOMETER_PIN = A4;

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  // Start serial communication
  Serial.begin(9600);

  // Initialize Timer with initial frequency
  startTimer(1); // Initial frequency set to 1Hz
}

void loop()
{
  // Read the analog value from the potentiometer
  int potValue = analogRead(POTENTIOMETER_PIN);

  // Map the potentiometer value (0-1023) to the desired frequency range (e.g., 0.1 to 10 Hz)
  double timerFrequency = map(potValue, 0, 1023, 1, 10); // Adjust range as needed

  // Print the selected frequency to Serial Monitor
  Serial.print("Selected Frequency: ");
  Serial.print(timerFrequency);
  Serial.println(" Hz");

  // Restart Timer with the new frequency
  startTimer(timerFrequency);

  // Add a delay to avoid reading the potentiometer too frequently
  delay(1000);
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
  unsigned long compareValue = (F_CPU / (2UL * 1024UL * timerFrequency)) - 1;

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
