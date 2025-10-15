# 1. 最低版本要求 (務必寫在最前面)
#   確保您的 CMake 版本足夠新，這是個好習慣。
cmake_minimum_required(VERSION 3.10) 

# 2. 定義專案名稱
#   這會隱式地定義一個變數 ${PROJECT_NAME}。
project(StructHW01 VERSION 1.0 LANGUAGES CXX) 

# 3. 設置編譯標準
#   推薦使用 C++17 或 C++20，確保代碼使用現代語法。
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) # 讓編譯器找不到標準時直接報錯

# 4. 收集源文件（可選，但常用於簡單專案）
#   FILE(GLOB ...) 會將當前目錄下所有 .cpp 文件存入變數 SOURCES
file(GLOB SOURCES "*.cpp" "src/*.cpp")

# 5. 添加頭文件路徑 (如果您的頭文件不在源文件同一目錄)
#   target_include_directories 專門用於指定目標的頭文件路徑
#   PRIVATE 表示只有此目標需要，PUBLIC 表示依賴此目標的其他目標也需要
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE 
    include 
)

# 6. 生成可執行文件
#   第一個參數是您最終可執行文件的名字
