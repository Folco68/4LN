cmake_minimum_required(VERSION 3.16)

project(4LN VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    # Root
    BeforeRelease.hpp
    Global.hpp
    main.cpp

    # Core
    Core/CopyData.cpp
    Core/CopyData.hpp
    Core/DriveData.cpp
    Core/DriveData.hpp
    Core/FileData.cpp
    Core/FileData.hpp
    Core/ThreadAnalyze.cpp
    Core/ThreadAnalyze.hpp
    Core/ThreadClone.cpp
    Core/ThreadClone.hpp

    # Docs
    Docs/Docs.qrc
    Docs/Changelog.txt
    Docs/License.txt
    Docs/Readme.txt
    Docs/TODO.txt

    # Resources
    Resources/Pictures.qrc
    Resources/Box.png
    Resources/DownArrow.png
    Resources/Folder.png
    Resources/LeftArrow.png
    Resources/RightArrow.png
    Resources/USBstick.png

    # UI
    UI/MainWindow.cpp
    UI/MainWindow.hpp
    UI/MainWindow.ui
    UI/WidgetDestination.cpp
    UI/WidgetDestination.hpp
    UI/WidgetDestination.ui
    UI/WidgetDrop.cpp
    UI/WidgetDrop.hpp
    UI/WidgetDrop.ui
    UI/WidgetOverwrite.cpp
    UI/WidgetOverwrite.hpp
    UI/WidgetOverwrite.ui
    UI/WindowAnalyze.cpp
    UI/WindowAnalyze.hpp
    UI/WindowAnalyze.ui
    UI/WindowClone.cpp
    UI/WindowClone.hpp
    UI/WindowClone.ui
    UI/WindowDiff.cpp
    UI/WindowDiff.hpp
    UI/WindowDiff.ui
    UI/WindowHelp.cpp
    UI/WindowHelp.hpp
    UI/WindowHelp.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(4LN
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Resources/Icon.rc

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 4LN APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(4LN SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(4LN
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(4LN PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.4LN)
endif()
set_target_properties(4LN PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS 4LN
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(4LN)
endif()
