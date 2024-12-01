include(FetchContent)
set(FETCH_PACKAGES "")

if(BUILD_MQT_MISIM_BINDINGS)
  if(NOT SKBUILD)
    # Manually detect the installed pybind11 package.
    execute_process(
      COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
      OUTPUT_STRIP_TRAILING_WHITESPACE
      OUTPUT_VARIABLE pybind11_DIR)

    # Add the detected directory to the CMake prefix path.
    list(APPEND CMAKE_PREFIX_PATH "${pybind11_DIR}")
  endif()

  # add pybind11 library
  find_package(pybind11 CONFIG REQUIRED)
endif()

if(BUILD_MQT_MISIM_BINDINGS)
  # add pybind11_json library
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.20)
    # Prefer using FetchContent instead of find_package for pybind11_json
    FetchContent_Declare(
      pybind11_json
      GIT_REPOSITORY https://github.com/pybind/pybind11_json
      GIT_TAG v3.0.1) # specify a release tag or commit hash if needed
    FetchContent_MakeAvailable(pybind11_json)
  else()
    message(WARNING "CMake version is too old to use FetchContent. Please consider updating.")
    find_package(pybind11_json QUIET)
    if(NOT pybind11_json_FOUND)
      message(FATAL_ERROR "pybind11_json package not found. Please install it.")
    endif()
  endif()
endif()
