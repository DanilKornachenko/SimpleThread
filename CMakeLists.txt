cmake_minimum_required(VERSION 3.10) # Минимальная версия CMake
project(SimpleThread)

# Стандарт С++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Находим библиотеку ncrses
find_package(Curses REQUIRED)

# Добавляем исполняемый файл
add_executable(SimpleThread app/SimpleThread.cpp)

# Линкуем библиотеку
target_link_libraries(SimpleThread ${CURSES_LIBRARIES})

# Указываем директорию для заголовочных файлов
include_directories(${CURSES_INCLUDE_DIRS})
