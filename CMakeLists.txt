cmake_minimum_required(VERSION 3.16)
project(my_qt_app)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable addr sanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lasan")

add_executable(my_qt_app main.cpp)

# Qt
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
qt6_add_resources(RCC_SOURCES images.qrc)
target_sources(my_qt_app PRIVATE ${RCC_SOURCES})
target_link_libraries(my_qt_app Qt6::Widgets)

# X11
find_package(X11 REQUIRED)
target_link_libraries(my_qt_app ${X11_LIBRARIES})
target_include_directories(my_qt_app PRIVATE ${X11_INCLUDE_DIR})
