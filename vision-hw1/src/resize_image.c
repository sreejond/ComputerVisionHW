#include <math.h>
#include <stdio.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
	x = (round(x) >= im.w) ? im.w - 1 : round(x);
	y = (round(y) >= im.h) ? im.h - 1 : round(y);
    int index = c * im.w * im.h + y * im.w + x;
    return im.data[index];
}

image nn_resize(image im, int w, int h)
{
    image resizeImage = make_image(w, h, im.c);
    float widthFraction = (float) im.w / w;
    float heightFraction = (float) im.h / h;
    int row, col, chanel;
    for (chanel = 0; chanel < im.c; chanel++)
    {
    	for (row = 0; row < h; row++)
    	{
    		for (col = 0; col < w; col++)
    		{
    			int index = chanel * w * h + row * w + col;
    			resizeImage.data[index] = nn_interpolate(im, col * widthFraction, row * heightFraction, chanel);
    		}
    	}
    }
    
    return resizeImage;
}

static float findDist(float a, float b)
{
    return a - b;
}

static float findPixel(image im, float x, float y, int c)
{
    int index = c * im.w * im.h + y * im.w + x;
    return im.data[index];
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float d1 = findDist(x, floor(x));
    float d2 = findDist(floor(x + 1.0), x);
    float d3 = findDist(y, floor(y));
    float d4 = findDist(floor(y + 1.0), y);

    float v1 = findPixel(im, floor(x), floor(y), c);
    float v2 = findPixel(im, ceil(x), floor(y), c);
    float v3 = findPixel(im, floor(x), ceil(y), c);
    float v4 = findPixel(im, ceil(x), ceil(y), c);

    float q1 = v1*d2 + v2*d1;
    float q2 = v3*d2 + v4*d1;
    float q = q1*d4 + q2*d3;
    
    return q;
}

image bilinear_resize(image im, int w, int h)
{
    image resizeImage = make_image(w, h, im.c);
    float widthFraction = (float) im.w / w;
    float heightFraction = (float) im.h / h;
    int row, col, chanel;
    for (chanel = 0; chanel < im.c; chanel++)
    {
        for (row = 0; row < h; row++)
        {
            for (col = 0; col < w; col++)
            {
                int index = chanel * w * h + row * w + col;
                resizeImage.data[index] = bilinear_interpolate(im, col * widthFraction, row * heightFraction, chanel);
            }
        }
    }
    
    return resizeImage;
}

