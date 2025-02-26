//--------------------------------------------------------//
//               Flappy Bird for the NES                  //
//--------------------------------------------------------//
// ~ by vitimbro and catsyser

/*

                     ▄███████████████▄
                   ▄████░░░░░░░░█▀    █▄    
                  ██░░░░░░░░░░░█▀      █▄
                 ██░░░░░░░░░░░█▀        █▄
                ██░░░░░░░░░░░░█          ██
               ██░░░░░░░░░░░░░█      ██  ██
              ██░░░░░░░░░░░░░░█▄     ██  ██
             ████████████░░░░░░██        ██
            ██░░░░░░░░░░░██░░░░░█████████████
            ██░░░░░░░░░░░██░░░░█▓▓▓▓▓▓▓▓▓▓▓▓▓█
            ██░░░░░░░░░░░██░░░█▓▓▓▓▓▓▓▓▓▓▓▓▓▓█
             ▀███████████▒▒▒▒█▓▓▓███████████▀
                ██▒▒▒▒▒▒▒▒▒▒▒▒█▓▓▓▓▓▓▓▓▓▓▓▓█
                 ██▒▒▒▒▒▒▒▒▒▒▒▒██▓▓▓▓▓▓▓▓▓▓█
                  █████▒▒▒▒▒▒▒▒▒▒██████████
                     ▀███████████▀
                     
*/

  // Branch vitor

//-------------------------------------------------------//
//                  LINKS AND INCLUDES                   //
//-------------------------------------------------------//

// Standard Libraries
#include <stdlib.h>
#include <string.h>

// NES-Specific Libraries
#include "neslib.h"                 // NES Library with useful NES functions
#include <nes.h>                    // CC65 NES Header (PPU definitions)

// Arithmetic and VRAM Utilities
#include "bcd.h"                    // BCD arithmetic support
//#link "bcd.c"

#include "vrambuf.h"                // VRAM update buffer
//#link "vrambuf.c"

// CHR Data
//#resource "tileset.chr"           // Character set (CHR) data
//#link "tileset.s"

// Game Nametables
#include "nametable_game.h"
#include "nametable_game_pipes.h"
#include "nametable_game_test_colors.h"

//--------------------------------------------------------//
//                 CONSTANTS AND DEFINES                  //
//--------------------------------------------------------//

// vertical mirroring (horizontal scroling)
#define NES_MIRRORING 1

#define PLAYER_X 50                      // player X position in pixels
#define PLAYER_INIT_Y 50                 // Initial Y position in pixels

#define PLAYER_MIN_Y 27
#define PLAYER_MAX_Y 186

#define SUBPIXELS 16                     // Subpixels per pixel (for smoother movement)

#define GRAVITY 4                       // Gravity applied to the player
#define MAX_GRAVITY 80

#define JUMP_SPEED -64 

#define SCROLL_X_SPEED 8 

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define TILE_SIZE 8

#define STATUS_BAR_HEIGHT_TILES 4

#define PLAYROWS 21

#define PIPE_WIDTH_TILES 4           // Pipe width in tiles (4 tiles wide)
#define PIPE_GAP_TILES 7             // Fixed gap size in tiles
#define PIPE_SPAWN_X 256             // New pipes spawn offscreen at x = 256
#define MAX_PIPES 2                  // Maximum active pipes at a time

// Pipe Metatile Definitions
#define PIPE_BRIM_MT  0  // Brim (edge part of pipe)
#define PIPE_BODY_MT  1  // Body (middle part of pipe)
#define SKY 2            // Sky


#define FADE_TIME 4                      // Fade in/out duration in frames
#define FLASH_TIME 6



#define BIRD_HITBOX_TOP_OFFSET  6   // Ignore top  pixels
#define BIRD_HITBOX_BOTTOM_OFFSET 6  // Ignore bottom  pixels
#define BIRD_HITBOX_LEFT_OFFSET  6   // Ignore left  pixels
#define BIRD_HITBOX_RIGHT_OFFSET 3   // Ignore right  pixels



// Pipe metatile patterns (each row represents 4 tile-wide metatile)
const char pipe_metatiles[3][4] = {
    {0xCC, 0xCD, 0xCE, 0xCF},  // PIPE_BRIM_MT (Brim)
    {0xDC, 0xDD, 0xDE, 0xDF},  // PIPE_BODY_MT (Body)
    {0x00, 0x00, 0x00, 0x00}   // SKY
};

// Pipe Heights (Upper, Lower)
const byte pipe_heights[5][2] = {
    {3, 11}, {5, 9}, {7, 7}, {9, 5}, {11, 3}
};

// Pipe Structure
typedef struct {
    int x;     // Pipe x-position (moves left)
    byte gap;  // Gap index (0-4, determining pipe height)
    byte slice_progress;  // Tracks how many tiles have been drawn
} Pipe;

// Pipe Array
Pipe pipes[MAX_PIPES];

char pipe_buffer[PLAYROWS]; 

char color_buffer[PLAYROWS / 4];  // Stores nametable attributes (1/4 size of nametable)


//-------------------- METASPRITES -----------------------//

// Define a 2x2 Metasprite for Flappy Bird
#define DEF_METASPRITE_2x2(name,code,pal) \
    const unsigned char name[]={          \
        0, 0, (code)+0, pal,              \
        8, 0, (code)+1, pal,              \
        0, 8, (code)+16, pal,             \
        8, 8, (code)+17, pal+1,           \
        128                               \
    };


//--------------------------------------------------------//
//                     PALETTE SETUP                      //
//--------------------------------------------------------//



/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x21,			            // screen color             ~sky
  0x19,0x29,0x39,0x00,	            // background palette 0     ~grass and pipes  
  0x0F,0x27,0x37,0x00,	            // background palette 1     ~ground and score
  0x25,0x25,0x25,0x00,	            // background palette 2     ~
  0x25,0x33,0x30,0x00,              // background palette 3     ~buildings and clouds

  0x0F,0x28,0x30,0x00,	            // sprite palette 0         ~bird body
  0x0F,0x28,0x16,0x00,	            // sprite palette 1         ~bird mouth
  0x25,0x25,0x25,0x00,	            // sprite palette 2         ~
  0x25,0x25,0x25	            // sprite palette 3         ~
};


//--------------------------------------------------------//
//                      METASPRITES                       //
//--------------------------------------------------------//

DEF_METASPRITE_2x2(bird, 0x111, 0); // define bird metasprite



//--------------------------------------------------------//
//                       VARIABLES                        //
//--------------------------------------------------------//


int scroll_x = 0;                     // x scroll position
int scroll_x_sub = 0;                 // x scroll position (subpixel)
int scroll_x_vel = SCROLL_X_SPEED;    // x scroll velocity in subpixels


int player_y = 0;                     // player y position
int player_y_sub = 0;                 // player y position (subpixel)
int player_y_vel = 0;                 // player y velocity in subpixels

bool player_alive = true;



//--------------------------------------------------------//
//                  FUNCTION PROTOTYPES                   //
//--------------------------------------------------------//


void setup_graphics();
void scroll_horizontal();

void initialize_player();
void update_player();

void handle_player_input();
void apply_player_physics();
void handle_player_movement(); 

void player_jump();

int apply_subpixel_movement(int *position, int *subpixel, int velocity);



void initialize_pipes();
void update_pipes();
void draw_pipe(byte pipe_index, int x_pos, byte gap_index, byte slice_progress);

void put_color(word addr);
word nametable_to_attribute_addr(word a);

bool check_collision();


void fade_in();
void fade_out();
void flash_screen();

//--------------------------------------------------------//
//                       FUNCTIONS                        //
//--------------------------------------------------------//



//  Initializes PPU and graphics-related settings.
 
void setup_graphics() {
  oam_clear();             // Clear OAM buffer, hiding all sprites
  pal_all(PALETTE);        // Load predefined color palette for background and sprites
  bank_bg(0);              // Use CHR bank 0 for background tiles
  bank_spr(1);             // Use CHR bank 1 for sprite tiles
}



// Applies subpixel-based movement and returns full pixel change.

int apply_subpixel_movement(int *position, int *subpixel, int velocity) {
    // Takes a position pointer, subpixel pointer, and velocity.
    int pixel_movement = 0;

    *subpixel += velocity;  // Accumulates velocity into subpixels.

    // Convert accumulated subpixels into full pixels
    while (abs(*subpixel) >= SUBPIXELS) {  
        if (*subpixel > 0) {      
            pixel_movement++;
            *subpixel -= SUBPIXELS;
        } else {                
            pixel_movement--;
            *subpixel += SUBPIXELS;
        }
    }

    *position += pixel_movement;  // Update main position with pixel movement
    return pixel_movement;        // Return how many pixels were moved
}



// Handles horizontal scrolling of the background.
 
void scroll_horizontal() {
  // Uses subpixel precision for smooth scrolling.
  
  if (player_alive) {
    apply_subpixel_movement(&scroll_x, &scroll_x_sub, scroll_x_vel); // Updates `scroll_x` using `apply_subpixel_movement()`.
  }
  
  // Reset scroll_x when it completes both nametables (512 pixels)
  if (scroll_x >= 512) {
    scroll_x -= 512;
  }

  // Sets the scroll register using `split()`.
  split(scroll_x, 0);  // Update the scroll position
}



// Initializes the player’s position.
 
void initialize_player() {
   player_y = PLAYER_INIT_Y; // Set player’s initial Y position
}



// Updates all aspects of the player’s movement each frame.
 
void update_player() {
  handle_player_input();     // Reads and Process controller input
  apply_player_physics();    // Apply gravity and movement physics
  handle_player_movement();  // Updates player movement using subpixel calculations
}


// Handles input from the controller.

void handle_player_input() {
  char controller_1 = pad_trigger(0);  // Read input from player 1
  
  if (controller_1 & PAD_A) {  
    player_jump();  // Initiate jump if A is pressed
  }
}


// Applies gravity to the player's velocity.

void apply_player_physics() {
  player_y_vel += GRAVITY;  // Apply gravity (Increases downward velocity each frame.)
  
  // Caps fall speed to `MAX_GRAVITY` to prevent excessive falling speed.
  if (player_y_vel > MAX_GRAVITY) {
    player_y_vel = MAX_GRAVITY;
  }
}


// Makes the player jump.
 
void player_jump() {
  if (player_alive) {
  	player_y_vel = JUMP_SPEED;    // Sets `player_y_vel` to a negative value (JUMP_SPEED) to move upward.
  }
}


// Handles vertical movement of the player using subpixels.

void handle_player_movement() {
  int new_y = player_y;

  // Apply subpixel movement for smoother motion
  apply_subpixel_movement(&new_y, &player_y_sub, player_y_vel);
  
  // Prevent the player from moving outside the screen bounds
  if (new_y < PLAYER_MIN_Y) new_y = PLAYER_MIN_Y + 1;   // Prevent moving too high
  if (new_y > PLAYER_MAX_Y) new_y = PLAYER_MAX_Y - 1; // Prevent moving too low
  
  player_y = new_y;  // Update player position
}








void initialize_pipes() {
    byte i;  
    for (i = 0; i < MAX_PIPES; i++) {
      pipes[i].slice_progress = PIPE_WIDTH_TILES;          // Reset drawing progress
    }
}


// Update Pipe Positions & Spawn New Pipes
void update_pipes() {
    byte i;
    int pixels_moved = apply_subpixel_movement(&scroll_x, &scroll_x_sub, scroll_x_vel);

    for (i = 0; i < MAX_PIPES; i++) {
        pipes[i].x -= pixels_moved;  // Move pipes left based on scroll speed
        
        if (pipes[i].slice_progress < PIPE_WIDTH_TILES) {
            draw_pipe(i, i * 128, pipes[i].gap, pipes[i].slice_progress);
        }
    }
  
    if ((scroll_x == 504 || scroll_x == 248) && pipes[0].slice_progress == PIPE_WIDTH_TILES) { // Pipe 0 (x = 0) spawns
      pipes[0].x = PIPE_SPAWN_X + 8;
      pipes[0].gap = rand8() % 5;
      pipes[0].slice_progress = 0;          // Reset drawing progress
      
    }

    if ((scroll_x == 120 || scroll_x == 376) && pipes[1].slice_progress == PIPE_WIDTH_TILES) { // Pipe 1 (x = 128) spawns
      pipes[1].x = PIPE_SPAWN_X + 8;
      pipes[1].gap = rand8() % 5;
      pipes[1].slice_progress = 0;          // Reset drawing progress
    }
    
}

// // Fills pipe_buffer with the correct tiles for the pipe slice
void draw_pipe(byte pipe_index, int x_pos, byte gap_index, byte slice_progress) {
    byte i;
	
    byte tile_x;
    byte upper_height;
    byte lower_height;
  
    // Determine the base nametable based on scroll_x
    int base_addr; 
    int addr;
  
    word attr_addr;
  
    if (scroll_x < 128) {        // (0 - 127) Pipes are written to Nametable B
        base_addr = NAMETABLE_B;
    } else if (scroll_x < 256) { // (128 - 255) Pipes go into Nametable A
        base_addr = NAMETABLE_A;
    } else if (scroll_x < 384) { // (256 - 383) Pipes are placed in Nametable A
        base_addr = NAMETABLE_A;
    } else {                     // (384 - 511) Pipes go into Nametable B
        base_addr = NAMETABLE_B;
    }
  
    tile_x = (x_pos % 256) / TILE_SIZE;

    upper_height = pipe_heights[gap_index][0];
    lower_height = pipe_heights[gap_index][1];
  
    // Clear the nametable buffer before writing
    memset(pipe_buffer, 0, sizeof(pipe_buffer));
  
  
     // --- Draw Upper Pipe ---
    for (i = 0; i < upper_height - 1; i++) {
        pipe_buffer[i] = pipe_metatiles[PIPE_BODY_MT][slice_progress];
    }
    pipe_buffer[upper_height - 1] = pipe_metatiles[PIPE_BRIM_MT][slice_progress];

    // --- Draw Sky (Gap) ---
    for (i = 0; i < PIPE_GAP_TILES; i++) {
        pipe_buffer[upper_height + i] = pipe_metatiles[SKY][slice_progress];
    }

    // --- Draw Lower Pipe ---
    pipe_buffer[upper_height + PIPE_GAP_TILES] = pipe_metatiles[PIPE_BRIM_MT][slice_progress];
    for (i = 0; i < lower_height - 1; i++) {
        pipe_buffer[upper_height + PIPE_GAP_TILES + 1 + i] = pipe_metatiles[PIPE_BODY_MT][slice_progress];
    }

    // Compute VRAM address and write the entire column in ONE call
    addr = base_addr + (STATUS_BAR_HEIGHT_TILES * 32) + tile_x + slice_progress;
    vrambuf_put(addr | VRAMBUF_VERT, pipe_buffer, PLAYROWS);
  
    // Compute attribute table address and write it to VRAM buffer
    attr_addr = nametable_to_attribute_addr(addr);
    put_color(attr_addr);

    pipes[pipe_index].slice_progress++;
}


// Writes attribute data to VRAM buffer (can't use VRAMBUF_VERT)
void put_color(word addr) {
    byte i;
    for (i = 0; i < PLAYROWS / 4; i++) {
        vrambuf_put(addr, &color_buffer[i], 1);
        addr += 8;  // Move to next row in attribute table (each row is 8 bytes)
    }
    vrambuf_end();
}

// Convert a nametable address to its corresponding attribute table address
word nametable_to_attribute_addr(word a) {
    return (a & 0x2C00) // Keep nametable origin
         | 0x3C0        // Start of attribute table
         | ((a >> 4) & 0x38) // Row within attribute table
         | ((a >> 2) & 0x07); // Column within attribute table
}





// Function to check collision with pipes
bool check_collision() {
    byte i;

    // Bird's adjusted hitbox
    byte bird_hitbox_top = player_y + BIRD_HITBOX_TOP_OFFSET;
    byte bird_hitbox_bottom = player_y + 16 - BIRD_HITBOX_BOTTOM_OFFSET;
    byte bird_hitbox_left = PLAYER_X + BIRD_HITBOX_LEFT_OFFSET;
    byte bird_hitbox_right = PLAYER_X + 16 - BIRD_HITBOX_RIGHT_OFFSET;

    for (i = 0; i < MAX_PIPES; i++) {
        // Check if the pipe is at the bird's x-position
        if (pipes[i].x <= bird_hitbox_right && pipes[i].x + 32 > bird_hitbox_left) {  
            
            // Get the Y-position of the gap
            byte gap_y = 56 + (pipes[i].gap * 16);

            // Check if the bird is within the gap
            if (bird_hitbox_top < gap_y || bird_hitbox_bottom > gap_y + 56) {
                return true;  // Collision detected, player dies
            }
        }
    }
    return false;  // No collision
}


//-----------------------------------------------------------------------------//
//                        Fade In/Out And Flash Functions                      //          //
//-----------------------------------------------------------------------------//


void fade_in() {
    char i;
    for (i = 0; i <= 4; ++i) {
        pal_bright(i); // Increase brightness
        delay(FADE_TIME); // Wait for next frame
    }
}

void fade_out() {
    signed char i;
    for (i = 4; i >= 0; --i) {
        pal_bright(i); // Decrease brightness
        delay(FADE_TIME); // Wait for next frame to smooth the transition
    }
}

void flash_screen() {
    char i;
    pal_bright(8);        // Set screen to maximum brightness
    delay(FLASH_TIME);    // Brief delay for the flash
    pal_bright(2);        // Restore to intermediate brightness
    delay(FLASH_TIME);    // Slight delay for visual impact
    pal_bright(8);        // Flash again to make it more noticeable
    delay(FLASH_TIME); 
    pal_bright(2);
    for (i = 2; i <= 4; ++i) {
        pal_bright(i); // Increase brightness
        delay(FLASH_TIME); // Wait for next frame
    }
}




//--------------------------------------------------------//
//                    MAIN GAME LOOP                      //
//--------------------------------------------------------//

void main(void)
{ 
  ppu_off();  // Disable rendering while setting up
  setup_graphics();

  vram_adr(NAMETABLE_A);
  vram_write(nametable_game, 1024);
  
  vram_adr(NAMETABLE_B);
  vram_write(nametable_game, 1024);
  
  
  vrambuf_clear();  // Clear VRAM buffer
  set_vram_update(updbuf);  // Link VRAM update buffer

  
  // enable rendering
  ppu_on_all();
  
  initialize_player();
  initialize_pipes();
  
  // infinite loop
  while(1) {
    char oam_id = 0; // variable to draw sprites
    
    oam_id = oam_spr(30, 30, 0x11E, OAM_BEHIND, 0); // sprite zero for splitting screen
    
    ppu_wait_nmi();   // wait for NMI to ensure previous frame finished
    vrambuf_clear();  // Clear VRAM buffer each frame immediately after NMI
        
    oam_id = oam_meta_spr(PLAYER_X, player_y, oam_id, bird); // draw flappy bird metasprite
    
    update_player();
    scroll_horizontal();
    
    if (player_alive) {
              
        update_pipes();       // Update and spawn pipes

        if (check_collision()) {
            // Handle death (e.g., stop movement, show game over screen)
            player_alive = false;
            //flash_screen();
        }
     }
    
  }
  

}









