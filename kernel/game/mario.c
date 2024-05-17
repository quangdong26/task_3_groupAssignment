#include "mario.h"

mario_t mario_char;
volatile int isReachTransition = 0;
int w_pressed = 0;
int a_pressed = 0;
int s_pressed = 0;
int d_pressed = 0;
volatile int frameCounter = 0;

void handleJumping(void) {
    if (mario_char.isJumping) { // If Mario is jumping
        if (frameCounter % FRAME_DELAY == 0) {
            mario_char.pastPos.Y = mario_char.currentPos.Y;
            mario_char.currentPos.Y -= mario_char.jumpVelocity; // Move Mario up
            mario_char.jumpVelocity -= GRAVITY; // Reduce jump velocity by gravity

            mario_char.pastPos.X = mario_char.currentPos.X;
            mario_char.currentPos.X += mario_char.horizontalSpeed; // Apply horizontal movement

            if (mario_char.currentPos.Y >= ground_obj.groundPos.Y - OBJECT_HEIGHT) {
                mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Snap back to ground level
                mario_char.isJumping = 0; // End the jump
                mario_char.jumpVelocity = 0; // Reset jump velocity
                mario_char.horizontalSpeed = 0; // Reset horizontal speed
                mario_char.jumpDirection = 0; // Reset jump direction
                mario_char.canDoubleJump = 1; // Reset double jump
            }

            deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
            displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT); // Update Mario's position
            setMarioHitBox(); // Update hitbox
        }
    }
}


void handleHorizontalMovement(MarioAction action) {
    float acceleration = 1.0; // Increased acceleration
    float maxSpeed = 15.0;
    float deceleration = 3.0; // Increased deceleration
    float threshold = 1.5; // Speed threshold for sharper turns

    switch (action) {
        case MOVE_RIGHT:
            if (mario_char.horizontalSpeed < -threshold) {
                mario_char.horizontalSpeed += deceleration; // Decelerate if moving left
            } else {
                mario_char.horizontalSpeed += acceleration; // Accelerate right
            }
            if (mario_char.horizontalSpeed > maxSpeed) mario_char.horizontalSpeed = maxSpeed; // Cap speed
            d_pressed = 1;
            break;
        case MOVE_LEFT:
            if (mario_char.horizontalSpeed > threshold) {
                mario_char.horizontalSpeed -= deceleration; // Decelerate if moving right
            } else {
                mario_char.horizontalSpeed -= acceleration; // Accelerate left
            }
            if (mario_char.horizontalSpeed < -maxSpeed) mario_char.horizontalSpeed = -maxSpeed; // Cap speed
            a_pressed = 1;
            break;
        default:
            if (mario_char.horizontalSpeed > 0) {
                mario_char.horizontalSpeed -= deceleration; // Decelerate to stop
                if (mario_char.horizontalSpeed < 0) mario_char.horizontalSpeed = 0;
            } else if (mario_char.horizontalSpeed < 0) {
                mario_char.horizontalSpeed += deceleration; // Decelerate to stop
                if (mario_char.horizontalSpeed > 0) mario_char.horizontalSpeed = 0;
            }
            a_pressed = 0;
            d_pressed = 0;
            break;
    }

    if (mario_char.horizontalSpeed != 0) { // Update position if moving
        mario_char.pastPos.X = mario_char.currentPos.X;
        mario_char.pastPos.Y = mario_char.currentPos.Y;
        mario_char.currentPos.X += mario_char.horizontalSpeed;
        setMarioHitBox();
        if(isReachTransition == 0) {
            deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
            displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
        }
    }
}

void applyGravity(void) {
    if (!mario_char.isJumping && mario_char.currentPos.Y < ground_obj.groundPos.Y - OBJECT_HEIGHT) { // Apply gravity if not jumping
        if (frameCounter % FRAME_DELAY == 0) {
            mario_char.pastPos.Y = mario_char.currentPos.Y;
            mario_char.currentPos.Y += GRAVITY;

            mario_char.pastPos.X = mario_char.currentPos.X;
            mario_char.currentPos.X += mario_char.horizontalSpeed;

            if (mario_char.currentPos.Y > ground_obj.groundPos.Y - OBJECT_HEIGHT) {
                mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Snap back to ground level
            }

            deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
            displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
            setMarioHitBox(); // Update hitbox
        }
    }
}


void marioMovement(MarioAction action) {
    if (action == JUMP || action == MOVE_RIGHT_JUMP || action == MOVE_LEFT_JUMP) { // Handle jumping
        if (!mario_char.isJumping && mario_char.currentPos.Y == ground_obj.groundPos.Y - OBJECT_HEIGHT) {
            mario_char.isJumping = 1;
            mario_char.jumpVelocity = JUMP_VELOCITY;

            if (action == MOVE_RIGHT_JUMP) {
                mario_char.jumpDirection = 1; // Jump to the right
                mario_char.horizontalSpeed = 5; // Adjust horizontal speed
            } else if (action == MOVE_LEFT_JUMP) {
                mario_char.jumpDirection = -1; // Jump to the left
                mario_char.horizontalSpeed = -5; // Adjust horizontal speed
            } else {
                mario_char.jumpDirection = 0; // Jump straight up
                mario_char.horizontalSpeed = 0; // No horizontal movement
            }
        } else if (mario_char.canDoubleJump && mario_char.isJumping) {
            mario_char.jumpVelocity = JUMP_VELOCITY; // Double jump
            mario_char.canDoubleJump = 0; // Disable further double jumps
        }
    } else {
        handleHorizontalMovement(action); // Handle horizontal movement
    }

}

void initStatMario(void) {
    mario_char.height_size = OBJECT_HEIGHT;
    mario_char.width_size = OBJECT_WIDTH;
    mario_char.currentPos.X = ground_obj.groundPos.X;
    mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Mario starts on the ground
    mario_char.isJumping = 0;
    mario_char.jumpVelocity = 0;
    mario_char.horizontalSpeed = 0;
    mario_char.canDoubleJump = 1; // Allow double jump initially
    setMarioHitBox();
}

void setMarioHitBox(void) {
    changeBoxSize(&mario_char.marioHitBox.bottom_left_corner, mario_char.currentPos, OBJECT_WIDTH,OBJECT_HEIGHT, TOP_LEFT_CORNER);
    changeBoxSize(&mario_char.marioHitBox.bottom_right_corner, mario_char.currentPos, OBJECT_WIDTH,OBJECT_HEIGHT, TOP_RIGHT_CORNER);
    changeBoxSize(&mario_char.marioHitBox.top_left_corner, mario_char.currentPos, OBJECT_WIDTH,OBJECT_HEIGHT, BOTTOM_LEFT_CORNER);
    changeBoxSize(&mario_char.marioHitBox.top_right_corner, mario_char.currentPos, OBJECT_WIDTH,OBJECT_HEIGHT, BOTTOM_RIGHT_CORNER);
        // set size of the hitbox
    mario_char.marioHitBox.width = mario_char.marioHitBox.top_right_corner.X - mario_char.marioHitBox.top_left_corner.X;
    mario_char.marioHitBox.height = mario_char.marioHitBox.bottom_left_corner.Y - mario_char.marioHitBox.top_left_corner.Y;
}

void renderPlayerInitPoint(void) {
    mario_char.height_size = OBJECT_HEIGHT;
    mario_char.width_size = OBJECT_WIDTH;
    mario_char.currentPos.X = ground_obj.groundPos.X;
    mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Mario starts on the ground
    mario_char.isJumping = 0;
    mario_char.jumpVelocity = 0;
    mario_char.horizontalSpeed = 0;
    mario_char.canDoubleJump = 1; // Allow double jump initially
    setMarioHitBox();
}