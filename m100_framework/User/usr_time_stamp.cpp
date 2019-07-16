/*  @file    voice_model.cpp
 *  @project NUAA_IARC
 *  @author  ZiwenLIU
 *  @date    July-11-2019
 * 
 *  @brief
 */

#include "main.hpp"

long micros(void)
{
    struct timeval time_now;
    long usec_retval;

    gettimeofday(&time_now, NULL);
    
    usec_retval = 1000000L * time_now.tv_sec + time_now.tv_usec;

    return usec_retval;

    // gettimeofday(&tEnd, NULL);

    // uint64_t deltaTime = 1000000L * (tEnd.tv_sec - tBegin.tv_sec ) + (tEnd.tv_usec - tBegin.tv_usec);

    // std::cout << "Time Spent :" << deltaTime << std::endl;
    // //printf("Time spent: %ldus\n", deltaTime);
}



  // 时间戳做差函数
  // struct timeval tBegin, tEnd;
  // gettimeofday(&tBegin, NULL);

  // int count = 0;
  
  // usleep(2300);
  
  // gettimeofday(&tEnd, NULL);
  
  // uint64_t deltaTime = 1000000L * (tEnd.tv_sec - tBegin.tv_sec ) + (tEnd.tv_usec - tBegin.tv_usec);

  // std::cout << "Time Spent :" << deltaTime << std::endl;
  // //printf("Time spent: %ldus\n", deltaTime);