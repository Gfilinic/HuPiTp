cmake_minimum_required(VERSION 3.16)

project(HuPiTp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Quick Graphs Quick3D Widgets Charts QuickWidgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    customchartview.h
    customchartview.cpp
    dht22.cpp
    dht22.h
    resources.qrc
)

qt_add_executable(appHuPiTp
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

qt_add_qml_module(appHuPiTp
    URI HuPiTp
    VERSION 1.0
    QML_FILES
        Main.qml
        Temperature.qml
        Humidity.qml
        Log.qml
)

set_target_properties(appHuPiTp PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appHuPiTp
    PRIVATE Qt6::Core
            Qt6::Quick
            Qt6::Graphs
            Qt6::Quick3D
            Qt6::Widgets
            Qt6::Charts
            Qt6::QuickWidgets
    PRIVATE wiringPi
)

include(GNUInstallDirs)
install(TARGETS appHuPiTp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(appHuPiTp)
endif()
