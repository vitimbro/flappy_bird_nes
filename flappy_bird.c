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

  // teste branch vitor

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

#define GRAVITY 5                       // Gravity applied to the player
#define MAX_GRAVITY 80

#define JUMP_SPEED -85 

#define SCROLL_X_SPEED 12

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define TILE_SIZE 8

#define STATUS_BAR_HEIGHT_TILES 4

#define PIPE_WIDTH_TILES 4           // Pipe width in tiles (4 tiles wide)
#define PIPE_GAP_TILES 7             // Fixed gap size in tiles
#define PIPE_SPAWN_X 256             // New pipes spawn offscreen at x = 256
#define MAX_PIPES 2                  // Maximum active pipes at a time

// Pipe Metatile Definitions
#define PIPE_BRIM_MT  0  // Brim (edge part of pipe)
#define PIPE_BODY_MT  1  // Body (middle part of pipe)

// Pipe metatile patterns (each row represents 4 tile-wide metatile)
const char pipe_metatiles[2][4] = {
    {0xCC, 0xCD, 0xCE, 0xCF}, // PIPE_BRIM_MT (Brim)
    {0xDC, 0xDD, 0xDE, 0xDF}  // PIPE_BODY_MT (Body)
};

// Pipe Heights (Upper, Lower)
const byte pipe_heights[5][2] = {
    {3, 11}, {5, 9}, {7, 7}, {9, 5}, {11, 3}
};

// Pipe Structure
typedef struct {
    int x;     // Pipe x-position (moves left)
    byte gap;  // Gap index (0-4, determining pipe height)
} Pipe;

// Pipe Array
Pipe pipes[MAX_PIPES];


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

/*

$ 0F: Black
$ 30: White

$ x0: Gray
$ x1: Azure
$ x2: Blue
$ x3: Violet
$ x4: Magenta
$ x5: Rose
$ x6: Red or maroon
$ x7: Orange
$ x8: Yellow or olive
$ x9: Chartreuse
$ xA: Green
$ xB: Spring
$ xC: Cyan

*/


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




void update_pipes();
void queue_pipe_update(int x_pos, byte gap_index);
void set_metatile(int base_addr, byte x, byte y, const char metatile[4]);
void init_pipes();



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
  apply_subpixel_movement(&scroll_x, &scroll_x_sub, scroll_x_vel); // Updates `scroll_x` using `apply_subpixel_movement()`.
  
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
  player_y_vel = JUMP_SPEED;    // Sets `player_y_vel` to a negative value (JUMP_SPEED) to move upward.
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





// Update Pipe Positions & Spawn New Pipes
void update_pipes() {
    byte i;
    int pixels_moved = apply_subpixel_movement(&scroll_x, &scroll_x_sub, scroll_x_vel);

    for (i = 0; i < MAX_PIPES; i++) {
        pipes[i].x -= pixels_moved;  // Move pipes left based on scroll speed

        // If a pipe moves offscreen (x < -16), respawn it at 256 with a new height
        if (pipes[i].x < -16  ) { 
            pipes[i].x = PIPE_SPAWN_X;
            pipes[i].gap = rand8() % 5;  // Assign a new random gap index
            queue_pipe_update(pipes[i].x, pipes[i].gap);  // Queue VRAM update
        }
    }
}

// Queue Pipe Updates in VRAM Buffer
void queue_pipe_update(int x_pos, byte gap_index) {
    byte i;

    // Determine the base nametable based on `scroll_x`
    int base_addr = (scroll_x < 256) ? NAMETABLE_A : NAMETABLE_B;
    byte tile_x = (x_pos % 256) / TILE_SIZE;

    byte upper_height = pipe_heights[gap_index][0];
    byte lower_height = pipe_heights[gap_index][1];

    // --- Draw Upper Pipe ---
    for (i = 0; i < upper_height - 1; i++) {
        set_metatile(base_addr, tile_x, STATUS_BAR_HEIGHT_TILES + i, pipe_metatiles[PIPE_BODY_MT]);
    }
    set_metatile(base_addr, tile_x, STATUS_BAR_HEIGHT_TILES + upper_height - 1, pipe_metatiles[PIPE_BRIM_MT]);

    // --- Draw Lower Pipe ---
    set_metatile(base_addr, tile_x, STATUS_BAR_HEIGHT_TILES + upper_height + PIPE_GAP_TILES, pipe_metatiles[PIPE_BRIM_MT]);
    for (i = 0; i < lower_height - 1; i++) {
        set_metatile(base_addr, tile_x, STATUS_BAR_HEIGHT_TILES + upper_height + PIPE_GAP_TILES + 1 + i, pipe_metatiles[PIPE_BODY_MT]);
    }
}


// Set Metatile Function (Writes to VRAM Buffer)
void set_metatile(int base_addr, byte x, byte y, const char metatile[4]) {
    int addr = base_addr + (y * 32) + x;  // Compute VRAM address

    vrambuf_put(addr, metatile, 4);  // Send 4 bytes to the VRAM buffer queue
}

// Initialize Pipe System
void init_pipes() {
    byte i;
    for (i = 0; i < MAX_PIPES; i++) {
        pipes[i].x = PIPE_SPAWN_X + (i * 128); // Spread initial pipes evenly
        pipes[i].gap = rand8() % 5;
    }
}




//--------------------------------------------------------//
//                    MAIN GAME LOOP                      //
//--------------------------------------------------------//

void main(void)
{ 
  ppu_off();  // Disable rendering while setting up
  setup_graphics();
  
  oam_spr(1, 30, 0x11E, OAM_BEHIND, 0); // sprite zero for splitting screen
  
  vrambuf_clear();  // Clear VRAM buffer
  set_vram_update(updbuf);  // Link VRAM update buffer
  
  
  vram_adr(NAMETABLE_A);
  vram_write(nametable_game, 1024);
  
  vram_adr(NAMETABLE_B);
  vram_write(nametable_game, 1024);

  
  // enable rendering
  ppu_on_all();
  
  initialize_player();
  init_pipes();  // Initialize pipes
  
  // infinite loop
  while(1) {
    char oam_id = 4; // variable to draw sprites   
    
    ppu_wait_nmi();   // wait for NMI to ensure previous frame finished
    vrambuf_clear();  // Clear VRAM buffer each frame immediately after NMI
   
    
    scroll_horizontal();
    
    oam_id = oam_meta_spr(PLAYER_X, player_y, oam_id, bird); // draw flappy bird metasprite
    
    update_player();
    update_pipes();       // Update and spawn pipes
    
  }
  

}









