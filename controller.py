import subprocess
import time
import sys
from pathlib import Path

# --- 設定 ---
MAX_RUNTIME = 600  # 10 分鐘 = 600 秒
C_PLUS_PLUS_EXECUTABLE = Path("./build/StructHW01.exe")


def run_struct(STRUCT_TYPE, LOG):
    arguments = [str(STRUCT_TYPE), str(LOG)]

    command_list = [C_PLUS_PLUS_EXECUTABLE.as_posix()] + arguments

    print(f"將執行的指令: {' '.join(command_list)}")
    print(f"最大運行時間設定為: {MAX_RUNTIME} 秒 (10 分鐘)")

    start_time = time.time()
    try:
        # 使用 subprocess.run 執行外部程式
        # timeout 參數是關鍵，它會在大於指定時間後發出 SIGTERM 訊號終止程式
        result = subprocess.run(
            command_list,  # 捕獲 C++ 程式的標準輸出和錯誤輸出
            capture_output=True,  # 以文本模式處理輸出
            text=True,  # 設定超時時間
            timeout=MAX_RUNTIME,
        )

        end_time = time.time()

        # 程式在時限內結束
        print("-" * 30)
        print("程式在時限內完成。")
        print(f"實際運行時間: {end_time - start_time:.2f} 秒")
        print("-" * 30)

        # 輸出 C++ 程式的結果 (例如您計算的 Checksum)
        print("C++ 輸出：")
        print(result.stdout)
        return 1

    except subprocess.TimeoutExpired:
        # 程式被 Python 強制停止
        end_time = time.time()
        print("-" * 30)
        print(f"\n警告：程式已運行超過 {MAX_RUNTIME} 秒！")
        print("Python 已強制停止 C++ 程式。")
        print(f"程式運行時間約為: {end_time - start_time:.2f} 秒")
        print("-" * 30)
        return -1

    except FileNotFoundError:
        print(
            f"\n錯誤：找不到可執行檔 '{C_PLUS_PLUS_EXECUTABLE}'。請確認路徑和檔案名稱是否正確。"
        )
        sys.exit(1)

    except Exception as e:
        print(f"\n發生未知錯誤: {e}")


if __name__ == "__main__":
    for stype in range(1, 4):
        timeout_occurred = False  # 為每個資料結構重設超時旗標
        print(f"\n{'=' * 20} 開始測試資料結構 {stype} {'=' * 20}")
        for slog in range(11, 26):
            if timeout_occurred:
                print(f"由於先前在 n=2^{slog - 1} 時已超時，跳過所有後續的 n 值測試。")
                break  # 跳出 slog 迴圈，直接測試下一個資料結構

            for t in range(10):
                run_result = run_struct(stype, slog)
                if run_result == -1:  # -1 代表超時
                    print(
                        f"結構 {stype} 在 n=2^{slog} 時超時，將跳過此結構後續的所有測試。"
                    )
                    timeout_occurred = True  # 設定旗標
                    break  # 跳出 t 迴圈
