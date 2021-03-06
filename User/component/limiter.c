/*
  限制器
*/

#include "limiter.h"

#include <math.h>
#include <stddef.h>

int8_t HeatLimiter_Apply(float heat_limit, float vbat, float dt_sec) {
  (void)heat_limit;
  (void)vbat;
  (void)dt_sec;
  return 0;
}

/**
 * @brief 底盘功率限制
 *
 * @param power_limit 功率阈值
 * @param vbat 电池电压
 * @param motor_out 电机输出值数组
 * @param cur_fb 电机电流反馈值数组
 * @param len 数组长度
 * @return int8_t 0对应没有错误
 */
int8_t PowerLimit_Apply(float power_limit, float vbat, float *motor_out,
                        float *cur_fb, uint32_t len) {
  if (motor_out == NULL) return -1;
  float sum_cur_fb = 0.0f;

  for (uint32_t i = 0; i < len; i++)
    sum_cur_fb += fabsf(cur_fb[i]); /* 检测功率是否超出限制 */
  if (power_limit > (sum_cur_fb * vbat)) return 0;

  float sum_motor_out = 0.0f;
  for (uint32_t i = 0; i < len; i++) {
    sum_motor_out += fabsf(motor_out[i]);
  }

  if (power_limit > 0.0f) {
    if ((sum_motor_out * vbat) > power_limit) {
      float current_scale =
          power_limit / vbat / sum_motor_out; /* 保持每个电机输出值比例不变 */
      for (uint32_t i = 0; i < len; i++) {
        motor_out[i] *= current_scale;
      }
    }
  }
  return 0;
}
