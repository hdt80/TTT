int val;
float scaled;
int res;

void printvals(int v, float s, int r) {
	Serial.print(v);
	Serial.print(" ");
	Serial.print(s);
	Serial.print(" ");
	Serial.println(r);
}

void setup() {
	pinMode(A0, INPUT);

	Serial.begin(9600);
}

void loop() {
	pinMode(12, OUTPUT);
	pinMode(11, INPUT); // Set to Hi-Z, hardly changes the output
	digitalWrite(12, HIGH);
	delay(10);
	val = analogRead(A0);
	digitalWrite(12, LOW);

	scaled = ((float) val / 1024.0) * 5.0;
	res = 4700.0 * ((5.0 - scaled) / (scaled));
	Serial.print("R0: ");
	printvals(val, scaled, res);

	pinMode(12, INPUT);
	pinMode(11, OUTPUT);
	digitalWrite(11, HIGH);
	delay(10);
	val = analogRead(A0);
	digitalWrite(11, LOW);

	scaled = ((float) val / 1024.0) * 5.0;
	res = 4700.0 * ((5.0 - scaled) / (scaled));
	Serial.print("R1: ");
	printvals(val, scaled, res);

	delay(1000);
}
