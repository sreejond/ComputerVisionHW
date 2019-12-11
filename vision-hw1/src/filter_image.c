#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    int chanel, row, col;
    for (chanel = 0; chanel < im.c; chanel++)
    {
        float sumOfPixels = 0.0;
        for (col = 0; col < im.w; col++)
        {
            for (row = 0; row < im.h; row++)
            {
                sumOfPixels += get_pixel(im, col, row, chanel);
            }
        }

        for (col = 0; col < im.w; col++)
        {
            for (row = 0; row < im.h; row++)
            {
                float pixel = get_pixel(im, col, row, chanel);
                set_pixel(im, col, row, chanel, pixel / sumOfPixels);
            }
        }
    }
}

image make_box_filter(int w)
{
    image boxFilter = make_image(w, w, 1);
    int row, col;
    for (row = 0; row < w; row++)
    {
        for (col = 0; col < w;  col++)
        {
            int index = row * w + col;
            boxFilter.data[index] = 1.0;
        }
    }

    l1_normalize(boxFilter);

    return boxFilter;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(im.c == filter.c || filter.c == 1);

    int numOfChanel = (preserve == 1) ? im.c : 1;
    image convolve = make_image(im.w, im.h, numOfChanel);

    int halfWidthOfFilter = filter.w / 2;
    int halfHeightOfFilter = filter.h / 2;

    for (int row = 0; row < im.h; row++)
    {
        for (int col = 0; col < im.w; col++)
        {
            float sum = 0.0;
            for (int chanel = 0; chanel < im.c; chanel++)
            {
                if (numOfChanel > 1)    sum = 0.0;

                for (int filter_r = 0; filter_r < filter.h; filter_r++)
                {
                    for (int filter_c = 0; filter_c < filter.w; filter_c++)
                    {
                        int im_r = row + filter_r - halfHeightOfFilter;
                        int im_c = col + filter_c - halfWidthOfFilter;
                        float im_pixel = get_pixel(im, im_c, im_r, chanel);
                        float filter_pixel;
                        if (filter.c > 1)
                            filter_pixel = get_pixel(filter, filter_c, filter_r, chanel);
                        else filter_pixel = get_pixel(filter, filter_c, filter_r, 0);
                        sum += im_pixel * filter_pixel;
                    }
                }

                if (numOfChanel > 1)
                {
                    sum = (sum > 1.0) ? 1.0 : sum;
                    sum = (sum < 0.0) ? 0.0 : sum;
                    set_pixel(convolve, col, row, chanel, sum);
                }
            }

            if (numOfChanel == 1)
            {
                sum = (sum > 1.0) ? 1.0 : sum;
                sum = (sum < 0.0) ? 0.0 : sum;
                set_pixel(convolve, col, row, 0, sum);
            }
        }
    }

    return convolve;
}

image make_highpass_filter()
{
    int numOfChanel = 1;
    image highpass = make_image(3, 3, numOfChanel);

    for (int chanel = 0; chanel < numOfChanel; chanel++)
    {
        set_pixel(highpass, 0, 0, chanel, 0.0);
        set_pixel(highpass, 1, 0, chanel, -1.0);
        set_pixel(highpass, 2, 0, chanel, 0.0);

        set_pixel(highpass, 0, 1, chanel, -1.0);
        set_pixel(highpass, 1, 1, chanel, 4.0);
        set_pixel(highpass, 2, 1, chanel, -1.0);

        set_pixel(highpass, 0, 2, chanel, 0.0);
        set_pixel(highpass, 1, 2, chanel, -1.0);
        set_pixel(highpass, 2, 2, chanel, 0.0);
    }

    return highpass;
}

image make_sharpen_filter()
{
    int numOfChanel = 1;
    image sharpen = make_image(3, 3, numOfChanel);

    for (int chanel = 0; chanel < numOfChanel; chanel++)
    {
        set_pixel(sharpen, 0, 0, chanel, 0.0);
        set_pixel(sharpen, 1, 0, chanel, -1.0);
        set_pixel(sharpen, 2, 0, chanel, 0.0);

        set_pixel(sharpen, 0, 1, chanel, -1.0);
        set_pixel(sharpen, 1, 1, chanel, 5.0);
        set_pixel(sharpen, 2, 1, chanel, -1.0);

        set_pixel(sharpen, 0, 2, chanel, 0.0);
        set_pixel(sharpen, 1, 2, chanel, -1.0);
        set_pixel(sharpen, 2, 2, chanel, 0.0);
    }

    return sharpen;
}

image make_emboss_filter()
{
    int numOfChanel = 1;
    image emboss = make_image(3, 3, numOfChanel);

    for (int chanel = 0; chanel < numOfChanel; chanel++)
    {
        set_pixel(emboss, 0, 0, chanel, -2.0);
        set_pixel(emboss, 1, 0, chanel, -1.0);
        set_pixel(emboss, 2, 0, chanel, 0.0);

        set_pixel(emboss, 0, 1, chanel, -1.0);
        set_pixel(emboss, 1, 1, chanel, 1.0);
        set_pixel(emboss, 2, 1, chanel, 1.0);

        set_pixel(emboss, 0, 2, chanel, 0.0);
        set_pixel(emboss, 1, 2, chanel, 1.0);
        set_pixel(emboss, 2, 2, chanel, 2.0);
    }

    return emboss;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

float gaussianDistribution(int x, int y, int center, float sigma)
{
    int sub_x = x - center;
    int sub_y = y - center;
    return 1.0 / (TWOPI * sigma * sigma) * exp(-1.0 * (sub_x * sub_x + sub_y * sub_y) / (2 * sigma * sigma));
}

image make_gaussian_filter(float sigma)
{
    int size = ceil(sigma * 6.0);
    size = (size % 2 == 0) ? size + 1 : size;
    int center = size / 2;
    image gaussianFilter = make_image(size, size, 1);

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            set_pixel(gaussianFilter, col, row, 0, gaussianDistribution(row, col, center, sigma));
        }
    }

    l1_normalize(gaussianFilter);

    return gaussianFilter;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image newImage = make_image(a.w, a.h, a.c);
    for (int chanel = 0; chanel < a.c; chanel++)
    {
        for (int row = 0; row < a.h; row++)
        {
            for (int col = 0; col < a.w; col++)
            {
                float p1 = get_pixel(a, col, row, chanel);
                float p2 = get_pixel(b, col, row, chanel);
                float p = p1 + p2;
                set_pixel(newImage, col, row, chanel, p);
            }
        }
    }

    return newImage;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image newImage = make_image(a.w, a.h, a.c);
    for (int chanel = 0; chanel < a.c; chanel++)
    {
        for (int row = 0; row < a.h; row++)
        {
            for (int col = 0; col < a.w; col++)
            {
                float p1 = get_pixel(a, col, row, chanel);
                float p2 = get_pixel(b, col, row, chanel);
                float p = p1 - p2;
                set_pixel(newImage, col, row, chanel, p);
            }
        }
    }

    return newImage;
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
