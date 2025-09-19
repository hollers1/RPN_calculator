cmake_minimum_required(VERSION 3.10)
project(RPNCalculator)

set(CMAKE_CXX_STANDARD 11)

# Включаем директории с исходными файлами
include_directories(include)

# Добавляем исполняемый файл
add_executable(rpn_calculator
    src/main.cpp
    src/rpn.cpp
)

# Устанавливаем свойства для более строгой компиляции
set_target_properties(rpn_calculator PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

enable_testing()
add_subdirectory(tests)

# Тесты
# enable_testing()
# add_subdirectory(tests)