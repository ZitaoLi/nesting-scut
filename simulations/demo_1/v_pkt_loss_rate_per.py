import matplotlib.pyplot as plt
import numpy as np
import os
import csv

PER_DIR_NAME_DICT = {
    'd-aeap': {
        '0.004': './b_n7_f10_none_dijkstra_lrf_aeap_0_004/results/',
        '0.02': './b_n7_f10_none_dijkstra_lrf_aeap_0_02/results/',
        '0.01': './b_n7_f10_none_dijkstra_lrf_aeap_0_01/results/',
    },
    'om-aeap': {
        '0.004': './b_n7_f10_none_backtracking_lrf_aeap_0_004/results/',
        '0.02': './b_n7_f10_none_backtracking_lrf_aeap_0_02/results/',
        '0.01': './b_n7_f10_none_backtracking_lrf_aeap_0_01/results/',
    },
    'op': {
        '0.004': './o_n7_f10_none_backtracking_lrf_aeap_0_004/results/',
        '0.02': './o_n7_f10_none_backtracking_lrf_aeap_0_02/results/',
        '0.01': './o_n7_f10_none_backtracking_lrf_aeap_0_01/results/',
    }
}
FIG_SIZE = (10, 5)
BAR_WIDTH = 0.2

fig, ax = plt.subplots(figsize=FIG_SIZE)
ax.set_ylabel('avg. pkt. loss rate')
x = ['0.004', '0.01', '0.02']
i = 0
for alias, dir_name_dict in PER_DIR_NAME_DICT.items():
    y = []
    for per, dir_name in dir_name_dict.items():
        with open(os.path.join(dir_name, 'flow-stat.csv')) as file:
            reader = csv.reader(file)
            all_no_of_sent_pkt = 0
            all_no_of_rcv_pkt = 0
            for line in reader:  # [fid, sent_pkt, rcv_pkt, no_of_flow]
                all_no_of_sent_pkt += int(line[1])
                all_no_of_rcv_pkt += int(line[2]) / int(line[3])
            pkt_loss_rate = (all_no_of_sent_pkt - all_no_of_rcv_pkt) / all_no_of_sent_pkt
            y.append(pkt_loss_rate)
    ax.bar(np.arange(len(x)) + BAR_WIDTH * i, y, width=BAR_WIDTH, tick_label=x, label='{}'.format(alias))
    for a, b in zip(np.arange(len(x)), y):
        ax.text(a + BAR_WIDTH * i, b, '{:.2%}'.format(b), ha='center', va='bottom', fontsize=7)
    ax.legend()
    i += 1
plt.show()
