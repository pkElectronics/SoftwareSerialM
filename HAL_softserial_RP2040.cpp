/**
 * pkElectronics
 *
 * Copyright (c) 2019 SoftwareSerialM [https://github.com/FYSETC/SoftwareSerialM]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "HAL_platform.h"

#if HAL_SS_PLATFORM == HAL_PLATFORM_RP2040

#include <Arduino.h>
#include <stdint.h>
#include "HAL_softserial_RP2040.h"

#define SS_TIMER_RATE 1000000ll //1MHz aka microsecond timer


alarm_pool_t* ss_alarm_pool;
struct repeating_timer ss_timer_instance;

void HAL_softSerial_init() { 
  ss_alarm_pool=     
  #if PICO_TIME_DEFAULT_ALARM_POOL_DISABLED == 1 
  alarm_pool_create(3,6);
  #else
  alarm_pool_get_default();
  #endif
  
}

bool HAL_ss_callback(repeating_timer_t* rt){
  SoftSerial_Handler();
  return true;
}

void HAL_softserial_setSpeed(uint32_t speed) {
  cancel_repeating_timer(&ss_timer_instance);
  if (speed != 0) {
    int64_t period  = (SS_TIMER_RATE / (speed*OVERSAMPLE)) - 1;
    period *= -1;
    alarm_pool_add_repeating_timer_us(ss_alarm_pool,period,HAL_ss_callback,NULL,&ss_timer_instance);
  }
}

#endif

