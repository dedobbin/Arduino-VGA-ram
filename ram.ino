#include "sram.h"
#include <VGAX.h>

int ram_addr = 0;
int n_entries = 5;

#define ANALOG_IN A0
#define BUTTON_IN 8

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
  pinMode(BUTTON_IN, INPUT);
  vga.begin();
  initRam();
  //fill_ram();
  readRam(vgaxfb, ram_addr, VGAX_HEIGHT * VGAX_BWIDTH);
}

int memptr = 0;
int wait = 0;
int threshold = rand()%200; 

byte mess(bool hyper)
{
  if (hyper){
    memptr++;
  }
  else if (wait ++ > threshold){
    wait = 0;
    memptr++;
    threshold+=6 - (rand() % 20);
    if (threshold <= 0){
      threshold = rand()%200;
    }
  }
  memcpy(vgaxfb, memptr, VGAX_HEIGHT * VGAX_BWIDTH);
}

bool put_pixel(byte x,byte y, byte color)
{
    if (x >= 0 && x < VGAX_WIDTH && y >= 0 && y < VGAX_HEIGHT) {
      vga.putpixel(x, y, color);
      return true;
    }
    return false;
}

bool put_pixels_arround(byte x, byte y, byte color, byte dist)
{
  byte dimension = dist * 2 + 1;
  for (int i=0;i<dimension;i++){
    for (int j=0;j<dimension;j++){
      put_pixel(i+x-dist, j+y-dist, color);
    }
  }
}

void loop()
{

  if (digitalRead(BUTTON_IN)){
    mess(true);
  }
  
  int val = analogRead(ANALOG_IN);
  if (val < 500 && val > 20) {
    // constantly restarting gives intense glitchy output
    vga.begin();
  }
  
  if (val > 500) {
    mess(false);
  }  

  //TODO: maybe something faster? 
  byte x = rand() % VGAX_WIDTH;
  byte y = rand() % VGAX_HEIGHT;
  byte pix = vga.getpixel(x, y);
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

    if (rand() % 10 == 0) {
      vga.putpixel(x, y, rand() % 3 + 1);
    }
  }

  //  for (int y=0;y<VGAX_HEIGHT;y++){
  //    for (int x=0;x<VGAX_WIDTH;x++){
  //      byte pix = vga.getpixel(x, y);
  //      switch(pix){
  //        case 0: //black
  //          break;
  //        case 1: //red
  //          break;
  //        case 2: //green
  //          break;
  //        case 3: // yellow
  //          break;
  //      }
  //    }
  //  }
}
