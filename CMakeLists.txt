cmake_minimum_required(VERSION 3.20)
project(lab-2 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG")

add_compile_options(/W4 /EHsc)

# папка де лежать послідовності
add_compile_definitions(DATA_DIR="${CMAKE_SOURCE_DIR}/data")


add_executable(generate_data generate.cpp sequences.cpp sequences.h)
target_compile_options(generate_data PRIVATE /O2)


set(LAB_FILES
    main.cpp
    experiments.cpp experiments.h
    predicates.cpp predicates.h
    sequences.cpp sequences.h
)

# експерименти з макс оптимізацією
add_executable(lab2_max ${LAB_FILES})
target_compile_options(lab2_max PRIVATE /O2)
target_compile_definitions(lab2_max PRIVATE "OPT_NAME=\"maximum (/O2)\"")

# і ті самі без оптимізації
add_executable(lab2_none ${LAB_FILES})
target_compile_options(lab2_none PRIVATE /Od)
target_compile_definitions(lab2_none PRIVATE "OPT_NAME=\"none (/Od)\"")