int val;
float scaled;
int res;

int val2;
float scaled2;
int res2;

void printvals(int v, float s, int r) {
	Serial.print(v);
	Serial.print(" ");
	Serial.print(s);
	Serial.print(" ");
	Serial.println(r);
}

float scale(float val, float base) {
	return (val / 1024.0) * base;
}

float result(float scaled, float base, float resistor) {
	return resistor * ((base - scaled) / scaled);
}

void setup() {
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);

	Serial.begin(9600);
}

void loop() {

	pinMode(12, OUTPUT);
	pinMode(11, INPUT); // Set to Hi-Z, hardly changes the output

	digitalWrite(12, HIGH);
	delay(10);

	val = analogRead(A0);
	val2 = analogRead(A1);

	digitalWrite(12, LOW);

	scaled = scale(val, 5);
	scaled2 = scale(val2, 5);

	//res = 4700.0 * ((5.0 - scaled) / (scaled));
	res = result(scaled, 5.0, 4700.0);
	res2 = result(scaled2, 5.0, 4700.0);
	//Serial.print("R0: ");
	//printvals(val, scaled, res);

	Serial.print(scaled);
	Serial.print(" ");
	Serial.println(scaled2);

	pinMode(12, INPUT);
	pinMode(11, OUTPUT);

	digitalWrite(11, HIGH);
	delay(10);

	val = analogRead(A0);
	val2 = analogRead(A1);

	digitalWrite(11, LOW);

	scaled = scale(val, 5);
	scaled2 = scale(val2, 5);

	res = result(scaled, 5.0, 4700.0);
	res2 = result(scaled2, 5.0, 4700.0);

	Serial.print(scaled);
	Serial.print(" ");
	Serial.println(scaled2);

	Serial.println("========================");

	//Serial.print("R1: ");
	//printvals(val, scaled, res);

	delay(1000);
}
