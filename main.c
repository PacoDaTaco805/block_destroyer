#define _POSIX_C_SOURCE 200809L

#include <bits/time.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/**
 * Number of microseconds in a second
 */
const int ns_per_s = 1000000000;

/**
 * The current target FPS
 */
const long FPS = 30;

/**
 * The number of microseconds each game tick (AKS loop iteration) should take to
 * achieve FPS fps
 */
const long ns_per_loop = ns_per_s / FPS;

/**
 * Current width of the terminal in characters
 */
uint32_t terminal_width = 0;

/**
 * Current height of the terminal in characters
 */
uint32_t terminal_height = 0;

typedef enum direction { NORTH, EAST, SOUTH, WEST } Direction;

typedef struct half_block {
  char character;
} Half_Block;

typedef struct block {
  Half_Block left_half_block;
  Half_Block right_half_block;
} Block;

typedef struct position {
  uint32_t x;
  uint32_t y;
} Position;

typedef struct size {
  uint32_t width;
  uint32_t height;
} Size;

typedef struct velocity {
  uint32_t speed;
  Direction direction;
} Velocity;

typedef struct object {
  Position position;
  Size size;
  Block **blocks;

} Object;

typedef struct layer {
  uint32_t width;
  uint32_t height;
  uint32_t object_count;
  Object *objects;

} Layer;

typedef struct frame {
  uint32_t width;
  uint32_t height;
  Block **blocks;
} Frame;

/**
 * Gets the latest dimensions of the terminal and updates the global variables
 */
void updateTerminalDimensions() {
  struct winsize window_size;

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size) == -1) {
    exit(1);
  }

  terminal_width = window_size.ws_row;
  terminal_height = window_size.ws_col;
}

/**
 * The handler for sigwinch signals
 */
static void sigwinchHandler(int sig) { updateTerminalDimensions(); }

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

uint64_t get_time_ns(struct timespec time_spec) {
  return time_spec.tv_sec * ns_per_s + time_spec.tv_nsec;
}

struct timespec get_time_spec_from_ns(uint64_t time_ns) {
  struct timespec time_spec = {.tv_sec = time_ns / ns_per_s,
                               .tv_nsec = time_ns % ns_per_s};
  return time_spec;
}

uint32_t write_object_to_frame(Object *object, Frame *frame) {
  uint32_t min_x = (object->position.x - object->size.width / 2);
  uint32_t max_x = (object->position.x + object->size.width / 2);

  uint32_t min_y = (object->position.y - object->size.height / 2);
  uint32_t max_y = (object->position.y + object->size.height / 2);

  uint32_t x_offset;
  uint32_t y_offset;

  if (min_x < 0 && max_x >= frame->width) {
    return -1;
  } else if (min_x < 0) {
    x_offset = 0;
  } else if (max_x >= frame->width) {
    x_offset = frame->width - object->size.width;
  }

  if (min_y < 0 && max_y >= frame->height) {
    return -1;
  } else if (min_y < 0) {
    y_offset = 0;
  } else if (max_y >= frame->width) {
    y_offset = frame->height - object->size.height;
  }

  for (int row = 0; row < object->size.height; row++) {
    if (y_offset + row >= frame->height) {
      return -1;
    }

    for (int column = 0; column < object->size.width; column++) {
      if (x_offset + column >= frame->width) {
        return -1;
      }
      frame->blocks[y_offset + row][x_offset + column] = object->blocks[row][column];
    }
  }

  return 0;
}

uint32_t generate_frame(Layer *layers, uint32_t layer_count, Frame *frame) {
  if (layers == NULL) {
    return -1;
  }

  frame = malloc(sizeof(Frame));
  frame->width = terminal_width;
  frame->height = terminal_height;

  frame->blocks = malloc(sizeof(Block *) * terminal_height);
  for (int row = 0; row < terminal_height; row++) {
    frame->blocks[row] = NULL;
  }

  Layer current_layer;

  for (int currentLayerIndex = 0; currentLayerIndex < layer_count;
       currentLayerIndex++) {
    current_layer = layers[currentLayerIndex];

    for (int current_obj_index = 0; current_obj_index < current_layer.object_count; current_obj_index++) {
      write_object_to_frame((current_layer.objects + currentLayerIndex), frame);
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  setupSigWinchSignaHandler();

  struct timespec start_time;
  struct timespec end_time;

  for (;;) {
    // Get initial time before work is done
    clock_gettime(CLOCK_REALTIME, &start_time);

    // ----------------------------------------
    // DO WORK HERE
    // ----------------------------------------

    // ----------------------------------------
    // Get time after doing work
    clock_gettime(CLOCK_REALTIME, &end_time);

    uint64_t start_time_ns = get_time_ns(start_time);
    uint64_t end_time_ns = get_time_ns(end_time);

    // Calcualte time it took to do work
    uint64_t delta = start_time_ns - end_time_ns;

    // Get time under time per frame
    uint64_t extra_time_ns = ns_per_loop - delta;

    // Sleep to keep loop times constant
    if (extra_time_ns > 0) {
      struct timespec time_to_sleep = get_time_spec_from_ns(extra_time_ns);
      nanosleep(&time_to_sleep, NULL);
    }

    printf("tick...\n");
  }
}
