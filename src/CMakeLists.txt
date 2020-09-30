blt_add_executable(NAME test_sfr
    SOURCES sfr.cpp slug_object.cpp
    DEPENDS_ON mpi slug pcg gsl boost gtest)

blt_add_executable(NAME test_rng
    SOURCES rng.cpp
    DEPENDS_ON pcg gtest)

blt_add_test(NAME test_sfr
    COMMAND test_sfr)

blt_add_test(NAME test_rng
    COMMAND test_rng)

blt_add_code_checks(PREFIX test_sfr
    SOURCES sfr.cpp slug_object.cpp)

blt_add_code_checks(PREFIX test_rng
    SOURCES rng.cpp)
