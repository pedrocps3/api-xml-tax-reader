cmake_minimum_required(VERSION 3.10)
project(ApiXmlReader)

add_executable(ApiXmlReader main.cpp)

find_package(Crow REQUIRED)

add_library(ICMSInformation ICMSInformation.cpp)

target_link_libraries(ApiXmlReader PRIVATE Crow::Crow tinyxml2 ICMSInformation)
