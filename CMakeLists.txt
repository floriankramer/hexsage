cmake_minimum_required(VERSION 3.11)
project(hexsage)

# Add colored output for Ninja
if ("${CMAKE_GENERATOR}" STREQUAL "Ninja")
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
     add_compile_options (-fdiagnostics-color=always)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
     add_compile_options (-fcolor-diagnostics)
  endif ()
endif()

# PkgConfig
find_package(PkgConfig REQUIRED)

# Gtk 3
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
add_definitions(${GTK3_CFLAGS_OTHER})

# gtkmm
pkg_check_modules(GTKMM gtkmm-3.0)

add_executable(hexsage src/main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/Model.cpp src/Model.h
    src/HexViewWidget.cpp src/HexViewWidget.h
    src/HexsageApplication.cpp src/HexsageApplication.h)
target_include_directories(hexsage PUBLIC ${GTK3_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})
target_link_libraries(hexsage ${GTK3_LIBRARIES} ${GTKMM_LIBRARIES})
