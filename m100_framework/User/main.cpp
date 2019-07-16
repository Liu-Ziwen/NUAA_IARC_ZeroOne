/*! @file flight-control/main.cpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  main for Flight Control API usage in a Linux environment.
 *  Provides a number of helpful additions to core API calls,
 *  especially for position control, attitude control, takeoff,
 *  landing.
 * 
 *  修改于 DJI-Onboard-SDK-3.8 中的 flight-control 例程
 *
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <iostream>
#include "main.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

#define PERM  S_IRUSR|S_IWUSR|IPC_CREAT

uint8_t loop_timer_400hz = 0;
uint8_t loop_timer_200hz = 0;
uint8_t loop_timer_100hz = 0;
uint8_t loop_timer_50hz = 0;
uint8_t loop_timer_10hz = 0;
uint8_t loop_timer_5hz = 0;
uint8_t loop_timer_2hz = 0;
uint16_t loop_timer_1hz = 0;
long timer_2p5ms = 0;


/*! main
 *
 */
int
main(int argc, char** argv)
{
  // Initialize variables
  int shmid;    //共享内存的ID
  uint8_t *p_addr, *c_addr; //父进程和子进程的共享内存变量

  int functionTimeout = 1;

  // Setup OSDK. // m100 Onboard_SDK 识别飞机型号及串口型号等初始化
  LinuxSetup linuxEnvironment(argc, argv);
  Vehicle*   vehicle = linuxEnvironment.getVehicle();
  if (vehicle == NULL)
  {
    std::cout << "Vehicle not initialized, exiting.\n";
    return -1;
  }

  // Obtain Control Authority
  vehicle->obtainCtrlAuthority(functionTimeout);

  // Setup Voice Model // 初始化语音模块
  voice_model_init();


  // 申请一段共享内存 share memory 用于两个线程之间的交互
  shmid = shmget(IPC_PRIVATE, 1024, PERM);
  if(shmid == -1)
  {
    std::cout << "Create Share Memory Error: " << strerror(errno) << std::endl;
    exit(1);
  }

  // fork函数调用测试
  pid_t result;
  result = fork();
  if(-1 == result)      //返回值为-1则出错
    std::cout << "fork error! \n";

  else if(0 == result)  //返回值为0则为子进程(循环判断到来的指令)
  {
    c_addr = (uint8_t*)shmat(shmid, 0, 0);
    *c_addr = (uint8_t)0x08;
    uint8_t i=0;
    std::cout << "child process! result = " << result << ", pid = " << getpid() << std::endl;
    std::cout << (int)*c_addr << " | " << (int*)c_addr << std::endl;
    
    while(1)
    {
      voice_cmd_read_anl();

      if(voice_cmd_buff[2]==0x01 && voice_cmd_buff[3]==0x01)
        *c_addr = 0x01;
      if(voice_cmd_buff[2]==0x01 && voice_cmd_buff[3]==0x02)
        *c_addr = 0x02;
      


      if(micros() - timer_2p5ms >= 2500)
      {
        timer_2p5ms = micros();
        loop_timer_400hz++;
        loop_timer_200hz++;
        loop_timer_100hz++;
        loop_timer_50hz++;
        loop_timer_10hz++;
        loop_timer_5hz++;
        loop_timer_2hz++;
        loop_timer_1hz++;
      }

      if(loop_timer_400hz >= 1)
      {
        loop_timer_400hz = 0;
      }

      if(loop_timer_200hz >= 2)
      {
        loop_timer_200hz = 0;
      }

      if(loop_timer_100hz >= 4)
      {
        loop_timer_100hz = 0;
      }

      if(loop_timer_50hz >= 8)
      {
        loop_timer_50hz = 0;
      }

      if(loop_timer_10hz >= 40)
      {
        loop_timer_10hz = 0;
      }

      if(loop_timer_5hz >= 80)
      {
        loop_timer_5hz = 0;
      }

      if(loop_timer_2hz >= 200)
      {
        loop_timer_2hz = 0;
      }

      if(loop_timer_1hz >= 400)
      {
        loop_timer_1hz = 0;
        static float count_1hz = 1;
        std::cout << "child process : " << count_1hz << std::endl;
        if(count_1hz++ >= 4 && *c_addr == 0x08)
        {
          count_1hz = 0;
        }
      }

    }
      
    
  }
  else                  //返回值大于0代表父进程(主进程：控制进程)
  {
    p_addr = (uint8_t*)shmat(shmid, 0, 0);
    std::cout << "father process! result = " << result << ", pid = " << getpid() << std::endl;
    std::cout << (int)*p_addr << " | " << (int*)p_addr << std::endl;

    while(1)
    {
      sleep(1);

      if(*p_addr == TAKEOFF)
      {
        std::cout << " start to excute flight control command!  | Take Off !\n";
        monitoredTakeoff(vehicle);
        *p_addr = 0x00;
      }

      if(*p_addr == LANDING)
      {
        std::cout << " start to excute flight control command!  | Landing !\n";
        monitoredLanding(vehicle);
        *p_addr = 0x00;
      }
    }

    
  }
  
  return 0;
}













