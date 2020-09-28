cmake_minimum_required(VERSION 3.1)
project(SlugSFR VERSION 1.0
  DESCRIPTION "Star formation skeleton code using the SLUG library"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# host-config (moved to separate file)
# [specify this with `cmake -C host_config.cmake ..`]

# include BLT

include(blt/SetupBLT.cmake)

# find external libraries

find_package(GSL REQUIRED)
blt_register_library(NAME gsl 
                     TREAT_INCLUDES_AS_SYSTEM off
                     INCLUDES ${GSL_INCLUDES}
                     LIBRARIES ${GSL_LIBRARIES})

set(Boost_USE_STATIC_LIBS off)
set(Boost_USE_MULTITHREADED off)
set(Boost_USE_STATIC_RUNTIME off)
find_package(Boost 1.60 REQUIRED COMPONENTS system)
message(STATUS "Boost version: ${Boost_VERSION}")
blt_register_library(NAME boost 
                     TREAT_INCLUDES_AS_SYSTEM off
                     INCLUDES ${BOOST_INCLUDES}
                     LIBRARIES ${BOOST_LIBRARIES})

include(ExternalProject)
ExternalProject_Add(project_slug
    SOURCE_DIR ${SlugSFR_SOURCE_DIR}/slug2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make lib MACHINE=linux-gnu FITS=DISABLE_FITS
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND cp <SOURCE_DIR>/src/libslug.so <INSTALL_DIR>/libslug.so
    BUILD_BYPRODUCTS <INSTALL_DIR>/libslug.so
)
include(ExternalProject)
ExternalProject_Get_Property(project_slug install_dir)
get_filename_component(slug_install_dir ${install_dir} ABSOLUTE)
message(STATUS "SLUG install directory is: ${slug_install_dir}")
ExternalProject_Get_Property(project_slug source_dir)
blt_register_library(NAME slug2
  TREAT_INCLUDES_AS_SYSTEM OFF
  DEFINES USE_SLUG=1
  INCLUDES ${source_dir}/src
  LIBRARIES ${slug_install_dir}/libslug.so)

# include source code

add_subdirectory(${SlugSFR_SOURCE_DIR}/src ${SlugSFR_BINARY_DIR}/src)