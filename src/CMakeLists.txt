find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")

find_package(GSL REQUIRED)

set(Boost_USE_STATIC_LIBS off)
set(Boost_USE_MULTITHREADED off)
set(Boost_USE_STATIC_RUNTIME off)
find_package(Boost 1.60 REQUIRED COMPONENTS system)
message(STATUS "Boost version: ${Boost_VERSION}")

include(ExternalProject)
ExternalProject_Add(project_slug
    URL https://bitbucket.org/krumholz/slug2/get/master.tar.gz
    PREFIX slug2-bitbucket
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make lib MACHINE=linux-gnu FITS=DISABLE_FITS
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND cp <SOURCE_DIR>/src/libslug.so <INSTALL_DIR>/libslug.so
    BUILD_BYPRODUCTS <INSTALL_DIR>/libslug.so
)
ExternalProject_Get_Property(project_slug install_dir)
get_filename_component(slug_install_dir ${install_dir} ABSOLUTE)
message(STATUS "SLUG install directory is: ${slug_install_dir}")
add_library(slug2 STATIC IMPORTED)
set_property(TARGET slug2 PROPERTY IMPORTED_LOCATION ${slug_install_dir}/libslug.so)
add_dependencies(slug2 project_slug)

add_executable(test_sfr sfr.cpp slug_object.cpp)
ExternalProject_Get_Property(project_slug source_dir)
include_directories(${source_dir}/src)
target_link_libraries(test_sfr PUBLIC MPI::MPI_CXX)
target_link_libraries(test_sfr PUBLIC GSL::gsl)
target_link_libraries(test_sfr PUBLIC Boost::system)
target_link_libraries(test_sfr PUBLIC slug2)

include(CTest)
add_test(NAME StarFormation COMMAND test_sfr)