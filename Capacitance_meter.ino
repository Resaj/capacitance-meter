/*
This program reads the capacitance of a capacitor using an RC low-pass circuit.

First of all, you need to select a resistor value "R_value".

Note: If the system takes a lot for showing the capacitor value, choose a lower R_value.
*/

const int ctrl = 4;
const int pin_adc = A0;
int R_value = 660; // [KOhms] Change this value to another higher if the capacitance is too small.
int th = 0;
int tao = 0;
float capacitance = 0;

void calculate_capacitance(void);
void print_capacitance(void);

void setup() {
  pinMode(ctrl, OUTPUT);
  digitalWrite(ctrl, HIGH);
  Serial.begin(9600);
  th = 1023 * 0.63; // This is the threshold which lets determining the time constant, tao.
                    // It's the 63% of the initial and final voltage levels: 5V
                    // Once tao is captured, we have to calculate the capacitance with the next formule: tao = R*C
  delay(1000);  
}

void loop() {
  unsigned long t = 0;

  t = millis();
  digitalWrite(ctrl, LOW);
  while(analogRead(pin_adc) > 1023-th);
  tao = millis() - t;
  digitalWrite(ctrl, HIGH);
  
  if(tao < 10)
  {
    Serial.println("Capacitance is too small. Change the circuit and code with a higher resistor value.");
    tao = 250;
  }
  else
  {
    calculate_capacitance();
    Serial.print("tao = ");
    Serial.print(tao);
    Serial.print(" ms\t");
    print_capacitance();
  }

  delay(tao*10);
}

void calculate_capacitance(void)
{
  capacitance = tao * 1e6 / R_value; // units: pF = ms * 1000000 / KOhms
}

void print_capacitance(void)
{
  Serial.print("capacitance = ");
  
  if(capacitance >= 1000000)
  {
    Serial.print(capacitance/1000000);
    Serial.println(" uF");
  }
  else if(capacitance >= 1000)
  {
    Serial.print(capacitance/1000);
    Serial.println(" nF");
  }
  else
  {
    Serial.print(capacitance);
    Serial.println(" pF");
  }
}
