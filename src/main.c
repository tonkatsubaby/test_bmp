#include "write_functions.c"
#include "draw_functions.c"

typedef unsigned char uint8_t;

// http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

int main(void)
{
   const int WIDTH  = 1500;
   const int HEIGHT = 1500;

   int length = WIDTH * HEIGHT;
   uint8_t rgb_vals[length * 3];

   for(int i = 0; i < length; i++)
   {
      rgb_vals[i * 3] = 255;
      rgb_vals[(i * 3 + 1)] = 255;
      rgb_vals[(i * 3 + 2)] = 255;
   }

   // line(0, 0, 400, 400, rgb_vals, WIDTH);
   // draw_vec2(1, 1, 120, 100, rgb_vals, WIDTH);
   generate_tree(750, 60, 10, rgb_vals, WIDTH, HEIGHT);

   generate_bmp(WIDTH, HEIGHT, rgb_vals);

   return 0;
}