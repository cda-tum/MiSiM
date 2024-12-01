pybind11_add_module(
  pymisim
  # Prefer thin LTO if available
  THIN_LTO
  # Optimize the bindings for size
  OPT_SIZE
  # Source code goes here
  bindings.cpp)
target_link_libraries(pymisim PRIVATE MQT::MiSiM)

# Install directive for scikit-build-core
install(
  TARGETS pymisim
  DESTINATION .
  COMPONENT mqt-misim_PythonModule)
