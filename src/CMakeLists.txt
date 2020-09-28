blt_add_executable(NAME test_sfr
                   SOURCES sfr.cpp slug_object.cpp
                   DEPENDS_ON mpi slug2 gsl boost gtest)

blt_add_test(NAME test_sfr
             COMMAND test_sfr)