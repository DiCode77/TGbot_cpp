cmake_minimum_required(VERSION 3.36)
project(TGbotCpp)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXA "${CMAKE_CURRENT_SOURCE_DIR}/TGbotCpp/samples")

set(PROJ_LIB_NAME "${PROJECT_NAME}Lib")

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

#// for Lib
file(GLOB SRC_FILES_LIB "${SRC}/TGbotCpp/*.cpp" 
                        "${SRC}/TGbotCpp/*.hpp" 
                        "${SRC}/TGbotCpp/curl/*.cpp" 
                        "${SRC}/TGbotCpp/curl/*.hpp" 
                        "${SRC}/Resources/*.hpp")

add_library(${PROJ_LIB_NAME} STATIC ${SRC_FILES_LIB})

target_include_directories(${PROJ_LIB_NAME} PUBLIC ${SRC}/TGbotCpp 
                                                   ${SRC}/TGbotCpp/curl 
                                                   ${SRC}/Resources)
                                                   
target_link_libraries(${PROJ_LIB_NAME} PUBLIC CURL::libcurl nlohmann_json::nlohmann_json)

set_target_properties(${PROJ_LIB_NAME} PROPERTIES CXX_STANDARD 20 
                                                 CXX_STANDARD_REQUIRED ON)

source_group("src" FILES ${SRC_FILES_LIB})

#//

set(EXA_FILES "${EXA}/main.cpp")

add_executable(${PROJECT_NAME} ${EXA_FILES}) 

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC}/TGbotCpp 
                                                   ${SRC}/TGbotCpp/curl 
                                                   ${SRC}/Resources)

target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl nlohmann_json::nlohmann_json ${PROJ_LIB_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                 CXX_STANDARD_REQUIRED ON)

source_group("samples" FILES ${EXA_FILES})
