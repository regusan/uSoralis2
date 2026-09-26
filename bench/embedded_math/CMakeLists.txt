cmake_minimum_required(VERSION 3.20)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(embedded_math_bench LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()

function(add_bench target source)
  add_executable(${target} ${source})
  target_link_libraries(${target} PRIVATE pico_stdlib m)
  target_compile_options(${target} PRIVATE
    -Os
    -ffunction-sections
    -fdata-sections
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  )
  target_link_options(${target} PRIVATE -Wl,--gc-sections)
  pico_enable_stdio_usb(${target} 0)
  pico_enable_stdio_uart(${target} 0)
  pico_add_extra_outputs(${target})
endfunction()

add_bench(bench_baseline baseline.cpp)

add_bench(bench_glm glm.cpp)
target_include_directories(bench_glm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/glm)

add_bench(bench_tinyregu tinyregu.cpp)
target_include_directories(bench_tinyregu PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/TinyReguMath3D/src
)
target_compile_definitions(bench_tinyregu PRIVATE TRM3D_NO_IOSTREAM)

add_bench(bench_bla bla.cpp)
target_include_directories(bench_bla PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/stubs
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/BasicLinearAlgebra
)

add_bench(bench_eigen eigen.cpp)
target_include_directories(bench_eigen PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/ArduinoEigen
)

add_bench(bench_cmsis cmsis.cpp)
target_sources(bench_cmsis PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/CMSIS-DSP/Source/BasicMathFunctions/arm_dot_prod_f32.c
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/CMSIS-DSP/Source/MatrixFunctions/arm_mat_mult_f32.c
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/CMSIS-DSP/Source/MatrixFunctions/arm_mat_inverse_f32.c
)
target_include_directories(bench_cmsis PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/CMSIS-DSP/Include
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/CMSIS-DSP/PrivateInclude
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/CMSIS_6/CMSIS/Core/Include
)
target_compile_definitions(bench_cmsis PRIVATE
  ARM_MATH_CM33
  DISABLEFLOAT16
)
