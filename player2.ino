#include "Nintendo.h"
#include "Gamecube.h"

#define DEADZONE 10
#define KEY_COUNT 20

CGamecubeController GamecubeController1(7);
CGamecubeConsole GamecubeConsole1(8);

int32_t simple[KEY_COUNT] = {0};
int32_t key[KEY_COUNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
// 0 : a
// 1 : b
// 2 : x
// 3 : y
// 4 : start
// 5 : dleft
// 6 : dright
// 7 : ddown
// 8 : dup
// 9 : z
// 10 : left
// 11 : right
// 12 : sleft
// 13 : sright
// 14 : sdown
// 15 : sup
// 16 : cleft
// 17 : cright
// 18 : cdown
// 19 : cup

void Simplify(Gamecube_Report_t& report){
  simple[0] = report.a;
  simple[1] = report.b;
  simple[2] = report.x;
  simple[3] = report.y;
  simple[4] = report.start;
  
  simple[5] = report.dleft;
  simple[6] = report.dright;
  simple[7] = report.ddown;
  simple[8] = report.dup;

  simple[9] = report.z;
  
  simple[10] = report.left != 0 || report.l != 0;
  simple[11] = report.right != 0 || report.r != 0;
  
  simple[12] = report.xAxis < (128 - DEADZONE);
  simple[13] = report.xAxis > (128 + DEADZONE);
  simple[14] = report.yAxis < (128 - DEADZONE);
  simple[15] = report.yAxis > (128 + DEADZONE);
  
  simple[16] = report.cxAxis < (128 - DEADZONE);
  simple[17] = report.cxAxis > (128 + DEADZONE);
  simple[18] = report.cyAxis < (128 - DEADZONE);
  simple[19] = report.cyAxis > (128 + DEADZONE);
}

void Expand(Gamecube_Report_t& report){
  report.a = simple[0];
  report.b = simple[1];
  report.x = simple[2];
  report.y = simple[3];
  report.start = simple[4];
  report.dleft = simple[5];
  report.dright = simple[6];
  report.ddown = simple[7];
  report.dup = simple[8];
  report.z = simple[9];
  report.left = simple[10] * 255;
  report.right = simple[11] * 255;
  report.l = simple[10];
  report.r = simple[11];
  report.xAxis = 128 + 127 * (simple[13] - simple[12]);
  report.yAxis = 128 + 127 * (simple[15] - simple[14]);
  report.cxAxis = 128 + 127 * (simple[17] - simple[16]);
  report.cyAxis = 128 + 127 * (simple[19] - simple[18]);
}

void ShuffleKeys() {
  for (int i=0; i < KEY_COUNT; i++) {
     int n = random(0, KEY_COUNT);
     int temp = key[n];
     key[n] =  key[i];
     key[i] = temp;
  }
}

void UseKey(){
  int temp[KEY_COUNT] = {0};
  for (int i=0; i < KEY_COUNT; i++) {
     temp[i] = simple[key[i]];
  }
  for (int i=0; i < KEY_COUNT; i++) {
     simple[i] = temp[i];
  }
}

void setup()
{
  randomSeed(analogRead(0));
  ShuffleKeys();
  ShuffleKeys();
  ShuffleKeys();
}


void loop()
{
  if (GamecubeController1.read())
  {
    auto report = GamecubeController1.getReport();
    Simplify(report);
    UseKey();
    Expand(report);
    if (!GamecubeConsole1.write(report))
    {
    }
  }
}
