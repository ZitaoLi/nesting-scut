import os
from PIL import Image

DIR_NAME = './b_n4_f10_none_backtracking_lrf_aeap/results/'

filenames = os.listdir(DIR_NAME)
imgs = []
for filename in filenames:
    if os.path.splitext(filename)[1] == '.png':
        img = Image.open(os.path.join(DIR_NAME, filename))
        imgs.append(img)
width, height = imgs[0].size
result = Image.new(imgs[0].mode, (width * 2, height * 2))

img1 = Image.new('RGB', (int(width / 2), height), (255, 255, 255))
img2 = Image.new('RGB', (int(width / 2), height), (255, 255, 255))
imgs.insert(2, img1)
imgs.insert(4, img2)

result.paste(imgs[0], (0, 0, width, height))
result.paste(imgs[1], (width, 0, width + width, height))
result.paste(imgs[2], (0, height, int(width / 2), height * 2))
result.paste(imgs[3], (int(width / 2), height, width + int(width / 2), height * 2))
result.paste(imgs[4], (int(width + width / 2), height, width * 2, height * 2))

result.show()
