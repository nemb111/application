cmake_minimum_required(VERSION 3.10)

project(Optimization_example CXX)

add_executable(opt_ex
    sender.cpp
    receiver.cpp
    main.cpp
)

# Compile for 32-bit, with warnings and optimization
target_compile_options(opt_ex PRIVATE -m32 -Wall -Wextra -O2)

# Link for 32-bit
target_link_options(opt_ex PRIVATE -m32 -Wl,--gc-sections -Wl,-s)
