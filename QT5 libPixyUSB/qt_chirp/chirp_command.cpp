//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

//#include <signal.h>
#include <unistd.h>
#include "libpixyusb2.h"

#define  COLOR_STEP_VALUE               5

Pixy2        pixy;
static bool  run_flag = true;


//void handle_SIGINT(int unused)
//{
//  // On CTRL+C - abort! //
//
//  run_flag = false;
//}

int main()
{
  int  Result;

  // Catch CTRL+C (SIGINT) signals //
  //signal (SIGINT, handle_SIGINT);

  printf ("=============================================================\n");
  printf ("= PIXY2 Chirp Command Demo                                  =\n");
  printf ("=============================================================\n");

  printf ("Connecting to Pixy2...");

  // Initialize Pixy2 Connection //
  {
    Result = pixy.init();

    if (Result < 0) {
        printf ("Error\n");
        printf ("pixy.init() returned %d\n", Result);
        return Result;
        }
    printf ("Success\n");
        }



    // Get Pixy2 Version information //
    Result = pixy.getVersion();
    if (Result < 0) {
        printf ("pixy.getVersion() returned %d\n", Result);
        return Result;
        }
    pixy.version->print();


    // Get F/W H/W Version---------------------
    uint16_t  *pixy_version;
    uint16_t  v[6];
    uint32_t  version_length;
    uint32_t  response;
    int iReturn = pixy.m_link.callChirp("version", END_OUT_ARGS, &response, &version_length, &pixy_version,  END_IN_ARGS);
    if (iReturn >= 0) {
        memcpy((void *)v,  pixy_version, 6 * sizeof(uint16_t));
        printf("Version F/W:%u.%u.%u  H/W:%u.%u.%u\n", v[0],v[1],v[2], v[3],v[4],v[5]);
        }

    //Set Lamp 1,1
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(1), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);
    //Set Lamp 0,1
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);


  printf ("PIXY2 Chirp Command Demo Exit\n");
}

