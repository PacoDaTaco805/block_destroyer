#include <stdint.h>

/**
 * This represents the state of a single character cell in the terminal.
 * Things like the character used, the color of the forground and background,
 * and styling.
 */
typedef struct half_block {
  char character;
} Half_Block;

/**
 * A block is basically a pixel and is comprised of two Half_Block structs.
 */
typedef struct block {
  Half_Block left_half_block;
  Half_Block right_half_block;
} Block;

typedef struct size {
  uint32_t width;
  uint32_t height;
} Size;

typedef struct position {
  float x;
  float y;
} Position;

typedef struct velocity {
  float speed;
  float direction;
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
