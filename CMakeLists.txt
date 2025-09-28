cmake_minimum_required(VERSION 3.36)
project(TGbotCpp)

set(SRC "${CMAKE_SOURCE_DIR}/")

include_directories(${SRC}) 

file(GLOB SRC_FILES "${SRC}/TGbotCpp/*.cpp" 
                    "${SRC}/TGbotCpp/*.hpp" 
                    "${SRC}/TGbotCpp/curl/*.cpp" 
                    "${SRC}/TGbotCpp/curl/*.hpp" 
                    "${SRC}/TGbotCpp/samples/*.cpp"
                    "${SRC}/Resources/*.hpp")


add_executable(${PROJECT_NAME} ${SRC_FILES}) 

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 
                                                 CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC}/TGbotCpp 
                                                   ${SRC}/TGbotCpp/curl 
                                                   ${SRC}/Resources)


find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)