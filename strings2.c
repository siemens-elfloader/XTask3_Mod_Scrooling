#include <swilib.h>
#include "strings2.h"
#define wslen(ws) ws->wsbody[0]

typedef struct
{
  unsigned short u;
  char win;
} TUNICODE2CHAR;

const TUNICODE2CHAR unicode2char[]=
{
  0x410,0xC0, // A
  0x411,0xC1, // A
  0x412,0xC2, // A
  0x413,0xC3, // A
  0x414,0xC4, // A
  0x415,0xC5, // A
  0x416,0xC6, // ?
  0x417,0xC7, // C
  0x418,0xC8, // E
  0x419,0xC9, // E
  0x41A,0xCA, // E
  0x41B,0xCB, // E
  0x41C,0xCC, // I
  0x41D,0xCD, // I
  0x41E,0xCE, // I
  0x41F,0xCF, // I
  0x420,0xD0, // ?
  0x421,0xD1, // N
  0x422,0xD2, // O
  0x423,0xD3, // O
  0x424,0xD4, // O
  0x425,0xD5, // O
  0x426,0xD6, // O
  0x427,0xD7, // ?
  0x428,0xD8, // O
  0x429,0xD9, // U
  0x42A,0xDA, // U
  0x42B,0xDB, // U
  0x42C,0xDC, // U
  0x42D,0xDD, // Y
  0x42E,0xDE, // ?
  0x42F,0xDF, // ?
  0x402,0x80, // _
  0x403,0x81, // _
  0x409,0x8A, // _
  0x40A,0x8C, // _
  0x40C,0x8D, // _
  0x40B,0x8E, // _
  0x40F,0x8F, // _
  0x40E,0xA1, // ? ...*
  0x408,0xA3, // _
  0x409,0xA5, // _ .*..
  0x401,0xA8, // ?
  0x404,0xAA, // ?
  0x407,0xAF, // ?
  0x406,0xB2, // _ .*..
  0x405,0xBD, // _
  0x430,0xE0, // a
  0x431,0xE1, // a
  0x432,0xE2, // a
  0x433,0xE3, // ?
  0x434,0xE4, // a
  0x435,0xE5, // a
  0x436,0xE6, // ?
  0x437,0xE7, // c
  0x438,0xE8, // e
  0x439,0xE9, // e
  0x43A,0xEA, // e
  0x43B,0xEB, // e
  0x43C,0xEC, // i
  0x43D,0xED, // i
  0x43E,0xEE, // i
  0x43F,0xEF, // i
  0x440,0xF0, // ?
  0x441,0xF1, // n
  0x442,0xF2, // o
  0x443,0xF3, // o
  0x444,0xF4, // o
  0x445,0xF5, // o
  0x446,0xF6, // o
  0x447,0xF7, // ?
  0x448,0xF8, // o
  0x449,0xF9, // u
  0x44A,0xFA, // u
  0x44B,0xFB, // u
  0x44C,0xFC, // u
  0x44D,0xFD, // y
  0x44E,0xFE, // ?
  0x44F,0xFF, // y
  0x452,0x90, // _
  0x453,0x83, // _
  0x459,0x9A, // _
  0x45A,0x9C, // _
  0x45C,0x9D, // _
  0x45B,0x9E, // _
  0x45F,0x9F, // _
  0x45E,0xA2, // ? ...*
  0x458,0xBC, // _
  0x491,0xB4, // _ .*..
  0x451,0xB8, // ?
  0x454,0xBA, // ?
  0x457,0xBF, // ?
  0x456,0xB3, // _ .*..
  0x455,0xBE, // _
  0x102,0xC3,
  0x103,0xE3,
  0x104,0xA5,
  0x105,0xB9,
  0x106,0xC6,
  0x107,0xE6,
  0x10C,0xC8,
  0x10D,0xE8,
  0x10E,0xCF,
  0x10F,0xEF,
  0x110,0xD0,
  0x111,0xF0,
  0x118,0xCA,
  0x119,0xEA,
  0x11A,0xCC,
  0x11B,0xEC,
  0x139,0xC5,
  0x13A,0xE5,
  0x13D,0xBC,
  0x13E,0xBE,
  0x141,0xA3,
  0x142,0xB3,
  0x143,0xD1,
  0x144,0xF1,
  0x147,0xD2,
  0x148,0xF2,
  0x154,0xC0,
  0x155,0xE0,
  0x158,0xD8,
  0x15A,0x8C,
  0x15B,0x9C,
  0x15E,0xAA,
  0x160,0x8A,
  0x161,0x9A,
  0x162,0xDE,
  0x163,0xFE,
  0x164,0x8D,
  0x165,0x9D,
  0x168,0xDB,
  0x169,0xFB,
  0x16E,0xD9,
  0x16F,0xF9,
  0x179,0x8F,
  0x17A,0x9F,
  0x17B,0xAF,
  0x17C,0xBF,
  0x17D,0x8E,
  0x17E,0x9E,
  0x0000,0
};

char char16to8(unsigned int c)
{
  const TUNICODE2CHAR *p=unicode2char;
  unsigned int i;
  if (c<128) return(c);
  while((i=p->u))
  {
    if (c==i)
    {
      return(p->win);
    }
    p++;
  }
  c&=0xFF;
  if (c<32) return(' ');
  return(c);
}

int convWS_to_ANSI(WSHDR *ws, char *buf)
{
  unsigned int sWs=ws->wsbody[0];
  int p=0;
  unsigned int cWs;
  while((p<255)&&(p<sWs))
  {
    cWs=ws->wsbody[p+1];
    buf[p]=char16to8(cWs);
    p++;
  }
  buf[p] = 0;
  return p;
}
