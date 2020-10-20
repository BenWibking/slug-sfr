blt_add_executable(NAME test_sfr
    SOURCES sfr.cpp slug_wrapper.cpp
    DEPENDS_ON slug gsl boost gtest)

blt_add_executable(NAME test_rng
    SOURCES rng.cpp
    DEPENDS_ON pcg gtest)

blt_add_test(NAME test_sfr
    COMMAND test_sfr)

blt_add_test(NAME test_rng
    COMMAND test_rng)

blt_add_code_checks(PREFIX test_sfr
    SOURCES sfr.cpp slug_wrapper.cpp)

blt_add_code_checks(PREFIX test_rng
    SOURCES rng.cpp)

blt_add_executable(NAME slug_benchmark
    SOURCES slug_bench.cpp slug_wrapper.cpp
    DEPENDS_ON slug gbenchmark)

blt_add_benchmark(NAME slug_benchmark
    COMMAND slug_benchmark)