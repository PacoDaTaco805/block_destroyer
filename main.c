#include <time.h>
#define _POSIX_C_SOURCE 200809L
#include <bits/types/struct_timeval.h>
#include <stdint.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <signal.h>

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
 * Current width of the terminal in characters
 */
uint32_t terminal_width = 0;

/**
 * Current height of the terminal in characters
 */
uint32_t terminal_height = 0;

typedef struct half_block {
   char character;
} Half_Block;


typedef struct block{
    Half_Block left_half_block;
    Half_Block right_half_block;
} Block;

typedef struct object{

} Object;

typedef struct layer{
  uint32_t width;
  uint32_t height;
  Object* objects;

} Layer;


typedef struct frame {
    uint32_t width;
    uint32_t height;
    Block** buffer;
} Frame;

/**
 * Gets the latest dimensions of the terminal and updates the global variables
 */
void updateTerminalDimensions() {
  struct winsize window_size;

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size) == -1){
    exit(1);
  }

  terminal_width = window_size.ws_row;
  terminal_height = window_size.ws_col;
}

/**
 * The handler for sigwinch signals
 */
static void sigwinchHandler(int sig) {
  updateTerminalDimensions();
}

/**
 * Sets up the sigwinch handler
 */
int setupSigWinchSignaHandler() {
  struct sigaction signal_action;

  sigemptyset(&signal_action.sa_mask);

  signal_action.sa_flags = 0;

  signal_action.sa_handler = sigwinchHandler;

  return sigaction(SIGWINCH, &signal_action, NULL);
}

/**
 * Calculates the time in microseconds from a timeval struct
 */
long get_time_us(struct timeval* time_value) {
  return time_value->tv_sec * us_per_s + time_value->tv_usec;
}

int main(int argc, char *argv[])
{
  setupSigWinchSignaHandler();

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
      nanosleep(extra_time * 1000, NULL);
    }


    printf("tick...\n");
  }
}
