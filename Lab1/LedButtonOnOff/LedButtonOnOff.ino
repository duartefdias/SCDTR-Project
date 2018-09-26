const int button = 3 // Button on digital pin 3
const int led = 8 // Button on digital pin 8 

// Variable thar defines is button is being pressed or not
buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  // Read state of button
  buttonState = digitalRead(button);

  // Turn on led if button is pressed
  if(buttonState == HIGH) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  
  delay(100);
}
