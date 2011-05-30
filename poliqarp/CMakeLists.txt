PROJECT(Corpus2Poliqarp)
cmake_minimum_required(VERSION 2.8.0)
include_directories("/usr/local/include/sakura/")
add_library(corpus2poliqarp SHARED pqclient.cpp)

target_link_libraries(corpus2poliqarp libpoliqarp corpus2)



