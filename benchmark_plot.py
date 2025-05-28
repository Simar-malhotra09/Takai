import matplotlib.pyplot as plt
import os
import json
import seaborn as sns

sns.set(style='whitegrid')

bm_output= './bm_output.json'
with open(bm_output, 'r') as f:
    benchmark_data= json.load(f)

benchmarks= benchmark_data['benchmarks']

thread_sizes = {
    t: {'file_size': [], 'time': []}
    for t in {b['name'].split('/')[2] for b in benchmarks}
}

for b in benchmarks:
    t_size = b['name'].split('/')[2]
    size_gb = int(b['name'].split('/')[1]) / (1024 ** 3)
    time = b['real_time']

    thread_sizes[t_size]['file_size'].append(size_gb)
    thread_sizes[t_size]['time'].append(time)


colors = ['dodgerblue', 'green', 'orange', 'crimson', 'purple', 'teal']

plt.figure(figsize=(10, 6))
for i, (t, values) in enumerate(sorted(thread_sizes.items(), key=lambda x: int(x[0]))):
    color = colors[i % len(colors)]
    plt.plot(values['file_size'], values['time'], marker='o', linewidth=2, color=color, label=f'{t} Threads')
    
    for x, y in zip(values['file_size'], values['time']):
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
