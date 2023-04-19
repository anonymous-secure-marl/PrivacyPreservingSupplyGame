import math
import sys
import numpy as np
import pandas as pd
from enum import Enum
from functools import reduce

class PerfTable(Enum):
    Table1 = 1
    Table2 = 2
    Appendix_Table1 = 3
    Appendix_Table2 = 4

perf_table = PerfTable.Table1

our_results = pd.read_csv("FP-op-plus.csv", delimiter=',')

our_results_SCI = our_results
our_results_GC = our_results

our_results_SCI = our_results_SCI[our_results_SCI['Using GC?'] == 0]
our_results_GC = our_results_GC[our_results_GC['Using GC?'] == 1]

our_results_SCI = our_results_SCI.drop(['Using GC?', '#Threads', 'Total Rounds'], axis=1)
our_results_GC = our_results_GC.drop(['Using GC?', '#Threads', 'Total Rounds'], axis=1)

data_frames = [our_results_SCI, our_results_GC]
for df in data_frames:
    df['Time (s)'] = df['Time (ms)']/1000.0
    df['Comm. (KiB)'] = df['Communication (Bytes)']/(1024.0)
    del df['Time (ms)']
    del df['Communication (Bytes)']

merged_results = reduce(lambda left, right: pd.merge(left,right,on=['Operator','#Operations'], how='outer'), data_frames).fillna(math.inf)
merged_results['Impr. Time'] = merged_results['Time (s)_x']/merged_results['Time (s)_y']
merged_results['Impr. Comm.'] = merged_results['Comm. (KiB)_x']/merged_results['Comm. (KiB)_y']
print(merged_results)
