#include "sram.h"
#include <VGAX.h>

int ram_addr = 0;
int n_entries = 500;

VGAX vga;

void fill_ram()
{
  byte vga_data[VGAX_HEIGHT * VGAX_BWIDTH];
  memset(vga_data, 0, sizeof(vga_data));

  for (int i = 0; i < n_entries; i++) {
    byte x = rand() % 30 * 4;
    byte y = rand() % VGAX_HEIGHT;
    byte clr = rand() % 3 + 1;

    //https://github.com/smaffer/vgax/blob/master/VGAX.h
    byte *p = vga_data + y * VGAX_BWIDTH + (x >> 2);
    byte bitpos = 6 - (x & 3) * 2;
    *p = (*p & ~(3 << bitpos)) | clr << bitpos;
  }

  initRam();
  writeRam(vga_data, ram_addr, sizeof(vga_data));
}

void setup()
{
  randomSeed(analogRead(A1));
  vga.begin();
  initRam();
  fill_ram();
  readRam(vgaxfb, ram_addr, VGAX_HEIGHT * VGAX_BWIDTH);
}


int init_wait = 100;
int wait = init_wait;
int memptr = 0;
byte vga_mess(bool hyper)
{
  if (hyper) {
    memptr += random(5) - 10;
  }
  else if (--wait == 0) {
    wait = init_wait;
    memptr += random(5) - 10;
  }
  memcpy(vgaxfb, memptr, VGAX_HEIGHT * VGAX_BWIDTH);
}

void loop()
{
  byte x = rand() % VGAX_WIDTH;
  byte y = rand() % VGAX_HEIGHT;
  byte pix = vga.getpixel(x, y);

  int potVal = analogRead(A0);

  vga_mess(false);

  

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
