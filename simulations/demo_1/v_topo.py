import os
from PIL import Image
import matplotlib.pyplot as plt

DIR_NAME = './b_n7_f10_none_backtracking_lrf_aeap_0_02/results/'

filenames = os.listdir(DIR_NAME)
imgs = []
for filename in filenames:
    if os.path.splitext(filename)[1] == '.png':
        img = Image.open(os.path.join(DIR_NAME, filename))
        # img.show()
        imgs.append(img)
width, height = imgs[0].size
left = 0
right = width
result = Image.new(imgs[0].mode, (width * len(imgs), height))
for i, img in enumerate(imgs):
    result.paste(img, (left, 0, right, height))
    left += width
    right += width
result.show()
