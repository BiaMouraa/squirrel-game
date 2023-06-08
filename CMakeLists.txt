cmake_minimum_required(VERSION 3.0.0)
project(teste VERSION 0.1.0)

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(projeto-openCV facedetect_extra.cpp frutinha.cpp)

target_link_libraries(projeto-openCV ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)