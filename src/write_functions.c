#ifndef WRITE_FUNCTIONS_H_INCLUDED
#define WRITE_FUNCTIONS_H_INCLUDED

#include <stdio.h>

typedef unsigned char uint8_t;

void print_ptr(uint8_t *ptr, int len)
{
   for(int i = 0; i < len; i++)
   {
      printf("%02x ", ptr[i]);
   }

   printf("\n");
}

void little_endian(uint8_t *ptr, int number)
{
   // array storing 8 bit unsigned chars
   // 32 bit number: 00000000 00000000 00000000 11000110

   // right shift 24: 00000000 00000000 00000000 00000000 | 00000000 00000000 11000110
   // right shift 16: 00000000 00000000 00000000 00000000 | 00000000 11000110
   // right shift 8:  00000000 00000000 00000000 00000000 | 11000110
   // right shift 0:  00000000 00000000 00000000 11000110 |
   // bitAND checks if it's within range

   // unsigned char takes the right-most bytes
   // example: int x = 0xFFEEDDCC;
   //          unsigned char byte = x;

   // byte will equal to 0xCC

   ptr[3] = (number >> 24) & 0xff;
   ptr[2] = (number >> 16) & 0xff;
   ptr[1] = (number >> 8)  & 0xff;
   ptr[0] = (number)       & 0xff;
}

void generate_bmp(int width, int height, uint8_t *pixel_array)
{
   FILE *bmp = fopen("./images/bmp_test.bmp", "wb");
   const int HEADER_SIZE = 54;

   uint8_t padding[3]    = {0, 0, 0};
   uint8_t padding_bytes = (4 - ((width * 3) % 4)) % 4;
   const int padded_size = HEADER_SIZE + ((width * 3) * height) + (padding_bytes * height);

   printf("%d\n\n", padding_bytes);

   uint8_t sig[2] = {0x42, 0x4d}; // 'BM'
   uint8_t file_size[4];          // size of file
   uint8_t unused[4];             // There's nothing for some reason
   uint8_t data_offset[4];        // useless
   uint8_t info_size[4];          // size of info header
   uint8_t le_w[4];               // width
   uint8_t le_h[4];               // height
   uint8_t pln[2] = {0x01, 0x00}; // always one
   uint8_t bpp[2] = {0x18, 0x00}; // bits per pixel
   uint8_t compression[4];        // because 24bpp
   uint8_t image_size[4];         // because compression
   uint8_t x_ppm[4];              // temporary value
   uint8_t y_ppm[4];              // temporary value
   uint8_t colors_used[4];        // 24bpp
   uint8_t important_colors[4];   // 24bpp

   //sig
   little_endian(file_size,        padded_size);
   little_endian(unused,           0          );                                
   little_endian(data_offset,      HEADER_SIZE);
   little_endian(info_size,        40         );
   little_endian(le_w,             width      );
   little_endian(le_h,             height     );
   //pln
   //bpp
   little_endian(compression,      0);
   little_endian(image_size,       0);
   little_endian(x_ppm,            1);
   little_endian(y_ppm,            1);
   little_endian(colors_used,      0);
   little_endian(important_colors, 0);

   print_ptr(sig,              2);
   print_ptr(file_size,        4);
   print_ptr(unused,           4);
   print_ptr(data_offset,      4);
   print_ptr(info_size,        4);
   print_ptr(le_w,             4);
   print_ptr(le_h,             4);
   print_ptr(pln,              2);
   print_ptr(bpp,              2);
   print_ptr(image_size,       4);
   print_ptr(x_ppm,            4);
   print_ptr(y_ppm,            4);
   print_ptr(colors_used,      4);
   print_ptr(important_colors, 4);

   fwrite(sig,              1, sizeof(sig), bmp);
   fwrite(file_size,        1, sizeof(int), bmp);
   fwrite(unused,           1, sizeof(int), bmp);
   fwrite(data_offset,      1, sizeof(int), bmp);
   fwrite(info_size,        1, sizeof(int), bmp);
   fwrite(le_w,             1, sizeof(int), bmp);
   fwrite(le_h,             1, sizeof(int), bmp);
   fwrite(pln,              1, sizeof(pln), bmp);
   fwrite(bpp,              1, sizeof(bpp), bmp);
   fwrite(compression,      1, sizeof(int), bmp);
   fwrite(image_size,       1, sizeof(int), bmp);
   fwrite(x_ppm,            1, sizeof(int), bmp);
   fwrite(y_ppm,            1, sizeof(int), bmp);
   fwrite(colors_used,      1, sizeof(int), bmp);
   fwrite(important_colors, 1, sizeof(int), bmp);

   uint8_t rgb_vals[padded_size];

   for(int y = height - 1; y >= 0; y--)
   {
      for(int x = 0; x < width; x++)
      {
         int index = (x + (y * width)) * 3;

         uint8_t rgb_arr[3] = {
            pixel_array[index + 2],
            pixel_array[index + 1],
            pixel_array[index]
         };

         fwrite(rgb_arr, 1, sizeof(rgb_arr), bmp);
      }
      fwrite(padding, 1, padding_bytes, bmp);
   }

   fclose(bmp);
}

#endif