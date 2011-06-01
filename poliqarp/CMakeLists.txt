PROJECT(Corpus2Poliqarp)
cmake_minimum_required(VERSION 2.8.0)
include_directories(${PoliqarpLibrary_SOURCE_DIR}/sakura)
include_directories(${PoliqarpLibrary_SOURCE_DIR})
include_directories(${PoliqarpLibrary_BINARY_DIR}/sakura)
include_directories(${PoliqarpLibrary_BINARY_DIR})
add_library(corpus2poliqarp SHARED pqclient.cpp)

add_executable(c2pqtest c2pqtest.cpp)
target_link_libraries(corpus2poliqarp libpoliqarp corpus2)
target_link_libraries(c2pqtest libpoliqarp corpus2poliqarp corpus2 pwrutils )




