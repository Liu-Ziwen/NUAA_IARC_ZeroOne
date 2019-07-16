/*  @file    main.hpp
 *  @project NUAA_IARC
 *  @author  ZiwenLIU
 *  @date    July-02-2019
 * 
 *  @brief
 */

#ifndef __MAIN_HPP_
#define __MAIN_HPP_

/* ================ INCLUDES ================ */
/* System Includes */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>  /* 提供类型 pid_t 的定义 */
#include <sys/wait.h>
#include <sys/shm.h>    /* 使用共享内存接口函数 */
#include <sys/ipc.h>
#include <unistd.h>
#include <cmath>

/* DJI OSDK Includes */
#include <dji_vehicle.hpp>
#include "dji_status.hpp"
#include <dji_linux_helpers.hpp>//../Onboard-SDK-3.8/sample/linux/common/

/* user includes */
#include "flight_control_sample.hpp"
#include "voice_model.hpp"
#include "usr_time_stamp.hpp"


/* ================ DEFINES ================= */
// #define FALSE -1
// #define TRUE 0
#define TAKEOFF     0x01
#define LANDING     0x02

/* ================ NAMESPACE =============== */
//Please avoid using " using namespace XXX " in a headfile





#endif // !__TTY_CONFIG_H_