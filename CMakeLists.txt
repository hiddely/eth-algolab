cmake_minimum_required(VERSION 3.12)
project(algolab)

message(STATUS "start running cmake...")

find_package(Boost 1.67.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(BoostTest main.cpp)

if(Boost_FOUND)

    target_link_libraries(BoostTest ${Boost_LIBRARIES})

endif()

set(CMAKE_CXX_STANDARD 11)

add_executable(algolab main.cpp week1/even_pairs/main.cpp week1/build_the_sum/main.cpp week1/dominoes/main.cpp week1/even_matrices/main.cpp week1/potw/main.cpp)
add_executable(strings practice/strings/main.cpp)
add_executable(even_pairs week1/even_pairs/main.cpp)
add_executable(build_the_sum week1/build_the_sum/main.cpp)
add_executable(dominoes week1/dominoes/main.cpp)
add_executable(even_matrices week1/even_matrices/main.cpp)
add_executable(week1potw week1/potw/main.cpp)
add_executable(search_snippets week2/search_snippets/main.cpp)
add_executable(beach_bars week2/beach_bars/main.cpp)
add_executable(week2potw week2/potw/main.cpp)
add_executable(burning_coins week2/burning_coins/main.cpp)