cmake_minimum_required(VERSION 2.8)

project(ScutRacing)
add_definitions(-std=c++11)

# add your EIGEN dir
find_package(Eigen3 REQUIRED)

include_directories(External/matplotlib)
include_directories(include)
include_directories(${EIGEN3_INCLUDE_DIR})

aux_source_directory(src ALL_SRCS)
add_executable(run_me ${ALL_SRCS})

# find python
find_package(Python2 COMPONENTS Development NumPy)
target_include_directories(run_me PRIVATE ${Python2_INCLUDE_DIRS}
                                          ${Python2_NumPy_INCLUDE_DIRS})
target_link_libraries(run_me Python2::Python Python2::NumPy)

# install
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/bin/")
install(TARGETS run_me RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
