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
# perf_table = PerfTable.Appendix_Table1
# perf_table = PerfTable.Table2
# perf_table = PerfTable.Appendix_Table2

# our_microbench_results = pd.read_csv("FP-op-new-new.csv", delimiter=',')
our_microbench_results = pd.read_csv("FP-op-plus.csv", delimiter=',')
our_microbench_results = our_microbench_results[our_microbench_results['Using GC?'] == 1]
our_geodesic_results = pd.read_csv("FP-geodesic.csv", delimiter=',')
EMP_microbench_results = pd.read_csv("EMP-float-bench.csv", delimiter=',')
EMP_geodesic_results = pd.read_csv("EMP-geodesic-bench.csv", delimiter=',')
ABY_microbench_results = pd.read_csv("ABYFloat-basic-op.csv", delimiter=',')
ABY_geodesic_results = pd.read_csv("ABYFloat-geodesic.csv", delimiter=',')
MP_SPDZ_results = pd.read_csv("MP-SPDZ-ring.csv", delimiter=',')

our_results = our_microbench_results
EMP_results = EMP_microbench_results
ABY_results = ABY_microbench_results

if perf_table == PerfTable.Table2 or perf_table == PerfTable.Appendix_Table2:
    our_results = our_geodesic_results
    EMP_results = EMP_geodesic_results
    ABY_results = ABY_geodesic_results
    MP_SPDZ_results = MP_SPDZ_results[0:0]

MP_SPDZ_results['Time (ms)'] = MP_SPDZ_results['Time (s)']*1000.0
MP_SPDZ_results['Communication (Bytes)'] = MP_SPDZ_results['Communication (MB)']*(10.0**6)
MP_SPDZ_results = MP_SPDZ_results.drop(['Time (s)', 'Communication (MB)'], axis=1)
ABY_GC_results = ABY_results[ABY_results['Sharing'] == 'YAO']
del ABY_GC_results['Sharing']
ABY_GMW_results = ABY_results[ABY_results['Sharing'] == 'BOOL']
del ABY_GMW_results['Sharing']

if perf_table == PerfTable.Table1 or perf_table == PerfTable.Appendix_Table1:
    our_results = our_results.drop(['Using GC?', '#Threads', 'Total Rounds'], axis=1)
else:
    our_results = our_results.drop(['#Threads', 'Total Rounds'], axis=1)
EMP_results = EMP_results.drop(['Total ANDs'], axis=1)
ABY_GC_results = ABY_GC_results.drop(['#Threads', 'Total ANDs', 'Total Rounds'], axis=1)
ABY_GMW_results = ABY_GMW_results.drop(['#Threads', 'Total ANDs', 'Total Rounds'], axis=1)

data_frames = [our_results, EMP_results, MP_SPDZ_results, ABY_GC_results, ABY_GMW_results]
frames_names = ['Our', 'EMP', 'MP-SPDZ', 'ABY-GC', 'ABY-GMW']
for i in range(len(data_frames)):
    data_frames[i][frames_names[i] + ' Time (s)'] = (data_frames[i]['Time (ms)']/1000.0)
    del data_frames[i]['Time (ms)']
    data_frames[i][frames_names[i] + ' Comm. (KiB)'] = (data_frames[i]['Communication (Bytes)']/(1024.0))
    del data_frames[i]['Communication (Bytes)']
merged_results = reduce(lambda  left,right: pd.merge(left,right,on=['Operator','#Operations'], how='outer'), data_frames).fillna(math.inf)
# print(merged_results)

if perf_table == PerfTable.Table1 or perf_table == PerfTable.Table2:
    merged_results['Improvement Time EMP'] = merged_results['EMP Time (s)']/merged_results['Our Time (s)']
    merged_results['Improvement Time MP-SPDZ'] = merged_results['MP-SPDZ Time (s)']/merged_results['Our Time (s)']
    merged_results['Improvement Comm. EMP'] = merged_results['EMP Comm. (KiB)']/merged_results['Our Comm. (KiB)']
    merged_results['Improvement Comm. MP-SPDZ'] = merged_results['MP-SPDZ Comm. (KiB)']/merged_results['Our Comm. (KiB)']
else:
    merged_results['Improvement Time ABY-GC'] = merged_results['ABY-GC Time (s)']/merged_results['Our Time (s)']
    merged_results['Improvement Time ABY-GMW'] = merged_results['ABY-GMW Time (s)']/merged_results['Our Time (s)']
    merged_results['Improvement Comm. ABY-GC'] = merged_results['ABY-GC Comm. (KiB)']/merged_results['Our Comm. (KiB)']
    merged_results['Improvement Comm. ABY-GMW'] = merged_results['ABY-GMW Comm. (KiB)']/merged_results['Our Comm. (KiB)']
print(merged_results)

# batch_sizes = [1, 10, 100, 1000, 10000, 100000]
# batch_sizes = [100, 1000, 10000, 100000]
batch_sizes = [1000, 10000, 100000]
operations = []
if perf_table == PerfTable.Table1 or perf_table == PerfTable.Appendix_Table1:
    operations = ['CMP', 'MUL', 'ADD', 'DIV', 'SQRT', 'SIN', 'COS', 'TAN', 'EXP2', 'LOG2', 'EXP', 'LN', 'ERF']
else:
    operations = [0, 1]
times_column = {}
comm_column = []
for num_ops in batch_sizes:
    times_column[num_ops] = []
for op in operations:
    for num_ops in batch_sizes:
        row = merged_results[merged_results['Operator'] == op][merged_results['#Operations'] == num_ops]
        if perf_table == PerfTable.Table1 or perf_table == PerfTable.Table2:
            times_column[num_ops].append(row.iloc[0]['Our Time (s)'])
            times_column[num_ops].append(row.iloc[0]['EMP Time (s)'])
            times_column[num_ops].append(row.iloc[0]['Improvement Time EMP'])
            # For MP-SPDZ
            times_column[num_ops].append(row.iloc[0]['MP-SPDZ Time (s)'])
            times_column[num_ops].append(row.iloc[0]['Improvement Time MP-SPDZ'])
            if num_ops == 10000:
                comm_column.append(row.iloc[0]['Our Comm. (KiB)']/num_ops)
                comm_column.append(row.iloc[0]['EMP Comm. (KiB)']/num_ops)
                comm_column.append(row.iloc[0]['Improvement Comm. EMP'])
                # For MP-SPDZ
                comm_column.append(row.iloc[0]['MP-SPDZ Comm. (KiB)']/num_ops)
                comm_column.append(row.iloc[0]['Improvement Comm. MP-SPDZ'])
        else:
            times_column[num_ops].append(row.iloc[0]['ABY-GC Time (s)'])
            times_column[num_ops].append(row.iloc[0]['Improvement Time ABY-GC'])
            times_column[num_ops].append(row.iloc[0]['ABY-GMW Time (s)'])
            times_column[num_ops].append(row.iloc[0]['Improvement Time ABY-GMW'])
            if num_ops == 10000:
                comm_column.append(row.iloc[0]['ABY-GC Comm. (KiB)']/num_ops)
                comm_column.append(row.iloc[0]['Improvement Comm. ABY-GC'])
                comm_column.append(row.iloc[0]['ABY-GMW Comm. (KiB)']/num_ops)
                comm_column.append(row.iloc[0]['Improvement Comm. ABY-GMW'])

df = pd.DataFrame(columns = batch_sizes)
for num_ops in batch_sizes:
    df[num_ops] = times_column[num_ops]
df['Comm. (KiB)/op'] = comm_column
# print(df)
if perf_table == PerfTable.Table1 or perf_table == PerfTable.Table2:
    for i in range(0, len(df), 5):
        for j in range(5):
            if df.iloc[i+j][batch_sizes[0]] == math.inf:
                continue
            for k in df.columns:
                if df.iloc[i+j][k] == math.inf:
                    print('&', '$' + '-' + '$', end=' ')
                elif j == 2 or j == 4:
                    print('&', '$(' + ('%.1f' % df.iloc[i+j][k]) + '\\text{x})$', end=' ')
                else:
                    print('&', '$' + ('%.2f' % df.iloc[i+j][k]) + '$', end=' ')
            print('')
else:
    for i in range(0, len(df), 4):
        for j in range(4):
            if df.iloc[i+j][batch_sizes[0]] == math.inf:
                continue
            for k in df.columns:
                if df.iloc[i+j][k] == math.inf:
                    print('&', '$' + '-' + '$', end=' ')
                elif j == 1 or j == 3:
                    print('&', '$(' + ('%.1f' % df.iloc[i+j][k]) + '\\text{x})$', end=' ')
                else:
                    print('&', '$' + ('%.2f' % df.iloc[i+j][k]) + '$', end=' ')
            print('')
