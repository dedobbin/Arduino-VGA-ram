#include "sram.h"
#include "audio.h"
#include <VGAX.h>

int ram_addr = 0;
int n_entries = 500;

VGAX vga;

void fill_ram()
{
  const int VGA_BWIDTH = 30;
  const int VGA_HEIGHT = 60;
  const int VGA_WIDTH = VGA_BWIDTH * 4;
  byte vga_data[VGA_HEIGHT * VGA_BWIDTH];
  memset(vga_data, 0, sizeof(vga_data));

  for (int i = 0; i < n_entries; i++) {
    byte x = rand() % 30 * 4;
    byte y = rand() % VGA_HEIGHT;
    byte clr = rand() % 3 + 1;

    //https://github.com/smaffer/vgax/blob/master/VGAX.h
    byte *p = vga_data + y * VGA_BWIDTH + (x >> 2);
    byte bitpos = 6 - (x & 3) * 2;
    *p = (*p & ~(3 << bitpos)) | clr << bitpos;
  }

  initRam();
  writeRam(vga_data, ram_addr, sizeof(vga_data));
}

void setup()
{
  randomSeed(analogRead(A1));
  setup_clock4();
  vga.begin();
  initRam();
  //fill_ram();
  readRam(vgaxfb, ram_addr, VGAX_HEIGHT * VGAX_BWIDTH);
}

int memptr = 0;
int wait = 0;
int threshold = 100; 

byte audio_mem(bool hyper)
{
  if (hyper){
    memptr += random(5)-10;
  }
  else if (wait ++ > threshold){
    wait = 0;
    memptr++;
    threshold+=6 - (random(20));
    if (threshold <= 0){
      threshold = random(100);
    }
  }
  memcpy(vgaxfb, memptr, VGAX_HEIGHT * VGAX_BWIDTH);
}

int last_used_audio_val = 0;
bool stay = false;
int stay_n = 40;
void loop()
{
  byte x = rand() % VGAX_WIDTH;
  byte y = rand() % VGAX_HEIGHT;
  byte pix = vga.getpixel(x, y);


  if (last_used_audio_val != last_audio_val){
    if (last_audio_val == 10){
      memset(vgaxfb, 0,  VGAX_HEIGHT * VGAX_BWIDTH);
    }
    
//    if (last_audio_val > 9 || stay) {
//      stay = true;
//      audio_mem(true);
//      stay_n --;
//      if (stay_n == 0){
//        stay_n = 10;
//        stay = false;
//      }
//    }
//
//    if (last_audio_val > 10){
//       audio_mem(false);
//    }
//
//    
//    if (last_audio_val > 10) {      
//      // constantly restarting gives intense glitchy output, also seems to corrupt something because it can result in smaller glitches which stay until vga.begin() is called again?
//      vga.begin();
//      readRam(vgaxfb, ram_addr, VGAX_HEIGHT * VGAX_BWIDTH);
//    }
    
    last_used_audio_val = last_audio_val;
  }

  //TODO: maybe something faster? would also be nice if speed based on audio
  if (pix != 0) {
    if (x - 1 >= 0) {
      vga.putpixel(x - 1, y, pix);
    }
    if (x + 1 < VGAX_WIDTH) {
      vga.putpixel(x + 1, y, pix);
    }
    if (y - 1 >= 0) {
      vga.putpixel(x, y - 1, pix);
    }
    if (y + 1 < VGAX_HEIGHT) {
      vga.putpixel(x, y + 1, pix);
    }

    if (random(10) == 0) {
      vga.putpixel(x, y, rand() % 3 + 1);
    }
  }

//    for (int i=0;i<20;i++){
//      vga.putpixel(rand()%VGAX_WIDTH,rand()%VGAX_HEIGHT, 0);
//    }


//    for (int y=0;y<VGAX_HEIGHT;y++){
//      for (int x=0;x<VGAX_WIDTH;x++){
//        byte pix = vga.getpixel(x, y);
//        switch(pix){
//          case 0: //black
//            break;
//          case 1: //red
//            break;
//          case 2: //green
//            break;
//          case 3: // yellow
//            break;
//        }
//      }
//    }
}
