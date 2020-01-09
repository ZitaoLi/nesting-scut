import os
from PIL import Image
import math
import matplotlib.pyplot as plt

DIR_NAMES = [
    './b_n7_f10_none_backtracking_lrf_aeap_0_01/results/',
    './o_n7_f10_none_backtracking_lrf_aeap_0_01/results/',
    './b_n7_f10_none_dijkstra_lrf_aeap_0_01/results/',
]
ROWS = 2

imgs = []
for dir_name in DIR_NAMES:
    filenames = os.listdir(dir_name)
    for filename in filenames:
        if os.path.splitext(filename)[1] == '.png':
            img = Image.open(os.path.join(dir_name, filename))
            imgs.append(img)
W, H = imgs[0].size
COLS = math.ceil(len(imgs) / ROWS)
result = Image.new(imgs[0].mode, (W * ROWS, H * COLS))
if len(imgs) % ROWS:
    for k in range(COLS * ROWS - len(imgs)):
        appended_img = Image.new('RGB', (W, H), (255, 255, 255))
        imgs.append(appended_img)
for i in range(COLS):
    for j in range(ROWS):
        left = j * W
        top = i * H
        right = W + j * W
        bottom = H + i * H
        result.paste(imgs[i * ROWS + j], box=(left, top, right, bottom))
result.thumbnail((W * 0.8, H * 0.8))
result.show()
