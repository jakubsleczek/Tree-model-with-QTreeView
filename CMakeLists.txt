cmake_minimum_required(VERSION 3.16)

project(TreeApp LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(TreeApp
  main.cpp
  treeitem.h
  treeitem.cpp
  treemodel.h
  treemodel.cpp
  mainwindow.h
  mainwindow.cpp
)

#set_target_properties(TreeApp PROPERTIES
#    WIN32_EXECUTABLE TRUE
#    MACOSX_BUNDLE TRUE
#)

target_link_libraries(TreeApp Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(TreeApp Qt${QT_VERSION_MAJOR}::Widgets)

# Resources:
set(treeapp_resource_files
    "odczyt.txt"
)

qt_add_resources(TreeApp "TreeApp"
    PREFIX
        "/"
    FILES
        ${treeapp_resource_files}
)

install(TARGETS TreeApp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
