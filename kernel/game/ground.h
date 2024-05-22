#include "../framebuffer/framebf.h"
#include "../utils/utils.h"

#include "../background/background2.h"
#include "../image/marioImg.h"

// Define ground position
#define GND_X_POS 0
#define GND_Y_POS 600

// define Left/right coordinate
enum corner_position {
    TOP_LEFT_CORNER,
    TOP_RIGHT_CORNER,
    BOTTOM_LEFT_CORNER,
    BOTTOM_RIGHT_CORNER
};

#define BACKGROUND1_HEIGHT 224
#define BACKGROUND1_WIDTH 3268
#define ENTIRE_SCREEN 3000

// Coordinate struct
typedef struct {
    volatile int X;
    volatile int Y;
} coordinate_t;

// Ground struct
typedef struct {
    coordinate_t groundPos;
    volatile int width;
    volatile int height;
} ground_t;
extern ground_t ground_obj;

void setGroundObject(void);