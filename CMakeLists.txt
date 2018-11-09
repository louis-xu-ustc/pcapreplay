cmake_minimum_required(VERSION 3.2)    
project(pcapreplay CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(pcapreplay src/pcapreplay.cc)
target_compile_options(pcapreplay PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(pcapreplay pcap)

install(TARGETS pcapreplay DESTINATION bin/)
