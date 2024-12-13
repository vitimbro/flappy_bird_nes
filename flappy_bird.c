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

//--------------------------------------------------------//
//                 CONSTANTS AND DEFINES                  //
//--------------------------------------------------------//

#define PLAYER_INIT_X 50                 // Initial X position in pixels
#define PLAYER_INIT_Y 50                 // Initial Y position in pixels

#define SUBPIXELS 16                     // Subpixels per pixel (for smoother movement)

#define GRAVITY 8                       // Gravity applied to the player
#define MAX_GRAVITY 80

#define JUMP_SPEED -120 


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
  0x19,0x29,0x39,0x00,	            // background palette 0     ~grass  
  0x19,0x27,0x37,0x00,	            // background palette 1     ~ground
  0x19,0x2C,0x39,0x00,	            // background palette 2     ~bushes
  0x2C,0x3D,0x30,0x00,              // background palette 3     ~buildings and clouds

  0x0F,0x28,0x30,0x00,	            // sprite palette 0         ~bird body
  0x0F,0x28,0x16,0x00,	            // sprite palette 1         ~bird mouth
  0x0F,0x1B,0x2A,0x00,	            // sprite palette 2         ~pipe shadow
  0x0F,0x2A,0x3A	            // sprite palette 3         ~pipe light
};


//--------------------------------------------------------//
//                      METASPRITES                       //
//--------------------------------------------------------//

DEF_METASPRITE_2x2(bird, 0x111, 0); // define bird metasprite





//--------------------------------------------------------//
//                       VARIABLES                        //
//--------------------------------------------------------//


int scroll_x = 0; // x scroll position
int scroll_x_vel = 1; // x scroll direction

int player_y = 0; // player y position
int player_y_sub = 0;
int player_y_vel_sub = 0; // player y velocity






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


//--------------------------------------------------------//
//                       FUNCTIONS                        //
//--------------------------------------------------------//


// setup PPU and tables
void setup_graphics() {
  oam_clear();             // clear OAM buffer, all the sprites are hidden
  pal_all(PALETTE);        // set palette colors
  bank_bg(0);              // select chr bank 0 for background
  bank_spr(1);             // select chr bank 1 for sprites
}

void scroll_horizontal() {
  // set scroll register
  // waits for NMI, which means no frame-skip?
  split(scroll_x, 0);
  // update scroll_x variable
  scroll_x += scroll_x_vel;

}

void initialize_player() {
   player_y = PLAYER_INIT_Y; // player y position
}

void update_player() {
  handle_player_input();
  apply_player_physics();
  handle_player_movement();
}

void handle_player_input() {
  char controller_1 = pad_trigger(0);
  
  if ((controller_1 & PAD_A)) {
        player_jump();  // Initiate jump
    }
  
}

void apply_player_physics() {
  player_y_vel_sub += GRAVITY;
  
  // Cap the fall speed
  if (player_y_vel_sub > MAX_GRAVITY) {
    player_y_vel_sub = MAX_GRAVITY;
  }
}

void handle_player_movement() {
  int pixel_movement = 0;
  int new_y = player_y;

  // Update the subpixel position by adding the velocity (in subpixels)
  player_y_sub += player_y_vel_sub;
  
  // Convert subpixels to full pixels when it exceeds 16 subpixels 
  while (abs(player_y_sub) >= SUBPIXELS) {
      if (player_y_sub > 0) {                // if player is falling
          pixel_movement++;
          player_y_sub -= SUBPIXELS;
      } else {                               // if player is going up
          pixel_movement--;
          player_y_sub += SUBPIXELS;
      }
  }
  
  new_y += pixel_movement;
  
  // Keep the player in the screen
  if (new_y < 18) new_y = 19;
  if (new_y > 186) new_y = 185;
  
  // Update player position
  player_y = new_y;
  
}

void player_jump() {
  player_y_vel_sub = JUMP_SPEED;
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
  vram_write(nametable_game, 1024);

  
  // enable rendering
  ppu_on_all();
  
  initialize_player();
  
  // infinite loop
  while(1) {
    char oam_id = 0; // variable to draw sprites
    
    oam_id = oam_spr(20, 18, 0x11E, OAM_BEHIND, oam_id); // sprite zero for splitting screen
    
    
    update_player();
    
    scroll_horizontal();
    oam_id = oam_meta_spr(PLAYER_INIT_X, player_y, oam_id, bird); // draw flappy bird metasprite
    
    
    
  }
  
}
