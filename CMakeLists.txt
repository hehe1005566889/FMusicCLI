cmake_minimum_required(VERSION 3.20)

project(NetEaseToo CXX)
set(CXX_STANDARD_REQUIRED 17)

file(GLOB_RECURSE jsrc "libraries/json/*.h" "libraries/json/*.cpp")

file(GLOB_RECURSE src "src/*.h" "src/*.hpp" "src/*.cpp")
message(${src})
add_executable(NetEaseToo ${src} ${jsrc})

add_subdirectory(libraries/httprequest)
add_subdirectory(libraries/fxtui)
add_subdirectory(libraries/taglibs)

target_link_libraries(NetEaseToo 
    HTTPRequest
)
target_include_directories(NetEaseToo PRIVATE 
    ${CMAKE_SOURCE_DIR}/libraries/httprequest/include
    ${CMAKE_SOURCE_DIR}/libraries/json
    ${CMAKE_SOURCE_DIR}/libraries/taglibs/taglib
    ${CMAKE_SOURCE_DIR}/src
)