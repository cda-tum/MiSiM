# set required cmake version
cmake_minimum_required(VERSION 3.19)

# This avoids googletest complaining that this (IPO) policy is not set
cmake_policy(SET CMP0069 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# project definition
project(
  MiSiM
  LANGUAGES CXX
  VERSION 0.1
  DESCRIPTION "MQT decision diagram package for mixed dimensional systems")

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/CheckSubmodule.cmake)
include(cmake/PackageAddTest.cmake)

include(cmake/ExternalDependencies.cmake)

# Use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# Standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Interface library to set project options
add_library(project_options INTERFACE)

# Compiler options
include(cmake/CompilerOptions.cmake)
enable_project_options(project_options)

# Sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

set(BINDINGS
    ON
    CACHE INTERNAL "Enable settings related to Python bindings")
# Some common settings for finding Python
set(Python_FIND_VIRTUALENV
    FIRST
    CACHE STRING "Give precedence to virtualenvs when searching for Python")
set(Python_FIND_FRAMEWORK
    LAST
    CACHE STRING "Prefer Brew/Conda to Apple framework Python")
set(Python_ARTIFACTS_INTERACTIVE
    ON
    CACHE BOOL "Prevent multiple searches for Python and instead cache the results.")

# top-level call to find Python
find_package(
  Python 3.8 REQUIRED
  COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

find_package(pybind11 REQUIRED)

# add main library code
add_library(
  ${PROJECT_NAME}
  INTERFACE
  include/dd/Complex.hpp
  include/dd/ComplexCache.hpp
  include/dd/ComplexNumbers.hpp
  include/dd/ComplexTable.hpp
  include/dd/ComplexValue.hpp
  include/dd/ComputeTable.hpp
  include/dd/Control.hpp
  include/dd/Definitions.hpp
  include/dd/Edge.hpp
  include/dd/GateMatrixDefinitions.hpp
  include/dd/MDDPackage.hpp
  include/dd/UnaryComputeTable.hpp
  include/dd/UniqueTable.hpp)

# add options and warnings to library
target_link_libraries(${PROJECT_NAME} INTERFACE project_options project_warnings)

# set include directories
target_include_directories(${PROJECT_NAME} INTERFACE include ${PROJECT_BINARY_DIR}/include)

# add MQT alias
add_library(MQT::MiSiM ALIAS ${PROJECT_NAME})
add_subdirectory(src)

# add test code
option(BUILD_MISIM_TESTS "Also build tests for MiSiM")

if(BUILD_MISIM_TESTS)
  check_submodule_present(googletest)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(test)
endif()
