project(vecvec)

cmake_minimum_required(VERSION 2.8)

find_package(HDF5 REQUIRED)

include_directories(/home/tkroeger/inst/include)

add_library(vecvec SHARED vecvec.cxx vecvec_hdf5.cxx)

add_executable(vecvec_test vecvec_test.cxx)
target_link_libraries(vecvec_test vecvec ${HDF5_hdf5_LIBRARY})

install(FILES vecvec.hxx vecvec_hdf5.hxx DESTINATION include)
install(TARGETS vecvec  RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

enable_testing()

add_test("vecvec_test" vecvec_test)
