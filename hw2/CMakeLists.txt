cmake_minimum_required(VERSION 2.8.4)
project(hw3)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp GLWidget.cpp MainWindow.cpp)
configure_file(model.obj ${CMAKE_CURRENT_BINARY_DIR}/model.obj COPYONLY)
configure_file(1.fsh ${CMAKE_CURRENT_BINARY_DIR}/1.fsh COPYONLY)
configure_file(2.fsh ${CMAKE_CURRENT_BINARY_DIR}/2.fsh COPYONLY)
configure_file(1.vsh ${CMAKE_CURRENT_BINARY_DIR}/1.vsh COPYONLY)
configure_file(2.vsh ${CMAKE_CURRENT_BINARY_DIR}/2.vsh COPYONLY)


set(QT_USE_QTOPENGL TRUE)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5OpenGL_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
include_directories(${Qt5OpenGL_DEFINITIONS})
QT5_ADD_RESOURCES(hw3_RESOURCES_RCC ${hw3_RESOURCES})
set(CMAKE_CXX_FLAGS "-std=c++0x ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_executable(hw3 ${SOURCE_FILES} ${hw3_RESOURCES_RCC})
target_link_libraries(hw3 ${OPENGL_LIBRARIES})
target_link_libraries(hw3 ${Qt5Widgets_LIBRARIES})
target_link_libraries(hw3 ${Qt5OpenGL_LIBRARIES})