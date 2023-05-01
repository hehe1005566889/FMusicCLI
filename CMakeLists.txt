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
    tag
)

target_include_directories(NetEaseToo PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libraries/httprequest/include
    ${CMAKE_SOURCE_DIR}/libraries/json
)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/taglib
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/taglib/toolkit
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/taglib/ape
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/taglib/mpeg
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/taglib/mpeg/id3v1
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/taglib/mpeg/id3v2
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/taglib/mpeg/id3v2/frames
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries/taglib/bindings/c/
)

add_definitions(-DTAGLIB_STATIC)

install(TARGETS ${CMAKE_PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


