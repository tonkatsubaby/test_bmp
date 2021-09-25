#ifndef DRAW_FUNCTIONS_H_INCLUDED
#define DRAW_FUNCTIONS_H_INCLUDED

typedef unsigned char uint8_t;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float radians(float angle)
{
   return angle * (M_PI / 180);
}

void line(int x1, int y1, int x2, int y2, uint8_t *arr, int width)
{
   printf("A(%03d, %03d) -> B(%03d, %03d)\n",
      x1, y1, x2, y2
   );

   float points_num;
   int points_num_x = x2 - x1;
   int points_num_y = y2 - y1;

   if(abs(points_num_x) > abs(points_num_y))
   {
      points_num = abs(points_num_x);
   }
   else
   {
      points_num = abs(points_num_y);
   }

   if(points_num == 0)
   {
      return;
   }

   for(int i = 0; i <= points_num; i++)
   {
      float t = i / points_num;

      int x = round(x1 + (t * points_num_x));
      int y = round(y1 + (t * points_num_y));

      int index = (x + (y * width)) * 3;

      arr[index]     = 0;
      arr[index + 1] = 0;
      arr[index + 2] = 0;
   }
}

void draw_vec2(int x, int y, int angle, float dist, uint8_t *arr, int width)
{
   int end_x = x + (fabs(cos(radians(angle))) * dist);
   int end_y = y + (fabs(sin(radians(angle))) * dist);

   line(x, y, end_x, end_y, arr, width);
}

void get_vec2(int x, int y, int angle, float dist, int *ptr_x, int *ptr_y)
{
   *ptr_x = x + ((cos(radians(angle - 90))) * dist);
   *ptr_y = y + ((sin(radians(angle - 90))) * dist);
}

void generate_tree(float length, int angle, int iterations, uint8_t *arr, int width, int height)
{
   int start_x = width / 2;
   int start_y = height - 1;

   int point_arr_len = pow(2, iterations);

   int points_x[point_arr_len];
   int points_y[point_arr_len];
   int points_a[point_arr_len];

   points_x[0] = start_x;
   points_y[0] = start_y;
   points_a[0] = 0;

   int layer_branches = pow(2, 0);

   printf("%d", point_arr_len);

   printf("\n\n\n\n--------------------------------------------\n\n\n\n");

   for(int layer = 0; layer < iterations; layer++)
   {
      int write_to = 0;
      int read_from = layer == 0 ? 0 : layer_branches;
      layer_branches = pow(2, layer);

      for(int i = 0; i < layer_branches; i++)
      {
         write_to = layer_branches + i;
         float dist = pow(2, layer);
         if(dist == 0)
         {
            dist = 1;
         }
         else
         {
            dist = length / dist;
         }

         int repeater = (int)floor(read_from + (float)i / 2);

         if(i % 2 == 0)
         {
            points_a[write_to] = (points_a[repeater] - angle);

            get_vec2(
               points_x[repeater],
               points_y[repeater],
               points_a[repeater],
               dist,
               &points_x[write_to],
               &points_y[write_to]
            );

            line(
               points_x[repeater],
               points_y[repeater],
               points_x[write_to],
               points_y[write_to],
               arr,
               width
            );
         }
         else
         {
            points_a[write_to] = (points_a[repeater] + angle);

            get_vec2(
               points_x[repeater],
               points_y[repeater],
               points_a[repeater] + angle*2,
               dist,
               &points_x[write_to],
               &points_y[write_to]
            );

            line(
               points_x[repeater],
               points_y[repeater],
               points_x[write_to],
               points_y[write_to],
               arr,
               width
            );
         }
      }
   }
}

#endif