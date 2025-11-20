cmake_minimum_required(VERSION 3.16)

project(qt_yolov11 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        inference.cpp
        mainwindow.h
        inference.h
        mainwindow.ui
)

set(OpenCV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/build/include")
set(OpenCV_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/build/x64/vc16/lib")
set(OpenCV_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/build/x64/vc16/bin")

include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIR}/opencv4)

link_directories(${OpenCV_LIB_DIR})

# 打印调试信息
message(STATUS "OpenCV Library Directory: ${OpenCV_LIB_DIR}")
message(STATUS "OpenCV Include Directory: ${OpenCV_INCLUDE_DIR}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")

# 检查库文件是否存在
if(EXISTS "${OpenCV_LIB_DIR}/opencv_world481d.lib")
    message(STATUS "Found Debug library: opencv_world481d.lib")
endif()
if(EXISTS "${OpenCV_LIB_DIR}/opencv_world481.lib")
    message(STATUS "Found Release library: opencv_world481.lib")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt_yolov11
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qt_yolov11 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qt_yolov11 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qt_yolov11
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qt_yolov11 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

# 链接OpenCV库（使用完整路径，支持Debug和Release配置）
# 注意：如果使用MinGW编译器，需要MinGW版本的OpenCV库（而不是vc16版本）
target_link_libraries(qt_yolov11 PRIVATE 
    "$<$<CONFIG:Debug>:${OpenCV_LIB_DIR}/opencv_world481d.lib>"
    "$<$<CONFIG:Release>:${OpenCV_LIB_DIR}/opencv_world481.lib>"
    "$<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${OpenCV_LIB_DIR}/opencv_world481.lib>"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qt_yolov11)
endif()
set_target_properties(qt_yolov11 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS qt_yolov11
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt_yolov11)
endif()
