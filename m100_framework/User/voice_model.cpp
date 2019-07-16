/*  @file    voice_model.cpp
 *  @project NUAA_IARC
 *  @author  ZiwenLIU
 *  @date    July-11-2019
 * 
 *  @brief
 */


#include <iostream>
#include "voice_model.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;



uint8_t serial_read_buff[512];
uint8_t voice_cmd_buff[VOICE_CMD_SIZE];
const char* voice_model_dev = "/dev/ttyUSB0";
uint32_t voice_model_baudrate = 57600;
LinuxSerialDevice dji_osdk_serial_device(voice_model_dev,voice_model_baudrate);
//uint8_t byte_cout = 0;


/*! voice_model_init
 *
 * 
*/
bool voice_model_init(void)
{
  //
  // dji_osdk_serial_device = LinuxSerialDevice(voice_model_dev,voice_model_baudrate);
  dji_osdk_serial_device.init();
  if(dji_osdk_serial_device.getDeviceStatus() == false)
      return false;
  else return true;
}



/*! voice_model_init
 *
 * 
*/
uint8_t voice_cmd_read_anl(void)
{
  uint8_t byte_count = dji_osdk_serial_device.readall(serial_read_buff,512);
  if(byte_count == VOICE_CMD_SIZE)
  {
    uint8_t j=0;
    if(serial_read_buff[0]==0xFA && serial_read_buff[1]==0xFB)
    {
      for(j=0;j<VOICE_CMD_SIZE;j++)
      {
        voice_cmd_buff[j] = serial_read_buff[j];
      }

      //校验位计算
      voice_cmd_buff[4] = voice_cmd_buff[2] + voice_cmd_buff[3];

      //若校验不通过则清空缓冲区
      if(voice_cmd_buff[4] != serial_read_buff[4])
      {
        for(j=0;j<VOICE_CMD_SIZE;j++)
        {
          voice_cmd_buff[j] = 0;
        }
      }
    }
  }

  return 0;

}












