// LDR parameters
const int LDRpin = A5;
const float m = -0.3869;
const float C = 4.874;
float lux = 0;

// Led parameters
const int led = 5;
int ledIntensity = 255; // [0, 255]

// Button that defines if room is occupied
int button = 0;

// Room lighting values
const int minValue = 200;
const int maxValue = 500;
const int minValueEmpty = 100;
const int maxValueEmpty = 200;

// Regra de 3 simples
double mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // Initial setup
  Serial.begin(9600);
  pinMode(LDRpin, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {

  // Get lux value
  int LDR = analogRead(LDRpin);
  float Vout = mapfloat(LDR, 0, 1023, 0, 5);
  int RLDR = 10000*(5-Vout)/Vout;
  lux = C * pow(( (5 / Vout) - 1), (1/m));

  // Print LDR measured values to console
  Serial.print("Tensão medida: ");
  Serial.println(Vout);
  Serial.print("Lux medidos: ");
  Serial.println(lux);
  Serial.println("\n");

  // Change led brightness value depending on lux value
  // If room is empty
  if(!button){
    if(lux > maxValueEmpty){
      ledIntensity -= 15;
    }
    else if(lux < minValueEmpty){
      ledIntensity += 15;
    }
  }
  //If room is occupied
  else{
    if(lux > maxValue){
      ledIntensity -= 15;
    }
    else if(lux < minValue){
      ledIntensity += 15;
    }
  }

  analogWrite(led, ledIntensity);
  
  delay(500);
}
