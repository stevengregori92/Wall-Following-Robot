#include <NewPing.h>
#include <AutoPID.h>
#include <tcs3200.h>

#define PIN_INPUT 3
#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 40  // Maximum distance (in cm) to ping.

// PIN MOTOR
#define ena 10
#define in1 8
#define in2 9
#define in3 6
#define in4 7
#define enb 5
#define threshold 10

#define OUTPUT_MIN -100
#define OUTPUT_MAX 100
#define KP 5
#define KI 0.1
#define KD 0

NewPing Sen_Kiri = NewPing(42, 44);
NewPing Sen_Kanan = NewPing(48, 46);
NewPing Sen_Tengah = NewPing(52, 50);

int red, not_red;
tcs3200 tcs(53, 51, 47, 45, 49);
int pwm_awal = 60;
int jkanan, jkiri, jdepan;

void motor(int kir, int kan) {
  if (kir > 0 && kan > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (kir < 0 && kan > 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (kir > 0 && kan < 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else if (kir < 0 && kan < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  analogWrite(ena, abs(kir));
  analogWrite(enb, abs(kan));
}


float kec_kanan, kec_kiri;
double jHarp = 11;
double input, _pid;
AutoPID myPID(&input, &jHarp, &_pid, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

unsigned long tstamp = 0;
bool jln = tr ue;


void setup() {
  // Measure the frequency
  Serial.begin(9600);
  myPID.setBangBang(15);
  myPID.setTimeStep(1);
}
void BSensor() {
  jkanan = Sen_Kanan.ping_cm();
  if (jkanan == 0 || jkanan >= 40) {
    jkanan = 40;
  }
  jkiri = Sen_Kiri.ping_cm();
  if (jkiri == 0 || jkiri >= 40) {
    jkiri = 40;
  }
  jdepan = Sen_Tengah.ping_cm();
  int dfront = Sen_Tengah.ping_cm();
  if (dfront != 0) {
    jdepan = dfront;
  } else if (jdepan == 0 || jdepan >= 40) {
    jdepan = 40;
  }

  not_red = tcs.colorRead('c');  //reads color value for not_red(clear)
  Serial.print("Color= ");
  Serial.print(not_red);
  Serial.print("    ");
  if (not_red < 60) {
    Serial.print("Color detected: RED ");
  } else {
    Serial.print("Color detected: NOT RED ");
  }

  Serial.println(String(jkiri) + " " + String(jdepan) + " " + String(jkanan));
}

void loop() {

  if (jln) {
    BSensor();

    input = jkiri;

    if (jdepan <= 15) {
      if (jkiri > 26) {
        motor(-pwm_awal - 30, pwm_awal + 30);
        delay(600);
      } else {
        motor(pwm_awal + 30, -pwm_awal - 30);
        delay(600);
      }
    } else if (not_red <= 60) {
      motor(0, 0);
    }

    else {
      myPID.run();
      motor(pwm_awal + _pid, pwm_awal - _pid);
      // Serial.println(String(kec_kiri) + " " + String(kec_kanan));
    }
  }
}
