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
#include "flight_control_sample.hpp"


using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;
using namespace std;

#define VOICE_CMD_SIZE 5 //语音指令缓冲区大小

/*! main
 *
 */
int
main(int argc, char** argv)
{
  // Initialize variables
  int functionTimeout = 1;

  // 定义语音模块串口号及接收变量
  const char* dev = (const char*) "/dev/ttyUSB2";
  uint8_t serial_read_buff[512];
  uint8_t voice_command_buff[VOICE_CMD_SIZE];
  uint8_t byte_count = 0;

  // Setup OSDK.
  LinuxSetup linuxEnvironment(argc, argv);
  Vehicle*   vehicle = linuxEnvironment.getVehicle();
  if (vehicle == NULL)
  {
    std::cout << "Vehicle not initialized, exiting.\n";
    return -1;
  }

  // Obtain Control Authority
  vehicle->obtainCtrlAuthority(functionTimeout);

  // 初始化语音模块串口
  LinuxSerialDevice dji_osdk_serial_device(dev,9600);
  dji_osdk_serial_device.init();
  if(dji_osdk_serial_device.getDeviceStatus() == false)
    return -1;

  // 循环接收语音指令
  while (1)
  {
    
    byte_count = dji_osdk_serial_device.readall(serial_read_buff, 512);

    if(byte_count == VOICE_CMD_SIZE)
    {
      uint8_t j=0;
      if(serial_read_buff[0]==0xFA && serial_read_buff[1]==0xFB)
      {
        for(j=0;j<VOICE_CMD_SIZE;j++)
        {
          voice_command_buff[j] = serial_read_buff[j];
        }

        //校验位计算
        voice_command_buff[4] = voice_command_buff[2] + voice_command_buff[3];

        //若校验不通过则清空缓冲区
        if(voice_command_buff[4] != serial_read_buff[4])
        {
          for(j=0;j<VOICE_CMD_SIZE;j++)
          {
            voice_command_buff[j] = 0;
          }
        }
      }
    }

    if(voice_command_buff[2]==0x01 && voice_command_buff[3]==0x01)
    {
      cout << "Take Off !\n";
      monitoredTakeoff(vehicle);
    }
    
    if(voice_command_buff[2]==0x01 && voice_command_buff[3]==0x02)
    {
      cout << "Land !\n";
      monitoredLanding(vehicle);
    }
  }
  // Display interactive prompt
  // std::cout
  //   << "| Available commands:                                            |"
  //   << std::endl;
  // std::cout
  //   << "| [a] Monitored Takeoff + Landing                                |"
  //   << std::endl;
  // std::cout
  //   << "| [b] Monitored Takeoff + Position Control + Landing             |"
  //   << std::endl;
  // char inputChar;
  // std::cin >> inputChar;

  // switch (inputChar)
  // {
  //   case 'a':
  //     monitoredTakeoff(vehicle);
  //     monitoredLanding(vehicle);
  //     break;
  //   case 'b':
  //     monitoredTakeoff(vehicle);
  //     moveByPositionOffset(vehicle, 0, 6, 6, 30);
  //     moveByPositionOffset(vehicle, 6, 0, -3, -30);
  //     moveByPositionOffset(vehicle, -6, -6, 0, 0);
  //     monitoredLanding(vehicle);
  //     break;
  //   default:
  //     break;
  // }

  return 0;
}




// int main(void)
// {
//   const char* dev = (const char*) "/dev/ttyUSB0";
//   //const char* file = (const char*) "/home/ubuntu/m100_framework/test.cpp";
//   // uint32_t baudrate = 9600;
//   uint8_t serial_read_buff[512];
//   uint8_t voice_command_buff[VOICE_CMD_SIZE];

//   LinuxSerialDevice dji_osdk_serial_device(dev,9600);
//   dji_osdk_serial_device.init();
//   if(dji_osdk_serial_device.getDeviceStatus() == false)
//     return -1;

//   while (1)
//   {
    
//     uint8_t byte_count = dji_osdk_serial_device.readall(serial_read_buff, 512);

//     // if(byte_count > 0)
//     // {
//     //   //int fd = open(file, O_RDWR);
//     //   if(/**/(serial_read_buff[0]=0xFA)  &&  
//     //       (serial_read_buff[1]=0xFB)   /*&&
//     //       (serial_read_buff[2]=0x64)   &&
//     //       (serial_read_buff[3]=0x00)   &&
//     //       (serial_read_buff[4]=0x02) &&
//     //       (serial_read_buff[5]=0xFD) */ )
//     //       cout << "YES !\n";
//     //   for (uint8_t i = 0; i < byte_count; i++)
//     //   {
//     //     /* code */
//     //     //cout << hex << byte_count << hex << serial_read_buff[i] << endl;
        
//     //     //serial_write_buff[i] = serial_read_buff[i];
//     //   }
//     //   //write(fd, serial_write_buff, byte_count);
//     //   //close(fd);
//     // }

//     if(byte_count == VOICE_CMD_SIZE)
//     {
//       uint8_t j=0;
//       if(serial_read_buff[0]==0xFA && serial_read_buff[1]==0xFB)
//       {
//         for(j=0;j<VOICE_CMD_SIZE;j++)
//         {
//           voice_command_buff[j] = serial_read_buff[j];
//         }

//         //校验位计算
//         voice_command_buff[4] = voice_command_buff[2] + voice_command_buff[3];

//         //若校验不通过则清空缓冲区
//         if(voice_command_buff[4] != serial_read_buff[4])
//         {
//           for(j=0;j<VOICE_CMD_SIZE;j++)
//           {
//             voice_command_buff[j] = 0;
//           }
//         }
//       }
//     }

//     if(voice_command_buff[2]==0x01 && voice_command_buff[3]==0x01)
//     {
//       cout << "Take Off !\n";
//     }
    
//     if(voice_command_buff[2]==0x01 && voice_command_buff[3]==0x02)
//     {
//       cout << "Land !\n";
//     }
//   }
  


// }














