cmake_minimum_required(VERSION 3.12)
project(algolab)

message(STATUS "start running cmake...")

find_package(Boost 1.67.0 COMPONENTS system filesystem REQUIRED)
find_package(CGAL QUIET COMPONENTS Core )
include(${CGAL_USE_FILE})

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

if ( CGAL_FOUND )

    include( ${CGAL_USE_FILE} )

#    include( CGAL_CreateSingleSourceCGALProgram )

else()

    message(STATUS "This program requires the CGAL library, and will not be compiled.")

endif()

add_executable(graphs week4/graphs/main.cpp)
add_executable(importantbridges week4/importantbridges/main.cpp)
add_executable(buddies week4/buddies/main.cpp)
add_executable(week6potw week6/potw/main.cpp)

if(Boost_FOUND)

    target_link_libraries(graphs ${Boost_LIBRARIES})
    target_link_libraries(importantbridges ${Boost_LIBRARIES})
    target_link_libraries(buddies ${Boost_LIBRARIES})
    target_link_libraries(week6potw ${Boost_LIBRARIES})

endif()

set(CMAKE_CXX_STANDARD 11)

add_executable(algolab main.cpp)
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
add_executable(great_game week2/great_game/main.cpp)
add_executable(luigi_gg week2/great_game/lol.cpp)
add_executable(hit week3/hit/main.cpp)
add_executable(firsthit week3/firsthit/main.cpp)
add_executable(antenna week3/antenna/main.cpp)
add_executable(almost_antenna week3/almost-antenna/main.cpp)
add_executable(week3potw week3/potw/main.cpp)
add_executable(light_at_the_museum week5/light_at_the_museum/main.cpp)
add_executable(attack_of_the_clones week5/attack_of_the_clones/main.cpp)
add_executable(punch week5/punch/main.cpp)