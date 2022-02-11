#include <Servo.h>
#include <SoftwareSerial.h>
Servo wheel_servo; //前輪
Servo grab_servo;  //アーム(掴む)
Servo tilt_servo;  //アーム(上下に傾ける)
const int Tx = 2;
const int Rx = 3;
SoftwareSerial bluetooth(Tx, Rx);

//ピンを定義
const int AIN1 = 9;
const int AIN2 = 10;
const int BIN1 = 8;
const int BIN2 = 7;
const int PWMA = 11;
const int PWMB = 6;
const int WheelPin = 13;
const int GrabPin = 5;
const int TiltPin = 4;

//コマンドキーの定義
const char AUTO_MODE = '!';
const char W = 'w'; //前進
const char S = 's'; //後進
const char X = 'x'; //ブレーキ
const char A = 'a'; //左にサーボモータ(前輪)を回転
const char D = 'd'; //右にサーボモータ(前輪)を回転
const char N = 'n'; //サーボモータ(前輪)の角度を0に戻す
const char GRAB = 'g'; //掴む
const char RELEASE = 'r'; //放す
const char UP = 'i'; //アームを上げる
const char DOWN = 'k'; //アームを下げる

char command;

//スピードの定義
const int SPEED_FORWARD = 100;
const int SPEED_BACK = 100;

//角度の定義
int wheel_angle = 90; //範囲は 0 ~ 180
int grab_angle = 0;
int release_angle = 180;
int up_angle = 180;
int down_angle = 0;

void setup() {
  bluetooth.begin(115200);
  wheel_servo.attach(WheelPin);
  wheel_servo.write(wheel_angle);
  grab_servo.attach(GrabPin);
  grab_servo.write(grab_angle);
  tilt_servo.attach(TiltPin);
  tilt_servo.write(down_angle);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);
}

void loop() {
  if (bluetooth.available() > 0) {
    command = bluetooth.read();
    bluetooth.flush();
    if (command == A) {
      // 左にサーボモータ(前輪)を回転
      rotate_left();
    } else if (command == D) {
      // 左にサーボモータ(前輪)を回転
      rotate_right();
    } else if (command == N) {
      // ニュートラル
      wheel_neutral();
    } else if (command == W) {
      // 前進
      forward(SPEED_FORWARD, SPEED_FORWARD);
    } else if (command == S) {
      // 後進
      back(SPEED_BACK, SPEED_BACK);
    } else if (command == X) {
      // ブレーキ
      brake();
    } else if (command == GRAB) {
      // 掴む
      grab();
    } else if (command == RELEASE) {
      // 放す
      release();
    } else if (command == UP) {
      // アームを上に傾ける
      raise();
    } else if (command == DOWN) {
      // アームを下に傾ける
      lower();
    }
  } 
}

//直進する
void forward(int right, int left) {
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
  analogWrite(PWMA,right);
  analogWrite(PWMB,left);
}

//バックする
void back(int right, int left) {
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
  analogWrite(PWMA,right);
  analogWrite(PWMB,left);
}

//ブレーキ
void brake() {
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,LOW);
}

//前輪を左に回転
void rotate_left() {
  if (is_valid_rotate(wheel_angle)) {
    wheel_angle -= 10;
    wheel_servo.write(wheel_angle);
  }
}

//前輪を右に回転
void rotate_right() {
  if (is_valid_rotate(wheel_angle)) {
    wheel_angle += 10;
    wheel_servo.write(wheel_angle);
  }
}

//現在の前輪の角度が0~180であるかを判定
bool is_valid_rotate(int angle) {
  return (angle < 180 && angle > 0);
}

//前輪の角度を0に戻す
void wheel_neutral() {
  wheel_servo.write(0);
}

//掴む
void grab() {
  grab_servo.write(grab_angle);
}

//放す
void release() {
  grab_servo.write(release_angle);
}

//アームの角度を上昇させる
void raise() {
  tilt_servo.write(up_angle);
}

//アームの角度を下降させる
void lower() {
  tilt_servo.write(down_angle);
}
