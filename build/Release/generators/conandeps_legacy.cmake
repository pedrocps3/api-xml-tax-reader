message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(Crow)
find_package(tinyxml2)

set(CONANDEPS_LEGACY  Crow::Crow  tinyxml2::tinyxml2 )