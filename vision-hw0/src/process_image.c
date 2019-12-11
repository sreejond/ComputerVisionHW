#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

static void clampPaddingRow(image im, int* y)
{
    if (*y >= im.h)  *y = im.h - 1;
    else if (*y < 0) *y = 0;
}

static void clampPaddingCol(image im, int* x)
{
    if (*x >= im.w)  *x = im.w - 1;
    else if (*x < 0) *x = 0;
}

static void clampPaddingChanel(image im, int* c)
{
    if (*c >= im.c)  *c = im.c - 1;
    else if (*c < 0) *c = 0;
}

float get_pixel(image im, int x, int y, int c)
{
    clampPaddingRow(im, &y);
    clampPaddingCol(im, &x);
    clampPaddingChanel(im, &c);

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

    int row, col, chanel;
    for (chanel = 0; chanel < im.c; chanel++)
    {
        for (row = 0; row < im.h; row++)
        {
            for (col = 0; col < im.w; col++)
            {
                set_pixel(copy, col, row, chanel, get_pixel(im, col, row, chanel));
            }
        }
    }
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
    assert(chanel != 0);

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
    return H;
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
            float c = V - three_way_min(rgb[0], rgb[1], rgb[2]);
            hsv[1] = c / V;
            hsv[0] = calculateHue(V, rgb, im.c);

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
