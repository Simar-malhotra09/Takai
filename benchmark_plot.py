import matplotlib.pyplot as plt
import os
import json
import seaborn as sns

sns.set(style='whitegrid')

bm_output= './bm_output.json'
with open(bm_output, 'r') as f:
    benchmark_data= json.load(f)

benchmarks= benchmark_data['benchmarks']

input_sizes_gb=[]
times=[]

threads= benchmarks[0]['name'].split('/')[2]

for benchmark in benchmarks:
    input_sizes_gb.append(int(benchmark['name'].split('/')[1]) / (1024 ** 3))
    times.append(benchmark['real_time'])

    assert threads==benchmark['name'].split('/')[2]

plt.figure(figsize=(10, 6))
plt.plot(input_sizes_gb, times, marker='o', linewidth=2, color='dodgerblue', label=f'{threads} Threads Internal')

for x, y in zip(input_sizes_gb, times):
    plt.text(x, y + 2, f"{y:.1f}s", ha='center', fontsize=9)

plt.title('📊 File Processing Benchmark: Runtime vs Input Size', fontsize=16, weight='bold')
plt.xlabel('Input Size (GB)', fontsize=14)
plt.ylabel('Runtime (Seconds)', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.6)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.legend(fontsize=12, loc='upper left')
plt.tight_layout()
plt.savefig('benchmark.png')
plt.show()

