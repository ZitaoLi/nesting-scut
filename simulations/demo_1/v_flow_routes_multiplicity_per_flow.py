import matplotlib.pyplot as plt
import numpy as np
import json
import os


SOLUTION_NAMES = {
    'om-aeap': './b_n7_f10_none_backtracking_lrf_aeap_0_02/b_n7_f10_none_backtracking_lrf_aeap',
    'op': './o_n7_f10_none_backtracking_lrf_aeap_0_02/o_n7_f10_none_backtracking_lrf_aeap',
}
BAR_WIDTH = 0.2
FIG_SIZE = (10, 5)


def v_repetition_degree_per_flow(filename):
    with open(filename, 'r') as file:
        res = json.load(file)
        fig, ax = plt.subplots(figsize=(10, 5))
        repetition_degree_per_flow_list = []
        for (flow_id, pairs) in res.items():
            dest_id_list = []
            no_of_repetition_edges_list = []
            for pair in pairs:
                dest_id_list.append(pair[1])
                no_of_repetition_edges_list.append(pair[4])  # no_of_repetition_edges / no_of_e2e_routes
            no_of_dest = len(dest_id_list)
            repetition_degree_per_flow = np.sum(no_of_repetition_edges_list) / no_of_dest
            repetition_degree_per_flow_list.append(repetition_degree_per_flow)
        labels = ['{}'.format(flow_id) for flow_id in res.keys()]
        ax.bar(np.arange(len(res)), repetition_degree_per_flow_list, width=BAR_WIDTH, tick_label=labels,
               label='repetition degree')
        ax.legend()
        ax.set_title('{}'.format(filename))
        ax.set_xlabel('Listeners')
        ax.set_ylabel('Repetition degree')
        plt.show()


fig, ax = plt.subplots(figsize=FIG_SIZE)
ax.set_title('Multiplicity of Flows')
ax.set_xlabel('flow id')
ax.set_ylabel('multiplicity')
i = 0
for alias, solution_name in SOLUTION_NAMES.items():
    with open(solution_name + '.json', 'r') as file:
        res = json.load(file)
        repetition_degree_per_flow_list = []
        for (flow_id, pairs) in res.items():
            dest_id_list = []
            no_of_repetition_edges_list = []
            for pair in pairs:
                dest_id_list.append(pair[1])
                no_of_repetition_edges_list.append(pair[4])  # no_of_repetition_edges / no_of_e2e_routes
            no_of_dest = len(dest_id_list)
            repetition_degree_per_flow = np.sum(no_of_repetition_edges_list) / no_of_dest
            repetition_degree_per_flow_list.append(repetition_degree_per_flow)
        labels = ['{}'.format(flow_id) for flow_id in res.keys()]
        ax.bar(np.arange(len(res))+BAR_WIDTH*i, repetition_degree_per_flow_list, width=BAR_WIDTH,
               tick_label=labels, label='{}'.format(alias))
        ax.legend()
        i += 1
plt.show()
