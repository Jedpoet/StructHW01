import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict
import os

# --- Configuration ---
LOG_FILES = {
    "Uniform IDs (result.log)": "result.log",
    "Dense IDs (result1.log)": "result1.log"
}
STRUCTURE_MAP = {
    1: "d_array",
    2: "s_array",
    3: "llpp"
}
OPERATIONS = ["insert", "search", "sum"]
ALL_K_VALUES = list(range(11, 26))

# --- Data Parsing ---
def parse_and_average_logs(log_file_path):
    temp_data = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
    try:
        with open(log_file_path, 'r') as f:
            for line in f:
                try:
                    parts = [int(p) for p in line.strip().split()]
                    if len(parts) != 5: continue
                    struct_id, k, insert_ms, search_ms, sum_ms = parts
                    if struct_id not in STRUCTURE_MAP or k not in ALL_K_VALUES: continue
                    struct_name = STRUCTURE_MAP[struct_id]
                    times_ms = [insert_ms, search_ms, sum_ms]
                    for op_idx, op_name in enumerate(OPERATIONS):
                        time_s = times_ms[op_idx] / 1000.0
                        temp_data[struct_name][k][op_name].append(time_s)
                except (ValueError, IndexError):
                    continue
    except FileNotFoundError:
        print(f"Error: Log file not found at {log_file_path}")
        return None

    avg_data = defaultdict(lambda: defaultdict(list))
    for struct_name, k_data in temp_data.items():
        for k, op_data in k_data.items():
            n = 2**k
            for op_name, times in op_data.items():
                if times:
                    avg_time = sum(times) / len(times)
                    avg_data[struct_name][op_name].append((n, avg_time))
    
    for struct_name, op_data in avg_data.items():
        for op_name, values in op_data.items():
            values.sort()
            
    return avg_data

# --- Data Presentation ---
def print_data_table(title, data):
    print("\n" + "="*60)
    print(f"Measured Data for: {title}")
    print("="*60)
    for op in OPERATIONS:
        print(f"\n--- Operation: {op.capitalize()} ---")
        header = f"{'k':>3} {'n':>10}"
        for struct_id in sorted(STRUCTURE_MAP.keys()):
            header += f" {STRUCTURE_MAP[struct_id]:>12}"
        print(header)
        print("-"*len(header))
        for k in ALL_K_VALUES:
            n = 2**k
            row = f"{'k':>3} {n:>10,}"
            for struct_id in sorted(STRUCTURE_MAP.keys()):
                struct_name = STRUCTURE_MAP[struct_id]
                time_val = next((t for n_val, t in data.get(struct_name, {}).get(op, []) if n_val == n), None)
                row += f" {time_val:.4f}" if time_val is not None else " {:>12}".format("(missing)")
            print(row)

# --- Interactive Estimation ---
def get_user_estimations(measured_data):
    print("\n" + "="*60)
    print("Interactive Estimation Required")
    print("="*60)
    print("Please provide estimates for the missing (timeout) values.")

    estimated_data = defaultdict(lambda: defaultdict(list))

    for struct_name, op_data in measured_data.items():
        for op_name, values in op_data.items():
            max_k_measured = int(np.log2(values[-1][0])) if values else 0
            if max_k_measured < ALL_K_VALUES[-1]:
                # Present hint
                print(f"\n--- Hint for [{struct_name} - {op_name.capitalize()}] ---")
                if len(values) >= 2:
                    for i in range(max(0, len(values) - 3), len(values) - 1):
                        n1, t1 = values[i]
                        n2, t2 = values[i+1]
                        k1, k2 = int(np.log2(n1)), int(np.log2(n2))
                        if t1 > 0:
                            growth = t2 / t1
                            print(f"  - Growth from k={k1} to k={k2}: {growth:.2f}x (from {t1:.4f}s to {t2:.4f}s)")
                last_n, last_t = values[-1]
                last_k = int(np.log2(last_n))
                print(f"  - Last measured time at k={last_k} is {last_t:.4f}s.")

                # Loop to get user input for all missing k
                for k_to_estimate in range(last_k + 1, ALL_K_VALUES[-1] + 1):
                    while True:
                        prompt = f"-> Enter estimated time (s) for k={k_to_estimate}: "
                        try:
                            user_input = input(prompt)
                            est_time = float(user_input)
                            if est_time > 0:
                                estimated_data[struct_name][op_name].append((2**k_to_estimate, est_time))
                                break
                            else:
                                print("    Please enter a positive number.")
                        except ValueError:
                            print("    Invalid input. Please enter a number.")
    return estimated_data

# --- Plotting ---
def generate_plots(title, measured_data, estimated_data, output_dir="."):
    file_title_prefix = title.split(" ")[0].lower()
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for op in OPERATIONS:
        plt.style.use('seaborn-v0_8-whitegrid')
        fig, ax = plt.subplots(figsize=(10, 7))

        for struct_id in sorted(STRUCTURE_MAP.keys()):
            struct_name = STRUCTURE_MAP[struct_id]
            
            # Plot measured data
            m_values = measured_data.get(struct_name, {}).get(op, [])
            if m_values:
                n_m, t_m = zip(*m_values)
                ax.plot(n_m, t_m, marker='o', linestyle='-', label=f"{struct_name} (Measured)")
            
            # Plot estimated data
            e_values = estimated_data.get(struct_name, {}).get(op, [])
            if e_values:
                # Combine last measured point with first estimated point for a continuous line
                if m_values:
                    plot_e_n, plot_e_t = zip(*([m_values[-1]] + e_values))
                else:
                    plot_e_n, plot_e_t = zip(*e_values)
                ax.plot(plot_e_n, plot_e_t, marker='x', linestyle='--', label=f"{struct_name} (Estimated)")

        ax.set_xscale('log', base=2)
        ax.set_yscale('log')
        ax.set_title(f"{op.capitalize()} Time vs. n ({title})")
        ax.set_xlabel("n (Number of Records, 2^k)")
        ax.set_ylabel("Time (seconds, log scale)")
        ax.legend()
        ax.grid(True, which="both", ls="--")

        plot_filename = os.path.join(output_dir, f"{file_title_prefix}_{op}_performance.png")
        plt.savefig(plot_filename)
        plt.close(fig)
        print(f"Generated plot: {plot_filename}")

# --- Main Execution ---
if __name__ == "__main__":
    for title, filepath in LOG_FILES.items():
        measured_data = parse_and_average_logs(filepath)
        if not measured_data:
            continue
        
        print_data_table(title, measured_data)
        
        user_estimations = get_user_estimations(measured_data)
        
        generate_plots(title, measured_data, user_estimations)
    
    print("\nAnalysis complete.")