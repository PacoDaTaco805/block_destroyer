#include <bits/types/struct_timeval.h>
#include <stdint.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

/**
 * Number of microseconds in a second
 */
const int us_per_s = 1000000;

/**
 * The current target FPS
 */
const long FPS = 30;

/**
 * The number of microseconds each game tick (AKS loop iteration) should take to achieve FPS fps
 */
const long ms_per_loop = us_per_s / FPS;

/**
 * Calculates the time in microseconds from a timeval struct
 */
long get_time_us(struct timeval* time_value) {
  return time_value->tv_sec * us_per_s + time_value->tv_usec;
}

int main(int argc, char *argv[])
{
  struct timeval start_time;
  struct timeval end_time;

  for (;;) {
    // Get initial time before work is done
    gettimeofday(&start_time, NULL);

    // ----------------------------------------
    // DO WORK HERE
    // ----------------------------------------







    // ----------------------------------------
    // Get time after doing work
    gettimeofday(&end_time, NULL);

    // Convert time structs in time in us
    long start_time_us = get_time_us(&start_time);
    long end_time_us = get_time_us(&end_time);

    // Calcualte time it took to do work
    long delta = end_time_us - start_time_us;

    // Get time under time per frame
    long extra_time = ms_per_loop - delta;

    // Sleep to keep loop times constant
    if (extra_time > 0)
    {
      usleep(extra_time);
    }


    printf("tick...\n");
  }
}
