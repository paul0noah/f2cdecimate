cmake_minimum_required(VERSION 3.16)
project(fine2coarsedec)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release)

## load external content
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# pybind11
include(pybind)
# Libigl
include(libigl)


# switch between python bindings and executable
if( BUILD_PYTHON_BINDINGS )
    message("Building Python Bindings")
    find_package(Python3 REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
    pybind11_add_module(fine2coarsedec pythonbinding/decimate.cpp)
    add_subdirectory(pythonbinding)
elseif( BUILD_MEX_FILE )
    message("Building MEX files")
    find_package(Matlab REQUIRED)
    set(PROJECT_NAME f2cdecimate_mex)
    add_subdirectory(mex)
else()
    message("Building CPP Executable")
    add_executable(${PROJECT_NAME} main.cpp)
endif()


# set project include dir
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

# Add your project files
add_subdirectory(include)
if( NOT BUILD_MEX_FILE )
	target_link_libraries(${PROJECT_NAME} PUBLIC igl::core)
endif()
