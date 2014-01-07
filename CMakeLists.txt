project(vecvec)

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(HDF5 REQUIRED)
find_package(Vigra REQUIRED)

include_directories(${VIGRA_INCLUDE_DIR})

add_library(vecvec SHARED vecvec.cxx vecvec_hdf5.cxx)

add_executable(vecvec_test vecvec_test.cxx)
target_link_libraries(vecvec_test vecvec ${HDF5_hdf5_LIBRARY})

install(FILES vecvec.hxx vecvec_hdf5.hxx DESTINATION include)
install(TARGETS vecvec  RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

enable_testing()

add_test("vecvec_test" vecvec_test)
