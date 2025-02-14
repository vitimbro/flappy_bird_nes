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

//--------------------------------------------------------//
//                 CONSTANTS AND DEFINES                  //
//--------------------------------------------------------//

// vertical mirroring (horizontal scroling)
#define NES_MIRRORING 1

#define PLAYER_INIT_X 50                 // Initial X position in pixels
#define PLAYER_INIT_Y 50                 // Initial Y position in pixels

#define SUBPIXELS 16                     // Subpixels per pixel (for smoother movement)

#define GRAVITY 5                       // Gravity applied to the player
#define MAX_GRAVITY 80

#define JUMP_SPEED -85 

#define SCROLL_X_SPEED 18


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


//--------------------------------------------------------//
//                       FUNCTIONS                        //
//--------------------------------------------------------//


/*
 * Initializes PPU and graphics-related settings.
 * - Clears sprite memory (OAM buffer) to hide all sprites.
 * - Sets the color palette for background and sprites.
 * - Selects CHR bank 0 for background tiles.
 * - Selects CHR bank 1 for sprite tiles.
 */
void setup_graphics() {
  oam_clear();             // Clear OAM buffer, hiding all sprites
  pal_all(PALETTE);        // Load predefined color palette
  bank_bg(0);              // Use CHR bank 0 for background tiles
  bank_spr(1);             // Use CHR bank 1 for sprite tiles
}


/*
 * Handles horizontal scrolling of the background.
 * - Uses subpixel precision for smooth scrolling.
 * - Updates `scroll_x` using `apply_subpixel_movement()`.
 * - Sets the scroll register using `split()`.
 */
void scroll_horizontal() {
  apply_subpixel_movement(&scroll_x, &scroll_x_sub, scroll_x_vel);
  split(scroll_x, 0);  // Update the NES scroll position
}


/*
 * Initializes the player’s position.
 * - Sets `player_y` to the predefined initial value.
 */
void initialize_player() {
   player_y = PLAYER_INIT_Y; // Set player’s initial Y position
}


/*
 * Updates all aspects of the player’s movement each frame.
 * - Reads player input.
 * - Applies physics (gravity).
 * - Updates player movement using subpixel calculations.
 */
void update_player() {
  handle_player_input();   // Process controller input
  apply_player_physics();  // Apply gravity and movement physics
  handle_player_movement();// Move player based on updated values
}


/*
 * Handles input from the controller.
 * - Reads button presses from Controller 1.
 * - Triggers jump action if the A button is pressed.
 */
void handle_player_input() {
  char controller_1 = pad_trigger(0);  // Read input from player 1
  
  if (controller_1 & PAD_A) {  
    player_jump();  // Initiate jump if A is pressed
  }
}


/*
 * Applies gravity to the player's velocity.
 * - Increases downward velocity each frame.
 * - Caps fall speed to `MAX_GRAVITY` to prevent excessive falling speed.
 */
void apply_player_physics() {
  player_y_vel += GRAVITY;  // Apply gravity to player velocity
  
  // Limit fall speed
  if (player_y_vel > MAX_GRAVITY) {
    player_y_vel = MAX_GRAVITY;
  }
}


/*
 * Makes the player jump.
 * - Sets `player_y_vel` to a negative value (JUMP_SPEED) to move upward.
 */
void player_jump() {
  player_y_vel = JUMP_SPEED;
}


/*
 * Handles vertical movement of the player using subpixels.
 * - Uses `apply_subpixel_movement()` to process smooth movement.
 * - Ensures the player stays within the screen bounds.
 */
void handle_player_movement() {
  int new_y = player_y;

  // Apply subpixel movement for smoother motion
  apply_subpixel_movement(&new_y, &player_y_sub, player_y_vel);
  
  // Prevent the player from moving outside the screen bounds
  if (new_y < 27) new_y = 28;   // Prevent moving too high
  if (new_y > 186) new_y = 185; // Prevent moving too low
  
  player_y = new_y;  // Update player position
}


/*
 * Applies subpixel-based movement and returns full pixel change.
 * - Takes a position pointer, subpixel pointer, and velocity.
 * - Accumulates velocity into subpixels.
 * - Converts subpixels to whole pixels when they exceed `SUBPIXELS` threshold.
 * - Returns the number of full pixels moved.
 */
int apply_subpixel_movement(int *position, int *subpixel, int velocity) {
    int pixel_movement = 0;

    *subpixel += velocity;  // Update subpixel position with velocity

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




//--------------------------------------------------------//
//                    MAIN GAME LOOP                      //
//--------------------------------------------------------//

void main(void)
{ 
  setup_graphics();
  // draw background  
  vram_adr(NAMETABLE_A);
  vram_write(nametable_game, 1024);
  
  vram_adr(NAMETABLE_B);
  vram_write(nametable_game_pipes, 1024);

  
  // enable rendering
  ppu_on_all();
  
  initialize_player();
  
  // infinite loop
  while(1) {
    char oam_id = 0; // variable to draw sprites
    
    oam_id = oam_spr(20, 30, 0x11E, OAM_BEHIND, oam_id); // sprite zero for splitting screen
    scroll_horizontal();
    
    update_player();
    
    oam_id = oam_meta_spr(PLAYER_INIT_X, player_y, oam_id, bird); // draw flappy bird metasprite
    
    ppu_wait_nmi();
    
  }
  
}
