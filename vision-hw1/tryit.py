from uwimg import *

im = load_image("data/dogsmall.jpg")
a = nn_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-nn")

im = load_image("data/dogsmall.jpg")
a = bilinear_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-bl")

im = load_image("data/dog.jpg")
a = nn_resize(im, im.w//7, im.h//7)
save_image(a, "dog7th-nn")

im = load_image("data/dog.jpg")
a = bilinear_resize(im, im.w//7, im.h//7)
save_image(a, "dog7th-bl")

im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")

im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
thumb = nn_resize(blur, blur.w//7, blur.h//7)
save_image(thumb, "dogthumb")

# 5. Highpass
im = load_image("data/dog.jpg")
f = make_highpass_filter()
highpass = convolve_image(im, f, 0)
save_image(highpass, "dog_highpass")

# 5. Sharpen
im = load_image("data/dog.jpg")
f = make_sharpen_filter()
sharpen = convolve_image(im, f, 1)
save_image(sharpen, "dog_sharpen")

# 5. Emboss
im = load_image("data/dog.jpg")
f = make_emboss_filter()
emboss = convolve_image(im, f, 1)
save_image(emboss, "dog_emboss")

# 6. gaussian
im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-gauss2")

# 7. hybrid 
# im = load_image("data/dog.jpg")
# f = make_gaussian_filter(2)
# lfreq = convolve_image(im, f, 1)
# hfreq = im - lfreq
# reconstruct = lfreq + hfreq
# save_image(lfreq, "low-frequency")
# save_image(hfreq, "high-frequency")
# save_image(reconstruct, "reconstruct")

# 8. Ronbledore
ron = load_image("data/ron.png")
dumbledore = load_image("data/dumbledore.png")
filter1 = make_gaussian_filter(3)
filter2 = make_gaussian_filter(5)
lfreqRon = convolve_image(dumbledore, filter1, 1)
lfreqDumbledore = convolve_image(ron, filter2, 1)
hfreqDumbledore = ron - lfreqDumbledore
ronbledore = lfreqRon + hfreqDumbledore;
save_image(lfreqRon, "low-freq-ron")
save_image(hfreqDumbledore, "high-freq-dum")
save_image(ronbledore, "ronbledore")


# 9. Sobel filter
from uwimg import *
im = load_image("data/dog.jpg")
res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
save_image(mag, "magnitude")