#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

static void clamp(int *a, int min, int max) 
{
    *a = *a < min ? min : *a;
    *a = *a > max ? max : *a;
}

float get_pixel(image im, int x, int y, int c)
{
    clamp(&x, 0, im.w - 1);
    clamp(&y, 0, im.h - 1);
    clamp(&c, 0, im.c - 1);

    int index = c * im.w * im.h + y * im.w + x;
    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x >= im.w || x < 0 || y >= im.h || y < 0 || c >= im.c || c < 0)   return;

    int index = c * im.w * im.h + y * im.w + x;
    im.data[index] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

static float getWeightedIntensity(int chanel, float intensity)
{
    switch (chanel)
    {
        case 0:
            return 0.299 * intensity;
        case 1:
            return 0.587 * intensity;
        case 2:
            return 0.114 * intensity;
        default:
            return 0.0;
    }
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    int row, col, chanel;
    for (row = 0; row < im.h; row++)
    {
        for (col = 0; col < im.w; col++)
        {
            float newIntensity = 0.0;
            for (chanel = 0; chanel < im.c; chanel++)
            {
                int index = chanel * im.w * im.h + row * im.w + col;
                newIntensity += getWeightedIntensity(chanel, im.data[index]);
            }
            gray.data[row * im.w + col] = newIntensity;
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    int row, col;
    for (row = 0; row < im.h; row++)
    {
        for (col = 0; col < im.w; col++)
        {
            int index = c * im.h * im.w + row * im.w + col;
            im.data[index] += v;
        }
    }
}

void clamp_image(image im)
{
    int chanel, row, col;
    for (chanel = 0; chanel < im.c; chanel++)
    {
        for (row = 0; row < im.h; row++)
        {
            for (col = 0; col < im.w; col++)
            {
                int index = chanel * im.h * im.w + row * im.w + col;
                if (im.data[index] < 0.0) im.data[index] = 0.0;
                else if (im.data[index] > 1.0)  im.data[index] = 1.0;
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

float calculateHue(float V, float* color, int chanel)
{
    /*assert(chanel != 0);

    float huePrime;
    switch (V)
    {
        case color[0]:
            huePrime = (color[1] - color[2]) / chanel;
            break;
        case color[1]:
            huePrime = (color[2] - color[0]) / chanel + 2;
            break;
        case color[2]:
            huePrime = (color[0] - color[1]) / chanel + 4;
            break;
        default:
            huePrime = 0.0;
    }

    float H = huePrime / 6;
    if (huePrime < 0)   H += 1;
    return H;*/
    return 0;
}

void rgb_to_hsv(image im)
{
    int chanel, row, col;
    
    for (row = 0; row < im.h; row++)
    {
        for (col = 0; col < im.w; col++)
        {
            float rgb[im.c];
            for (chanel = 0; chanel < im.c; chanel++)
            {
                int index = chanel * im.h * im.w + row * im.w + col;
                rgb[chanel] = im.data[index];
            }

            float hsv[im.c];
            hsv[2] = three_way_max(rgb[0], rgb[1], rgb[2]);
            float c = hsv[2] - three_way_min(rgb[0], rgb[1], rgb[2]);
            hsv[1] = c / hsv[2];
            hsv[0] = calculateHue(hsv[2], rgb, im.c);

            for (chanel = 0; chanel < im.c; chanel++)
            {
                int index = chanel * im.h * im.w + row * im.w + col;
                im.data[index] = hsv[chanel];
            }
        }
    }
}

void hsv_to_rgb(image im)
{
    
}
