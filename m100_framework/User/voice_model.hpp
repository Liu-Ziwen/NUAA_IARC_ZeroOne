/*  @file    voice_model.hpp
 *  @project NUAA_IARC
 *  @author  ZiwenLIU
 *  @date    July-11-2019
 * 
 *  @brief
 */

#ifndef __VOICE_MODEL_HPP_
#define __VOICE_MODEL_HPP_

#include <dji_vehicle.hpp>
#define VOICE_CMD_SIZE 5    //the size of voice command buffer


bool voice_model_init(void);
uint8_t voice_cmd_read_anl(void);

extern uint8_t voice_cmd_buff[VOICE_CMD_SIZE];





#endif // !__VOICD_MODEL_HPP_


