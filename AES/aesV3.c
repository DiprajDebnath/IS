#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// https://en.wikipedia.org/wiki/Rijndael_S-box
unsigned char s_box[256] = {
   0X63, 0X7C, 0X77, 0X7B, 0XF2, 0X6B, 0X6F, 0XC5, 0X30, 0X01, 0X67, 0X2B, 0XFE, 0XD7, 0XAB, 0X76,
   0XCA, 0X82, 0XC9, 0X7D, 0XFA, 0X59, 0X47, 0XF0, 0XAD, 0XD4, 0XA2, 0XAF, 0X9C, 0XA4, 0X72, 0XC0,
   0XB7, 0XFD, 0X93, 0X26, 0X36, 0X3F, 0XF7, 0XCC, 0X34, 0XA5, 0XE5, 0XF1, 0X71, 0XD8, 0X31, 0X15,
   0X04, 0XC7, 0X23, 0XC3, 0X18, 0X96, 0X05, 0X9A, 0X07, 0X12, 0X80, 0XE2, 0XEB, 0X27, 0XB2, 0X75,
   0X09, 0X83, 0X2C, 0X1A, 0X1B, 0X6E, 0X5A, 0XA0, 0X52, 0X3B, 0XD6, 0XB3, 0X29, 0XE3, 0X2F, 0X84,
   0X53, 0XD1, 0X00, 0XED, 0X20, 0XFC, 0XB1, 0X5B, 0X6A, 0XCB, 0XBE, 0X39, 0X4A, 0X4C, 0X58, 0XCF,
   0XD0, 0XEF, 0XAA, 0XFB, 0X43, 0X4D, 0X33, 0X85, 0X45, 0XF9, 0X02, 0X7F, 0X50, 0X3C, 0X9F, 0XA8,
   0X51, 0XA3, 0X40, 0X8F, 0X92, 0X9D, 0X38, 0XF5, 0XBC, 0XB6, 0XDA, 0X21, 0X10, 0XFF, 0XF3, 0XD2,
   0XCD, 0X0C, 0X13, 0XEC, 0X5F, 0X97, 0X44, 0X17, 0XC4, 0XA7, 0X7E, 0X3D, 0X64, 0X5D, 0X19, 0X73,
   0X60, 0X81, 0X4F, 0XDC, 0X22, 0X2A, 0X90, 0X88, 0X46, 0XEE, 0XB8, 0X14, 0XDE, 0X5E, 0X0B, 0XDB,
   0XE0, 0X32, 0X3A, 0X0A, 0X49, 0X06, 0X24, 0X5C, 0XC2, 0XD3, 0XAC, 0X62, 0X91, 0X95, 0XE4, 0X79,
   0XE7, 0XC8, 0X37, 0X6D, 0X8D, 0XD5, 0X4E, 0XA9, 0X6C, 0X56, 0XF4, 0XEA, 0X65, 0X7A, 0XAE, 0X08,
   0XBA, 0X78, 0X25, 0X2E, 0X1C, 0XA6, 0XB4, 0XC6, 0XE8, 0XDD, 0X74, 0X1F, 0X4B, 0XBD, 0X8B, 0X8A,
   0X70, 0X3E, 0XB5, 0X66, 0X48, 0X03, 0XF6, 0X0E, 0X61, 0X35, 0X57, 0XB9, 0X86, 0XC1, 0X1D, 0X9E,
   0XE1, 0XF8, 0X98, 0X11, 0X69, 0XD9, 0X8E, 0X94, 0X9B, 0X1E, 0X87, 0XE9, 0XCE, 0X55, 0X28, 0XDF,
   0X8C, 0XA1, 0X89, 0X0D, 0XBF, 0XE6, 0X42, 0X68, 0X41, 0X99, 0X2D, 0X0F, 0XB0, 0X54, 0XBB, 0X16
};
const char inv_s_box[256] = {
  0X52, 0X09, 0X6A, 0XD5, 0X30, 0X36, 0XA5, 0X38, 0XBF, 0X40, 0XA3, 0X9E, 0X81, 0XF3, 0XD7, 0XFB,
  0X7C, 0XE3, 0X39, 0X82, 0X9B, 0X2F, 0XFF, 0X87, 0X34, 0X8E, 0X43, 0X44, 0XC4, 0XDE, 0XE9, 0XCB,
  0X54, 0X7B, 0X94, 0X32, 0XA6, 0XC2, 0X23, 0X3D, 0XEE, 0X4C, 0X95, 0X0B, 0X42, 0XFA, 0XC3, 0X4E,
  0X08, 0X2E, 0XA1, 0X66, 0X28, 0XD9, 0X24, 0XB2, 0X76, 0X5B, 0XA2, 0X49, 0X6D, 0X8B, 0XD1, 0X25,
  0X72, 0XF8, 0XF6, 0X64, 0X86, 0X68, 0X98, 0X16, 0XD4, 0XA4, 0X5C, 0XCC, 0X5D, 0X65, 0XB6, 0X92,
  0X6C, 0X70, 0X48, 0X50, 0XFD, 0XED, 0XB9, 0XDA, 0X5E, 0X15, 0X46, 0X57, 0XA7, 0X8D, 0X9D, 0X84,
  0X90, 0XD8, 0XAB, 0X00, 0X8C, 0XBC, 0XD3, 0X0A, 0XF7, 0XE4, 0X58, 0X05, 0XB8, 0XB3, 0X45, 0X06,
  0XD0, 0X2C, 0X1E, 0X8F, 0XCA, 0X3F, 0X0F, 0X02, 0XC1, 0XAF, 0XBD, 0X03, 0X01, 0X13, 0X8A, 0X6B,
  0X3A, 0X91, 0X11, 0X41, 0X4F, 0X67, 0XDC, 0XEA, 0X97, 0XF2, 0XCF, 0XCE, 0XF0, 0XB4, 0XE6, 0X73,
  0X96, 0XAC, 0X74, 0X22, 0XE7, 0XAD, 0X35, 0X85, 0XE2, 0XF9, 0X37, 0XE8, 0X1C, 0X75, 0XDF, 0X6E,
  0X47, 0XF1, 0X1A, 0X71, 0X1D, 0X29, 0XC5, 0X89, 0X6F, 0XB7, 0X62, 0X0E, 0XAA, 0X18, 0XBE, 0X1B,
  0XFC, 0X56, 0X3E, 0X4B, 0XC6, 0XD2, 0X79, 0X20, 0X9A, 0XDB, 0XC0, 0XFE, 0X78, 0XCD, 0X5A, 0XF4,
  0X1F, 0XDD, 0XA8, 0X33, 0X88, 0X07, 0XC7, 0X31, 0XB1, 0X12, 0X10, 0X59, 0X27, 0X80, 0XEC, 0X5F,
  0X60, 0X51, 0X7F, 0XA9, 0X19, 0XB5, 0X4A, 0X0D, 0X2D, 0XE5, 0X7A, 0X9F, 0X93, 0XC9, 0X9C, 0XEF,
  0XA0, 0XE0, 0X3B, 0X4D, 0XAE, 0X2A, 0XF5, 0XB0, 0XC8, 0XEB, 0XBB, 0X3C, 0X83, 0X53, 0X99, 0X61,
  0X17, 0X2B, 0X04, 0X7E, 0XBA, 0X77, 0XD6, 0X26, 0XE1, 0X69, 0X14, 0X63, 0X55, 0X21, 0X0C, 0X7D
};


// Lookup table values for Galois field multiplication
// https://en.wikipedia.org/wiki/Rijndael_MixColumns
// GF(256) 2x multiplication lookup table used for mix_columns operation
unsigned char mul2[256] = {
  0X00,0X02,0X04,0X06,0X08,0X0A,0X0C,0X0E,0X10,0X12,0X14,0X16,0X18,0X1A,0X1C,0X1E,
  0X20,0X22,0X24,0X26,0X28,0X2A,0X2C,0X2E,0X30,0X32,0X34,0X36,0X38,0X3A,0X3C,0X3E,
  0X40,0X42,0X44,0X46,0X48,0X4A,0X4C,0X4E,0X50,0X52,0X54,0X56,0X58,0X5A,0X5C,0X5E,
  0X60,0X62,0X64,0X66,0X68,0X6A,0X6C,0X6E,0X70,0X72,0X74,0X76,0X78,0X7A,0X7C,0X7E,
  0X80,0X82,0X84,0X86,0X88,0X8A,0X8C,0X8E,0X90,0X92,0X94,0X96,0X98,0X9A,0X9C,0X9E,
  0XA0,0XA2,0XA4,0XA6,0XA8,0XAA,0XAC,0XAE,0XB0,0XB2,0XB4,0XB6,0XB8,0XBA,0XBC,0XBE,
  0XC0,0XC2,0XC4,0XC6,0XC8,0XCA,0XCC,0XCE,0XD0,0XD2,0XD4,0XD6,0XD8,0XDA,0XDC,0XDE,
  0XE0,0XE2,0XE4,0XE6,0XE8,0XEA,0XEC,0XEE,0XF0,0XF2,0XF4,0XF6,0XF8,0XFA,0XFC,0XFE,
  0X1B,0X19,0X1F,0X1D,0X13,0X11,0X17,0X15,0X0B,0X09,0X0F,0X0D,0X03,0X01,0X07,0X05,
  0X3B,0X39,0X3F,0X3D,0X33,0X31,0X37,0X35,0X2B,0X29,0X2F,0X2D,0X23,0X21,0X27,0X25,
  0X5B,0X59,0X5F,0X5D,0X53,0X51,0X57,0X55,0X4B,0X49,0X4F,0X4D,0X43,0X41,0X47,0X45,
  0X7B,0X79,0X7F,0X7D,0X73,0X71,0X77,0X75,0X6B,0X69,0X6F,0X6D,0X63,0X61,0X67,0X65,
  0X9B,0X99,0X9F,0X9D,0X93,0X91,0X97,0X95,0X8B,0X89,0X8F,0X8D,0X83,0X81,0X87,0X85,
  0XBB,0XB9,0XBF,0XBD,0XB3,0XB1,0XB7,0XB5,0XAB,0XA9,0XAF,0XAD,0XA3,0XA1,0XA7,0XA5,
  0XDB,0XD9,0XDF,0XDD,0XD3,0XD1,0XD7,0XD5,0XCB,0XC9,0XCF,0XCD,0XC3,0XC1,0XC7,0XC5,
  0XFB,0XF9,0XFF,0XFD,0XF3,0XF1,0XF7,0XF5,0XEB,0XE9,0XEF,0XED,0XE3,0XE1,0XE7,0XE5
};
// GF(256) 3x multiplication lookup table used for mix_columns operation
unsigned char mul3[256] = {
  0X00,0X03,0X06,0X05,0X0C,0X0F,0X0A,0X09,0X18,0X1B,0X1E,0X1D,0X14,0X17,0X12,0X11,
  0X30,0X33,0X36,0X35,0X3C,0X3F,0X3A,0X39,0X28,0X2B,0X2E,0X2D,0X24,0X27,0X22,0X21,
  0X60,0X63,0X66,0X65,0X6C,0X6F,0X6A,0X69,0X78,0X7B,0X7E,0X7D,0X74,0X77,0X72,0X71,
  0X50,0X53,0X56,0X55,0X5C,0X5F,0X5A,0X59,0X48,0X4B,0X4E,0X4D,0X44,0X47,0X42,0X41,
  0XC0,0XC3,0XC6,0XC5,0XCC,0XCF,0XCA,0XC9,0XD8,0XDB,0XDE,0XDD,0XD4,0XD7,0XD2,0XD1,
  0XF0,0XF3,0XF6,0XF5,0XFC,0XFF,0XFA,0XF9,0XE8,0XEB,0XEE,0XED,0XE4,0XE7,0XE2,0XE1,
  0XA0,0XA3,0XA6,0XA5,0XAC,0XAF,0XAA,0XA9,0XB8,0XBB,0XBE,0XBD,0XB4,0XB7,0XB2,0XB1,
  0X90,0X93,0X96,0X95,0X9C,0X9F,0X9A,0X99,0X88,0X8B,0X8E,0X8D,0X84,0X87,0X82,0X81,
  0X9B,0X98,0X9D,0X9E,0X97,0X94,0X91,0X92,0X83,0X80,0X85,0X86,0X8F,0X8C,0X89,0X8A,
  0XAB,0XA8,0XAD,0XAE,0XA7,0XA4,0XA1,0XA2,0XB3,0XB0,0XB5,0XB6,0XBF,0XBC,0XB9,0XBA,
  0XFB,0XF8,0XFD,0XFE,0XF7,0XF4,0XF1,0XF2,0XE3,0XE0,0XE5,0XE6,0XEF,0XEC,0XE9,0XEA,
  0XCB,0XC8,0XCD,0XCE,0XC7,0XC4,0XC1,0XC2,0XD3,0XD0,0XD5,0XD6,0XDF,0XDC,0XD9,0XDA,
  0X5B,0X58,0X5D,0X5E,0X57,0X54,0X51,0X52,0X43,0X40,0X45,0X46,0X4F,0X4C,0X49,0X4A,
  0X6B,0X68,0X6D,0X6E,0X67,0X64,0X61,0X62,0X73,0X70,0X75,0X76,0X7F,0X7C,0X79,0X7A,
  0X3B,0X38,0X3D,0X3E,0X37,0X34,0X31,0X32,0X23,0X20,0X25,0X26,0X2F,0X2C,0X29,0X2A,
  0X0B,0X08,0X0D,0X0E,0X07,0X04,0X01,0X02,0X13,0X10,0X15,0X16,0X1F,0X1C,0X19,0X1A
};
// GF(256) 9x multiplication lookup table used for inv_mix_columns operation
unsigned char mul9[256] = {
  0X00,0X09,0X12,0X1B,0X24,0X2D,0X36,0X3F,0X48,0X41,0X5A,0X53,0X6C,0X65,0X7E,0X77,
  0X90,0X99,0X82,0X8B,0XB4,0XBD,0XA6,0XAF,0XD8,0XD1,0XCA,0XC3,0XFC,0XF5,0XEE,0XE7,
  0X3B,0X32,0X29,0X20,0X1F,0X16,0X0D,0X04,0X73,0X7A,0X61,0X68,0X57,0X5E,0X45,0X4C,
  0XAB,0XA2,0XB9,0XB0,0X8F,0X86,0X9D,0X94,0XE3,0XEA,0XF1,0XF8,0XC7,0XCE,0XD5,0XDC,
  0X76,0X7F,0X64,0X6D,0X52,0X5B,0X40,0X49,0X3E,0X37,0X2C,0X25,0X1A,0X13,0X08,0X01,
  0XE6,0XEF,0XF4,0XFD,0XC2,0XCB,0XD0,0XD9,0XAE,0XA7,0XBC,0XB5,0X8A,0X83,0X98,0X91,
  0X4D,0X44,0X5F,0X56,0X69,0X60,0X7B,0X72,0X05,0X0C,0X17,0X1E,0X21,0X28,0X33,0X3A,
  0XDD,0XD4,0XCF,0XC6,0XF9,0XF0,0XEB,0XE2,0X95,0X9C,0X87,0X8E,0XB1,0XB8,0XA3,0XAA,
  0XEC,0XE5,0XFE,0XF7,0XC8,0XC1,0XDA,0XD3,0XA4,0XAD,0XB6,0XBF,0X80,0X89,0X92,0X9B,
  0X7C,0X75,0X6E,0X67,0X58,0X51,0X4A,0X43,0X34,0X3D,0X26,0X2F,0X10,0X19,0X02,0X0B,
  0XD7,0XDE,0XC5,0XCC,0XF3,0XFA,0XE1,0XE8,0X9F,0X96,0X8D,0X84,0XBB,0XB2,0XA9,0XA0,
  0X47,0X4E,0X55,0X5C,0X63,0X6A,0X71,0X78,0X0F,0X06,0X1D,0X14,0X2B,0X22,0X39,0X30,
  0X9A,0X93,0X88,0X81,0XBE,0XB7,0XAC,0XA5,0XD2,0XDB,0XC0,0XC9,0XF6,0XFF,0XE4,0XED,
  0X0A,0X03,0X18,0X11,0X2E,0X27,0X3C,0X35,0X42,0X4B,0X50,0X59,0X66,0X6F,0X74,0X7D,
  0XA1,0XA8,0XB3,0XBA,0X85,0X8C,0X97,0X9E,0XE9,0XE0,0XFB,0XF2,0XCD,0XC4,0XDF,0XD6,
  0X31,0X38,0X23,0X2A,0X15,0X1C,0X07,0X0E,0X79,0X70,0X6B,0X62,0X5D,0X54,0X4F,0X46
};
// GF(256) 11x multiplication lookup table used for inv_mix_columns operation
unsigned char mul11[256] = {
  0X00,0X0B,0X16,0X1D,0X2C,0X27,0X3A,0X31,0X58,0X53,0X4E,0X45,0X74,0X7F,0X62,0X69,
  0XB0,0XBB,0XA6,0XAD,0X9C,0X97,0X8A,0X81,0XE8,0XE3,0XFE,0XF5,0XC4,0XCF,0XD2,0XD9,
  0X7B,0X70,0X6D,0X66,0X57,0X5C,0X41,0X4A,0X23,0X28,0X35,0X3E,0X0F,0X04,0X19,0X12,
  0XCB,0XC0,0XDD,0XD6,0XE7,0XEC,0XF1,0XFA,0X93,0X98,0X85,0X8E,0XBF,0XB4,0XA9,0XA2,
  0XF6,0XFD,0XE0,0XEB,0XDA,0XD1,0XCC,0XC7,0XAE,0XA5,0XB8,0XB3,0X82,0X89,0X94,0X9F,
  0X46,0X4D,0X50,0X5B,0X6A,0X61,0X7C,0X77,0X1E,0X15,0X08,0X03,0X32,0X39,0X24,0X2F,
  0X8D,0X86,0X9B,0X90,0XA1,0XAA,0XB7,0XBC,0XD5,0XDE,0XC3,0XC8,0XF9,0XF2,0XEF,0XE4,
  0X3D,0X36,0X2B,0X20,0X11,0X1A,0X07,0X0C,0X65,0X6E,0X73,0X78,0X49,0X42,0X5F,0X54,
  0XF7,0XFC,0XE1,0XEA,0XDB,0XD0,0XCD,0XC6,0XAF,0XA4,0XB9,0XB2,0X83,0X88,0X95,0X9E,
  0X47,0X4C,0X51,0X5A,0X6B,0X60,0X7D,0X76,0X1F,0X14,0X09,0X02,0X33,0X38,0X25,0X2E,
  0X8C,0X87,0X9A,0X91,0XA0,0XAB,0XB6,0XBD,0XD4,0XDF,0XC2,0XC9,0XF8,0XF3,0XEE,0XE5,
  0X3C,0X37,0X2A,0X21,0X10,0X1B,0X06,0X0D,0X64,0X6F,0X72,0X79,0X48,0X43,0X5E,0X55,
  0X01,0X0A,0X17,0X1C,0X2D,0X26,0X3B,0X30,0X59,0X52,0X4F,0X44,0X75,0X7E,0X63,0X68,
  0XB1,0XBA,0XA7,0XAC,0X9D,0X96,0X8B,0X80,0XE9,0XE2,0XFF,0XF4,0XC5,0XCE,0XD3,0XD8,
  0X7A,0X71,0X6C,0X67,0X56,0X5D,0X40,0X4B,0X22,0X29,0X34,0X3F,0X0E,0X05,0X18,0X13,
  0XCA,0XC1,0XDC,0XD7,0XE6,0XED,0XF0,0XFB,0X92,0X99,0X84,0X8F,0XBE,0XB5,0XA8,0XA3
};
// GF(256) 13x multiplication lookup table used for inv_mix_columns operation
unsigned char mul13[256] = {
  0X00,0X0D,0X1A,0X17,0X34,0X39,0X2E,0X23,0X68,0X65,0X72,0X7F,0X5C,0X51,0X46,0X4B,
  0XD0,0XDD,0XCA,0XC7,0XE4,0XE9,0XFE,0XF3,0XB8,0XB5,0XA2,0XAF,0X8C,0X81,0X96,0X9B,
  0XBB,0XB6,0XA1,0XAC,0X8F,0X82,0X95,0X98,0XD3,0XDE,0XC9,0XC4,0XE7,0XEA,0XFD,0XF0,
  0X6B,0X66,0X71,0X7C,0X5F,0X52,0X45,0X48,0X03,0X0E,0X19,0X14,0X37,0X3A,0X2D,0X20,
  0X6D,0X60,0X77,0X7A,0X59,0X54,0X43,0X4E,0X05,0X08,0X1F,0X12,0X31,0X3C,0X2B,0X26,
  0XBD,0XB0,0XA7,0XAA,0X89,0X84,0X93,0X9E,0XD5,0XD8,0XCF,0XC2,0XE1,0XEC,0XFB,0XF6,
  0XD6,0XDB,0XCC,0XC1,0XE2,0XEF,0XF8,0XF5,0XBE,0XB3,0XA4,0XA9,0X8A,0X87,0X90,0X9D,
  0X06,0X0B,0X1C,0X11,0X32,0X3F,0X28,0X25,0X6E,0X63,0X74,0X79,0X5A,0X57,0X40,0X4D,
  0XDA,0XD7,0XC0,0XCD,0XEE,0XE3,0XF4,0XF9,0XB2,0XBF,0XA8,0XA5,0X86,0X8B,0X9C,0X91,
  0X0A,0X07,0X10,0X1D,0X3E,0X33,0X24,0X29,0X62,0X6F,0X78,0X75,0X56,0X5B,0X4C,0X41,
  0X61,0X6C,0X7B,0X76,0X55,0X58,0X4F,0X42,0X09,0X04,0X13,0X1E,0X3D,0X30,0X27,0X2A,
  0XB1,0XBC,0XAB,0XA6,0X85,0X88,0X9F,0X92,0XD9,0XD4,0XC3,0XCE,0XED,0XE0,0XF7,0XFA,
  0XB7,0XBA,0XAD,0XA0,0X83,0X8E,0X99,0X94,0XDF,0XD2,0XC5,0XC8,0XEB,0XE6,0XF1,0XFC,
  0X67,0X6A,0X7D,0X70,0X53,0X5E,0X49,0X44,0X0F,0X02,0X15,0X18,0X3B,0X36,0X21,0X2C,
  0X0C,0X01,0X16,0X1B,0X38,0X35,0X22,0X2F,0X64,0X69,0X7E,0X73,0X50,0X5D,0X4A,0X47,
  0XDC,0XD1,0XC6,0XCB,0XE8,0XE5,0XF2,0XFF,0XB4,0XB9,0XAE,0XA3,0X80,0X8D,0X9A,0X97
};
// GF(256) 14x multiplication lookup table used for inv_mix_columns operation
unsigned char mul14[256] = {
  0x00,0x0e,0x1c,0x12,0x38,0x36,0x24,0x2a,0x70,0x7e,0x6c,0x62,0x48,0x46,0x54,0x5a,
  0xe0,0xee,0xfc,0xf2,0xd8,0xd6,0xc4,0xca,0x90,0x9e,0x8c,0x82,0xa8,0xa6,0xb4,0xba,
  0xdb,0xd5,0xc7,0xc9,0xe3,0xed,0xff,0xf1,0xab,0xa5,0xb7,0xb9,0x93,0x9d,0x8f,0x81,
  0x3b,0x35,0x27,0x29,0x03,0x0d,0x1f,0x11,0x4b,0x45,0x57,0x59,0x73,0x7d,0x6f,0x61,
  0xad,0xa3,0xb1,0xbf,0x95,0x9b,0x89,0x87,0xdd,0xd3,0xc1,0xcf,0xe5,0xeb,0xf9,0xf7,
  0x4d,0x43,0x51,0x5f,0x75,0x7b,0x69,0x67,0x3d,0x33,0x21,0x2f,0x05,0x0b,0x19,0x17,
  0x76,0x78,0x6a,0x64,0x4e,0x40,0x52,0x5c,0x06,0x08,0x1a,0x14,0x3e,0x30,0x22,0x2c,
  0x96,0x98,0x8a,0x84,0xae,0xa0,0xb2,0xbc,0xe6,0xe8,0xfa,0xf4,0xde,0xd0,0xc2,0xcc,
  0x41,0x4f,0x5d,0x53,0x79,0x77,0x65,0x6b,0x31,0x3f,0x2d,0x23,0x09,0x07,0x15,0x1b,
  0xa1,0xaf,0xbd,0xb3,0x99,0x97,0x85,0x8b,0xd1,0xdf,0xcd,0xc3,0xe9,0xe7,0xf5,0xfb,
  0x9a,0x94,0x86,0x88,0xa2,0xac,0xbe,0xb0,0xea,0xe4,0xf6,0xf8,0xd2,0xdc,0xce,0xc0,
  0x7a,0x74,0x66,0x68,0x42,0x4c,0x5e,0x50,0x0a,0x04,0x16,0x18,0x32,0x3c,0x2e,0x20,
  0xec,0xe2,0xf0,0xfe,0xd4,0xda,0xc8,0xc6,0x9c,0x92,0x80,0x8e,0xa4,0xaa,0xb8,0xb6,
  0x0c,0x02,0x10,0x1e,0x34,0x3a,0x28,0x26,0x7c,0x72,0x60,0x6e,0x44,0x4a,0x58,0x56,
  0x37,0x39,0x2b,0x25,0x0f,0x01,0x13,0x1d,0x47,0x49,0x5b,0x55,0x7f,0x71,0x63,0x6d,
  0xd7,0xd9,0xcb,0xc5,0xef,0xe1,0xf3,0xfd,0xa7,0xa9,0xbb,0xb5,0x9f,0x91,0x83,0x8d
};


// https://en.wikipedia.org/wiki/Rijndael_key_schedule
unsigned char rcon[256] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
  0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
  0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
  0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
  0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
  0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
  0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
  0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
  0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
  0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
  0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
  0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
  0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
  0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
  0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};



void key_expansion_core(unsigned char* in, unsigned char i) {
  unsigned int * q = (unsigned int *) in;
  // Left rotate bytes
  *q = (*q >> 8 | ((*q & 0xff) << 24));

  in[0] = s_box[in[0]]; in[1] = s_box[in[1]];
  in[2] = s_box[in[2]]; in[3] = s_box[in[3]];

  // RCon XOR
  in[0] ^= rcon[i];
}


void key_expansion(unsigned char* input_key, unsigned char* expanded_keys) {
  // Set first 16 bytes to input_key
  for (int i = 0; i < 16; i++)
    expanded_keys[i] = input_key[i];

  unsigned int bytes_generated = 16;
  int rcon_iteration = 1;
  unsigned char temp[4];

  // Generate the next 160 bytes
  while (bytes_generated < 176) {
    // Read 4 bytes for the core
    for (int i = 0; i < 4; i++)
      temp[i] = expanded_keys[i + bytes_generated - 4];

    // Perform the core once for each 16 byte key
    if (bytes_generated % 16 == 0)
      key_expansion_core(temp, rcon_iteration++);

    // XOR temp with [bytes_generated-16], and store in expanded_keys
    for (unsigned char a = 0; a < 4; a++) {
      expanded_keys[bytes_generated] = expanded_keys[bytes_generated - 16] ^ temp[a];
      bytes_generated++;
    }
  }
}



void sub_bytes(unsigned char* state) {
  // Substitute each state value with another byte in the Rijndael S-Box
  for (int i = 0; i < 16; i++)
    state[i] = s_box[state[i]];
}



void inv_sub_bytes(unsigned char* state) {
  // Substitute each state value with another byte in the Rijndael S-Box
  for (int i = 0; i < 16; i++)
    state[i] = inv_s_box[state[i]];
}


void shift_rows(unsigned char* state) {
  unsigned char tmp[16];

  // First row don't shift (idx = idx)
  tmp[0] = state[0];
  tmp[4] = state[4];
  tmp[8] = state[8];
  tmp[12] = state[12];

  // Second row shift right once (idx = (idx + 4) % 16)
  tmp[1] = state[5];
  tmp[5] = state[9];
  tmp[9] = state[13];
  tmp[13] = state[1];

  // Third row shift right twice (idx = (idx +/- 8) % 16)
  tmp[2] = state[10];
  tmp[6] = state[14];
  tmp[10] = state[2];
  tmp[14] = state[6];

  // Fourth row shift right three times (idx = (idx - 4) % 16)
  tmp[3] = state[15];
  tmp[7] = state[3];
  tmp[11] = state[7];
  tmp[15] = state[11];

  for (int i = 0; i < 16; i++)
     state[i] = tmp[i];
}


void inv_shift_rows(unsigned char* state) {
  unsigned char tmp[16];

  // First row don't shift (idx = idx)
  tmp[0] = state[0];
  tmp[4] = state[4];
  tmp[8] = state[8];
  tmp[12] = state[12];

  // Second row shift right once (idx = (idx - 4) % 16)
  tmp[1] = state[13];
  tmp[5] = state[1];
  tmp[9] = state[5];
  tmp[13] = state[9];

  // Third row shift right twice (idx = (idx +/- 8) % 16)
  tmp[2] = state[10];
  tmp[6] = state[14];
  tmp[10] = state[2];
  tmp[14] = state[6];

  // Fourth row shift right three times (idx = (idx + 4) % 16)
  tmp[3] = state[7];
  tmp[7] = state[11];
  tmp[11] = state[15];
  tmp[15] = state[3];

  for (int i = 0; i < 16; i++)
     state[i] = tmp[i];
}


void mix_columns(unsigned char* state) {
  // Dot product and byte mod of state

  unsigned char tmp[16];
  // Column 1 entries
  tmp[0] = (unsigned char) (mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3]);
  tmp[1] = (unsigned char) (state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3]);
  tmp[2] = (unsigned char) (state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]]);
  tmp[3] = (unsigned char) (mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]]);
 
  // Column 2 entries
  tmp[4] = (unsigned char) (mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7]);
  tmp[5] = (unsigned char) (state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7]);
  tmp[6] = (unsigned char) (state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]]);
  tmp[7] = (unsigned char) (mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]]);
 
  // Column 3 entries
  tmp[8] = (unsigned char) (mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11]);
  tmp[9] = (unsigned char) (state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11]);
  tmp[10] = (unsigned char) (state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]]);
  tmp[11] = (unsigned char) (mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]]);
 
  // Column 4 entries
  tmp[12] = (unsigned char) (mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15]);
  tmp[13] = (unsigned char) (state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15]);
  tmp[14] = (unsigned char) (state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]]);
  tmp[15] = (unsigned char) (mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]]);

  for (int i = 0; i < 16; i++)
     state[i] = tmp[i];
}


void inv_mix_columns(unsigned char* state) {
  unsigned char tmp[16];

  // Column 1
  tmp[0] = (unsigned char) (mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]]);
  tmp[1] = (unsigned char) (mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]]);
  tmp[2] = (unsigned char) (mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]]);
  tmp[3] = (unsigned char) (mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]]);
 
  // Column 2
  tmp[4] = (unsigned char) (mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]]);
  tmp[5] = (unsigned char) (mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]]);
  tmp[6] = (unsigned char) (mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]]);
  tmp[7] = (unsigned char) (mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]]);
 
  // Column 3
  tmp[8] = (unsigned char) (mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]]);
  tmp[9] = (unsigned char) (mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]]);
  tmp[10] = (unsigned char) (mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]]);
  tmp[11] = (unsigned char) (mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]]);
 
  // Column 4
  tmp[12] = (unsigned char) (mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]]);
  tmp[13] = (unsigned char) (mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]]);
  tmp[14] = (unsigned char) (mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]]);
  tmp[15] = (unsigned char) (mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]]);

  for (int i = 0; i < 16; i++)
     state[i] = tmp[i];
}


void add_round_key(unsigned char* state, unsigned char* round_key) {
  for (int i = 0; i < 16; i++)
    state[i] ^= round_key[i];
}


char * aes_encrypt(unsigned char* message, unsigned char* expanded_key) {
  unsigned char state[16];

  // Take only the first 16 characters of the message
  for (int i = 0; i < 16; i++)
     state[i] = message[i];

  const unsigned int round_cnt = 9;
  add_round_key(state, expanded_key);

  for (int i = 0; i < round_cnt; i++) {
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state);
    add_round_key(state, expanded_key + (16 * (i + 1)));
  }

  // Final round
  sub_bytes(state);
  shift_rows(state);
  add_round_key(state, expanded_key + 160);

  char * enc_msg  = (char *) malloc(16);
  memcpy(enc_msg, state, 16);
  return enc_msg;
}


char * aes_decrypt(unsigned char* message, unsigned char* expanded_key) {
  unsigned char state[16];

  // Take only the first 16 characters of the message
  for (int i = 0; i < 16; i++)
     state[i] = message[i];

  const int round_cnt = 9;
  add_round_key(state, expanded_key + 160);

  for (int i = round_cnt; i > 0; i--) {
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, expanded_key + (16 * i));
    inv_mix_columns(state);
  }
  inv_shift_rows(state);
  inv_sub_bytes(state);
  add_round_key(state, expanded_key);

  char * dec_msg = (char *) malloc(16);
  memcpy(dec_msg, state, 16);
  return dec_msg;
}


char * right_pad_str(char * str, unsigned int pad_len) {
  const unsigned int str_len = strlen(str);
  unsigned int padded_str_len = str_len;
  if (padded_str_len % pad_len != 0)
    padded_str_len = (padded_str_len / pad_len + 1) * pad_len;

  unsigned char * padded_str = malloc(padded_str_len);
  for (int i = 0; i < padded_str_len; i++) {
    if (i >= str_len) padded_str[i] = 0;
    else padded_str[i] = str[i];
  }
  return padded_str;
}


void print_hex(char * msg) {
  for (int i = 0; i < 16; i++)
    printf("%02X ", (unsigned char) *(msg+i));
  putchar('\n');
}

int main(int argc, char * argv[]) {

  // Default values
  char *message = "This is a message we will encrypt with AES!";

  char * padded_msg = right_pad_str(message, 16);
  int padded_msg_len = strlen(message);
  if (padded_msg_len % 16)
    padded_msg_len = (padded_msg_len / 16 + 1) * 16;

  // expand key to 176 bytes
  char aes_key[16] = {
    1,  2,  3,  4,
    5,  6,  7,  8,
    9, 10, 11, 12,
    13, 14, 15, 16
    };
  unsigned char expanded_key[176];
  key_expansion(aes_key, expanded_key);

  // Iterate padded message in blocks of 16 bytes and encrypt
  char * enc_msg;
  for (int i = 0; i < padded_msg_len; i+=16) {
    // encrypt message
    enc_msg = aes_encrypt(padded_msg+i, expanded_key);
    print_hex(enc_msg);
    free(enc_msg);
  }

  free(padded_msg);

  return 0;
}