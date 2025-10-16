import subprocess
import time
import sys
import os
from pathlib import Path

# --- 設定 ---
MAX_RUNTIME = 600  # 10 分鐘 = 600 秒
C_PLUS_PLUS_EXECUTABLE = Path("./build/StructHW01.exe")

# 定義實驗類型
EXPERIMENTS = {
    "uniform": "result.log",
    "dense": "result1.log"
}

STRUCTURE_TYPES = range(1, 4)
K_VALUES = range(11, 26)
REPETITIONS = 10

def run_single_test(struct_type, k_value, exp_type):
    """執行單次 C++ 測試"""
    arguments = [str(struct_type), str(k_value), exp_type]
    command_list = [C_PLUS_PLUS_EXECUTABLE.as_posix()] + arguments

    print(f"-- Executing: {' '.join(command_list)}")

    start_time = time.time()
    try:
        result = subprocess.run(
            command_list,
            capture_output=True,
            text=True,
            timeout=MAX_RUNTIME,
            check=False  # Don't raise exception for non-zero exit codes
        )

        if result.returncode != 0:
            print(f"  -> C++ program exited with error code {result.returncode}.")
            print(f"  -> Stderr: {result.stderr.strip()}")

    except subprocess.TimeoutExpired:
        print(f"  -> WARNING: Timeout after {MAX_RUNTIME} seconds.")
        return -1  # Return -1 to indicate timeout
    except FileNotFoundError:
        print(f"\nERROR: Executable not found at '{C_PLUS_PLUS_EXECUTABLE}'. Please compile it first.")
        sys.exit(1)
    except Exception as e:
        print(f"\nAn unexpected error occurred: {e}")
        return -2 # Return -2 for other errors

    return 1 # Success

if __name__ == "__main__":
    # 遍歷兩種實驗
    for exp_type, log_file in EXPERIMENTS.items():
        print(f"\n{'=' * 25} STARTING EXPERIMENT: {exp_type.upper()} {'=' * 25}")
        print(f"Output will be logged to: {log_file}")

        # 在實驗開始前，清空對應的 log 檔案
        if os.path.exists(log_file):
            os.remove(log_file)
            print(f"Cleared old log file: {log_file}")

        # 遍歷三種資料結構
        for stype in STRUCTURE_TYPES:
            timeout_occurred = False
            print(f"\n{'-' * 20} Testing Structure Type: {stype} {'-' * 20}")

            # 遍歷 k 值 (n = 2^k)
            for k in K_VALUES:
                if timeout_occurred:
                    print(f"  -> Skipping k={k} and subsequent values due to previous timeout.")
                    continue

                print(f"  Running tests for k={k} (n=2^{k})")
                # 重複執行 10 次
                for i in range(REPETITIONS):
                    print(f"    Repetition {i+1}/{REPETITIONS}...")
                    run_result = run_single_test(stype, k, exp_type)

                    if run_result == -1:  # Timeout
                        print(f"  -> Timeout occurred for structure {stype} at k={k}.")
                        timeout_occurred = True
                        break  # Break from repetitions loop
                    elif run_result == -2: # Other error
                        print("  -> Halting tests for this structure due to an unexpected error.")
                        timeout_occurred = True # Treat as fatal for this structure
                        break

                if timeout_occurred:
                    break # Break from k_values loop

    print("\nAll experiments finished.")
