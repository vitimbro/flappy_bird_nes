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
#include "nametable_ufabc.h"
#include "nametable_another_universe.h"

#include "nametable_game.h"
#include "nametable_title.h"
#include "nametable_death.h"


// FamiTone Music and Sound Data
//#link "famitone2.s"
void __fastcall__ famitone_update(void);


// Gameplay Music 
//#link "music_data.s"
extern char music_game[];


// Sound Effects
//#link "sfx_data.s"
extern char sfx_game[];

//--------------------------------------------------------//
//                 CONSTANTS AND DEFINES                  //
//--------------------------------------------------------//

#define NES_MIRRORING 1                  // Vertical mirroring (for horizontal scroling)

#define PLAYER_X 50                      // player X position in pixels
#define PLAYER_INIT_Y 50                 // Initial Y position in pixels

#define PLAYER_MIN_Y 27                  // player upper limit
#define PLAYER_MAX_Y 186		 // player bottom limit

#define SUBPIXELS 16                     // Subpixels per pixel (for smoother movement)

#define GRAVITY 4                        // Gravity applied to the player
#define MAX_GRAVITY 80

#define JUMP_SPEED -68 			

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

#define CITY_HEIGHT 7

// Pipe Metatile Definitions
#define PIPE_BRIM  0  
#define PIPE_BODY_SKY  1  
#define SKY 2            

#define CLOUDS_OUTLINE 3
#define CLOUDS_FILLING 4
#define BUIDINGS_TOP 5
#define BUILDINGS_BOTTOM 6

#define PIPE_BODY_CITY 7
#define PIPE_BODY_FLOOR 8


#define FADE_TIME 4                      // Fade in/out duration in frames
#define FLASH_TIME 6


#define BIRD_HITBOX_TOP_OFFSET  6   // Ignore top  pixels
#define BIRD_HITBOX_BOTTOM_OFFSET 6  // Ignore bottom  pixels
#define BIRD_HITBOX_LEFT_OFFSET  6   // Ignore left  pixels
#define BIRD_HITBOX_RIGHT_OFFSET 3   // Ignore right  pixels


// Game State Definitions
#define STATE_CREDITS 0
#define STATE_MENU  1
#define STATE_GAME  2
#define STATE_DEATH 3




// Pipe metatile patterns (each row represents 4 tile-wide metatile)
const char pipe_metatiles[9][4] = {
    {0xCC, 0xCD, 0xCE, 0xCF},   // PIPE_BRIM_MT (Brim)
    {0xDC, 0xDD, 0xDE, 0xDF},   // PIPE_BODY_MT (Body)
    {0x00, 0x00, 0x00, 0x00},   // SKY
  
    {0xC0, 0xC1, 0xC2, 0xC3},   // CLOUDS_OUTLINE
    {0x03, 0x03, 0x03, 0x03},   // CLOUDS_FILLING
    {0xD0, 0xD1, 0xD2, 0xD3},   // BUIDINGS_TOP
    {0xE0, 0xE1, 0xE2, 0xE3},   // BUILDINGS_BOTTOM
  
    {0xEC, 0xED, 0xEE, 0xEF},   // PIPE_BODY_CITY 
    {0xFC, 0xFD, 0xFE, 0xFF}    // PIPE_BODY_FLOOR 
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
    bool passed;  // Flag to track if the pipe has been passed
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
  0x0F,0x33,0x30,0x00,              // background palette 3     ~buildings and clouds

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

bool player_alive = false;             // 


unsigned int score = 0;


// Game state variable
unsigned char game_state = STATE_CREDITS; // Start with menu

// Timer variable (stores frames left)
unsigned int timer_frames = 0;



//--------------------------------------------------------//
//                  FUNCTION PROTOTYPES                   //
//--------------------------------------------------------//

//--------------- Initialization and Setup ---------------//

void setup_graphics();
void setup_audio();

void setup_menu();
void setup_game();
void setup_death();
void setup_credits();

void update_menu();
void update_game();
void update_death();

void check_game_state();


void initialize_player();
void initialize_pipes();


void scroll_horizontal();


void update_player();

void handle_player_input();
void apply_player_physics();
void handle_player_movement(); 

void player_jump();

int apply_subpixel_movement(int *position, int *subpixel, int velocity);




void update_pipes();
void draw_pipe(byte pipe_index, int x_pos, byte gap_index, byte slice_progress);

void put_color(word addr);
word nametable_to_attribute_addr(word a);

bool check_collision();


void start_timer(unsigned int wait_time);
bool timer_done();

//---------------- HUD and Visual Effects ----------------//

void fade_in();
void fade_out();
void flash_screen();

void update_score_display();




void seed_rng();

//--------------------------------------------------------//
//                       FUNCTIONS                        //
//--------------------------------------------------------//



//  Initializes PPU and graphics-related settings.
 
void setup_graphics() {
  ppu_off();               // Disable rendering while setting up
  oam_clear();             // Clear OAM buffer, hiding all sprites
  pal_all(PALETTE);        // Load predefined color palette for background and sprites
  bank_bg(0);              // Use CHR bank 0 for background tiles
  bank_spr(1);             // Use CHR bank 1 for sprite tiles
  
  vrambuf_clear();          // Clear VRAM buffer
  set_vram_update(updbuf);  // Link VRAM update buffer
}


void setup_audio() {
  famitone_init(music_game); // Initialize FamiTone with the menu music by default
  sfx_init(sfx_game);             // Initialize sound effects
  nmi_set_callback(famitone_update); // Set FamiTone update function to be called during NMI
  music_play(0);
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

}



// Initializes the player’s position.
 
void initialize_player() {
   player_y = PLAYER_INIT_Y; // Set player’s initial Y position
   player_alive = true;

   player_y_sub = 0;                 // player y position (subpixel)
   player_y_vel = 0;                 // player y velocity in subpixels
}



// Updates all aspects of the player’s movement each frame.
 
void update_player() {
  apply_player_physics();    // Apply gravity and movement physics
  handle_player_movement();  // Updates player movement using subpixel calculations
}


// Handles input from the controller.

void handle_player_input() {
  char controller_1 = pad_trigger(0);  // Read input from player 1
  
  if ((controller_1 & PAD_A) && (player_alive)) {  
    player_jump();  // Initiate jump if A is pressed
  }
  
  if (!player_alive) {// Wait for Start button to return to the main menu
      if (controller_1 & PAD_START) {
          sfx_play(5,5);
          fade_out(); // Fade out before changing the state

          game_state = STATE_DEATH; // Switch back to menu
          setup_death(); // Load menu nametable
          fade_in();

      }
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
 
   sfx_play(2, 1);
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





void initialize_pipes() {
    byte i;  
    for (i = 0; i < MAX_PIPES; i++) {
      pipes[i].slice_progress = PIPE_WIDTH_TILES;          // Reset drawing progress
      pipes[i].x = 0;
      pipes[i].gap = 0;
      pipes[i].passed = false;
    }
  
    scroll_x = 0;                     // x scroll position
    scroll_x_sub = 0;                 // x scroll position (subpixel)
    scroll_x_vel = SCROLL_X_SPEED;
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
      pipes[0].passed = false;
      
    }

    if ((scroll_x == 120 || scroll_x == 376) && pipes[1].slice_progress == PIPE_WIDTH_TILES) { // Pipe 1 (x = 128) spawns
      pipes[1].x = PIPE_SPAWN_X + 8;
      pipes[1].gap = rand8() % 5;
      pipes[1].slice_progress = 0;          // Reset drawing progress
      pipes[1].passed = false;
    }
    
}

// Fills pipe_buffer with the correct tiles for the pipe slice
void draw_pipe(byte pipe_index, int x_pos, byte gap_index, byte slice_progress) {
    byte i;
	
    byte tile_x;
  	
    byte background_tiles = 0;
    byte offset = 0;
  
    byte upper_height;
    byte lower_height;
  
    // Variables to handle number of tiles in each pipe height
    int n_pipe_body_sky_upper;
    int n_sky;
    int n_background;
    int n_pipe_body_sky_lower;
    int n_pipe_body_city;
  
    byte cont = 0;
  
  
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
  
    // Clear the nametable buffer before writing
    memset(pipe_buffer, 0, sizeof(pipe_buffer));
  
    memset(color_buffer, 0, sizeof(color_buffer)); // default: all palette 0
  
  
    upper_height = pipe_heights[gap_index][0];
    lower_height = pipe_heights[gap_index][1];
  
    if (upper_height >= lower_height) {
	background_tiles = (upper_height-lower_height)/2;
        offset = 1;
    }
  
    // Variables to handle number of tiles in each pipe height
    n_pipe_body_sky_upper = upper_height - 1;
    n_sky = PIPE_GAP_TILES - background_tiles;
    n_background = background_tiles;
    n_pipe_body_sky_lower = lower_height - CITY_HEIGHT - 1;
    n_pipe_body_city = CITY_HEIGHT - 1 - background_tiles - offset;
  
  
    //-------------------------------------------------------------
  
    // --- Draw Upper Pipe Body Sky ---
    for (i = 0; i < n_pipe_body_sky_upper; i++) {
        pipe_buffer[cont] = pipe_metatiles[PIPE_BODY_SKY][slice_progress];
        cont++;
    }
  
    // --- Draw Upper Pipe Brim ---
    pipe_buffer[cont] = pipe_metatiles[PIPE_BRIM][slice_progress];
    cont++;
  
    // --- Draw Sky ---
    for (i = 0; i < n_sky; i++) {
        pipe_buffer[cont] = pipe_metatiles[SKY][slice_progress];
        cont++;
    }
  
    // --- Draw Background ---
    for (i = 0; i < n_background; i++) {
        pipe_buffer[cont] = pipe_metatiles[CLOUDS_OUTLINE + i][slice_progress];
        cont++;
    }
  
    // --- Draw Lower Pipe Brim ---
    pipe_buffer[cont] = pipe_metatiles[PIPE_BRIM][slice_progress];
    cont++;
  
    // --- Draw Lower Pipe Body Sky ---
    for (i = 0; i < n_pipe_body_sky_lower; i++) {
        pipe_buffer[cont] = pipe_metatiles[PIPE_BODY_SKY][slice_progress];
        cont++;
    }
  
    // --- Draw Lower Pipe Body City ---
    for (i = 0; i < n_pipe_body_city; i++) {
        pipe_buffer[cont] = pipe_metatiles[PIPE_BODY_CITY][slice_progress];
        cont++;
    }
  
    // --- Draw Lower Pipe Body Floor ---
    pipe_buffer[cont] = pipe_metatiles[PIPE_BODY_FLOOR][slice_progress];
    cont++;
  
   //-------------------------------------------------------------
  
    // Clear color buffer first
    for (i = 0; i < PLAYROWS / 4; i++) {
        color_buffer[i] = 0x00;
    }

    // For each tile row in the column
    for (i = 0; i < cont; i++) {
        // Check if it's a background tile that needs palette 3
        if (i >= n_pipe_body_sky_upper + 1 + n_sky &&
            i < n_pipe_body_sky_upper + 1 + n_sky + n_background) {

            byte attr_index = i / 4;
            byte row_in_attr = i % 4;

            // These are the quadrant bit shifts
            byte shift_left  = (row_in_attr < 2) ? 0 : 4; // TL or BL
            byte shift_right = (row_in_attr < 2) ? 2 : 6; // TR or BR

            // Set palette 3 (0b11) for both left and right halves
            color_buffer[attr_index] &= ~((0b11 << shift_left) | (0b11 << shift_right));
            color_buffer[attr_index] |=  ((0b11 << shift_left) | (0b11 << shift_right));
        }
    }


  //-----------------------------------------------------------------

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
    return (a & 0x2C00)       // Keep nametable origin
         | 0x3C0              // Start of attribute table
         | ((a >> 4) & 0x38)  // Row within attribute table
         | ((a >> 2) & 0x07); // Column within attribute table
}





// Function to check collision with pipes
bool check_collision() {
    byte i;
    bool collision = false;

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
          
            // Increase score only if the player has just passed the pipe
            if ((pipes[i].x < PLAYER_X) && (!pipes[i].passed)) {  
                pipes[i].passed = true;  //  Mark pipe as passed
                score++;  //  Increase score only once
                sfx_play(7,4);
                update_score_display();  //  Update score display
            }

            // Check if the bird is within the gap
            if (bird_hitbox_top < gap_y || bird_hitbox_bottom > gap_y + 56) {
                collision = true;  // Collision detected, player dies
            }
        }
    }
  
    return collision;  // No collision
}


void update_score_display() {
    char score_str[3];  // Buffer to hold the score as a string

    // Convert score to ASCII (0-9 tiles match ASCII table)
    score_str[2] = '0' + (score % 10);           // Ones place
    score_str[1] = '0' + ((score / 10) % 10);    // Tens place
    score_str[0] = '0' + ((score / 100) % 10);   // Hundreds place

    // Queue the score update in the VRAM buffer (tiles 8,1 - 10,1)
    vrambuf_put(NTADR_A(8,1), score_str, 3);
}




void seed_rng() {
    set_rand(nesclock());  // Use VBlank counter as the random seed
}


// Start a timer with a given duration (in frames)
void start_timer(unsigned int wait_time) {
    timer_frames = wait_time;
}

// Check if the timer has finished (returns true when done)
bool timer_done() {
    if (timer_frames > 0) {
        timer_frames--;  // Decrease timer every frame
        return false;    // Timer still running
    }
    return true;  // Timer finished
}


//-----------------------------------------------------------------------------//
//                        Fade In/Out And Flash Functions                      //          
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
    start_timer(FLASH_TIME);    // Brief delay for the flash
    if (timer_done) pal_bright(2);        // Restore to intermediate brightness
    start_timer(FLASH_TIME);    // Brief delay for the flash
    if (timer_done) pal_bright(8);        // Restore to intermediate brightness
    start_timer(FLASH_TIME); 
    if (timer_done) pal_bright(2);        // Restore to intermediate brightness
    for (i = 2; i <= 4; ++i) {
        start_timer(FLASH_TIME); 
        if (timer_done) pal_bright(i); // Increase brightness
    }
}





//------------------------------- Setup States ------------------------------------//

// Load the nametable for the menu state
void setup_menu() { 
  ppu_off(); // Turn off rendering to safely update VRAM
  setup_audio();
  vram_adr(NAMETABLE_A);
  vram_write(nametable_title, 1024);
  ppu_on_all(); // Turn rendering back on
}

// Load the nametable for the game state
void setup_game() {
  ppu_off();                         // Turn off rendering to safely update VRAM
  vram_adr(NAMETABLE_A);
  vram_write(nametable_game, 1024);
  
  vram_adr(NAMETABLE_B);
  vram_write(nametable_game, 1024);

  ppu_on_all(); // Turn rendering back on
}

// Load the nametable for the death state
void setup_death() {
  ppu_off(); // Turn off rendering to safely update VRAM
  vram_adr(NAMETABLE_A);
  vram_write(nametable_death, 1024);
  ppu_on_all(); // Turn rendering back on

}

void setup_credits() {
  ppu_off(); // Turn off rendering to safely update VRAM
  pal_bright(0);
  vram_adr(NAMETABLE_A);
  vram_write(nametable_ufabc, 1024);
  ppu_on_all(); // Turn rendering back on
  fade_in();
  delay(80);
  
  fade_out();
  ppu_off();
  vram_adr(NAMETABLE_A);
  vram_write(nametable_another_universe, 1024);
  ppu_on_all();
  fade_in();
  delay(80);
  fade_out();
  
  setup_menu();
  game_state = STATE_MENU;
  fade_in();
}


//------------------------------- Update States ------------------------------------//


// Handle the menu state
void update_menu() {
  char controller_1 = pad_trigger(0);  // Read input from player 1
  // Wait for Start button to begin the game
  if (controller_1 & PAD_START) {
    seed_rng();  // Set a different random seed every playthrough
    
    sfx_play(5,5);
    fade_out(); // Fade out before changing the state
    delay(60);
    
    game_state = STATE_GAME; // Switch to game state
    setup_game(); // Load game nametable

    fade_in(); // Fade in after loading the new state
    initialize_pipes();
    initialize_player();
    score = 0;

  }
}

// Handle the game state
void update_game() {
    char oam_id = 0; // variable to draw sprites
    char controller_1 = pad_trigger(0);  // Read input from player 1
    
    oam_id = oam_spr(30, 30, 0x11E, OAM_BEHIND, 0); // sprite zero for splitting screen
    
    ppu_wait_nmi();   // wait for NMI to ensure previous frame finished
    vrambuf_clear();  // Clear VRAM buffer each frame immediately after NMI
  
    handle_player_input();     // Reads and Process controller input
    
    // Sets the scroll register using `split()`.
    split(scroll_x, 0);  // Update the scroll position
  
    oam_id = oam_meta_spr(PLAYER_X, player_y, oam_id, bird); // draw flappy bird metasprite
 
    update_player();
  
    if (player_alive) scroll_horizontal();
           
    update_pipes();       // Update and spawn pipes

    if ((player_alive) && (check_collision())) {
      // Handle death (e.g., stop movement, show game over screen)
      player_alive = false;
      music_stop();
      sfx_play(4, 1);    
      scroll_x_sub = 0;     // x scroll position (subpixel)
      scroll_x_vel = 0;    // x scroll velocity in subpixels
    } 
    
}


// Handle the death state
void update_death() {
    char controller_1 = pad_trigger(0);  // Read input from player 1
    
 
    // Wait for Start button to return to the main menu
    if (controller_1 & PAD_START) {
        sfx_play(5,5);
        fade_out(); // Fade out before changing the state

        game_state = STATE_MENU; // Switch back to menu
        setup_menu(); // Load menu nametable
        fade_in();

    }
  
}

//------------------------------- Check State ------------------------------------//

// Check the current game state and update accordingly
void check_game_state() {
    if (game_state == STATE_MENU) {
        update_menu();
    } else if (game_state == STATE_GAME) {
        update_game();
    } else if (game_state == STATE_DEATH) {
        update_death();
    }
}


//--------------------------------------------------------//
//                    MAIN GAME LOOP                      //
//--------------------------------------------------------//

void main(void)
{ 

  setup_graphics();
  
  setup_credits();
  
  // infinite main loop
  while(1) {
      check_game_state(); // Check and update based on the game state
  }

}









