blt_add_executable(NAME test_sfr
                   SOURCES sfr.cpp slug_object.cpp
                   DEPENDS_ON mpi slug pcg gsl boost gtest)

blt_add_test(NAME test_sfr
             COMMAND test_sfr)

blt_add_code_checks(PREFIX test_sfr
                    SOURCES sfr.cpp slug_object.cpp)
