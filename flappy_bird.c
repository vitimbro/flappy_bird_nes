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







//--------------------------------------------------------//
//                     PALETTE SETUP                      //
//--------------------------------------------------------//


/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x21,			            // screen color

  0x19,0x29,0x39,0x00,	            // background palette 0
  0x19,0x27,0x37,0x00,	            // background palette 1
  0x19,0x2C,0x39,0x00,	            // background palette 2
  0x2C,0x3D,0x30,0x00,              // background palette 3

  0x16,0x35,0x24,0x00,	            // sprite palette 0
  0x00,0x37,0x25,0x00,	            // sprite palette 1
  0x0D,0x2D,0x3A,0x00,	            // sprite palette 2
  0x0D,0x27,0x2A	            // sprite palette 3
};



//--------------------------------------------------------//
//                       VARIABLES                        //
//--------------------------------------------------------//










//--------------------------------------------------------//
//                  FUNCTION PROTOTYPES                   //
//--------------------------------------------------------//


void setup_graphics();




//--------------------------------------------------------//
//                       FUNCTIONS                        //
//--------------------------------------------------------//


// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
}


//--------------------------------------------------------//
//                    MAIN GAME LOOP                      //
//--------------------------------------------------------//

void main(void)
{
  setup_graphics();
  // draw message  
  vram_adr(NAMETABLE_A);
  vram_write(nametable_game, 1024);
  // enable rendering
  ppu_on_all();
  // infinite loop
  while(1) {
  }
}
