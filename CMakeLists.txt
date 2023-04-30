cmake_minimum_required(VERSION 3.20)

project(NetEaseToo CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_SOURCE_DIR}/src/*.h" 
    "${CMAKE_SOURCE_DIR}/src/*.hpp" 
    "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE JSON_FILES 
    "${CMAKE_SOURCE_DIR}/libraries/json/*.h" 
    "${CMAKE_SOURCE_DIR}/libraries/json/*.cpp")

add_executable(NetEaseToo ${SRC_FILES} ${JSON_FILES})

add_subdirectory(libraries/HTTPRequest)
add_subdirectory(libraries/FTXUI)
add_subdirectory(libraries/taglib)

target_link_libraries(NetEaseToo PRIVATE 
    HTTPRequest
)

target_include_directories(NetEaseToo PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libraries/httprequest/include
    ${CMAKE_SOURCE_DIR}/libraries/json
)


