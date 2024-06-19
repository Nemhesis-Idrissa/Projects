cmake_minimum_required(VERSION 3.5)

project(TranslatorU VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

list(APPEND CMAKE_PREFIX_PATH "~/Qt/6.3.0/gcc_64")


set(APPICON_RC "${CMAKE_SOURCE_DIR}/appicon.rc")
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TranslatorU
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        menu.h menu.cpp
        translatorwindow.h translatorwindow.cpp
        translatorwindow.h translatorwindow.cpp
        ${APPICON_RC}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TranslatorU APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TranslatorU SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TranslatorU
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TranslatorU PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TranslatorU)
endif()
set_target_properties(TranslatorU PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TranslatorU
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(SSL_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/no-asm)
else()
  set(SSL_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR})
endif()

if(Qt6_VERSION VERSION_GREATER_EQUAL 6.5.0)

  if(NOT OPENSSL_ROOT_DIR)
    set(OPENSSL_ROOT_DIR ${SSL_ROOT_PATH}/ssl_3/${CMAKE_ANDROID_ARCH_ABI})
  endif()
  list(APPEND ANDROID_EXTRA_LIBS
       ${SSL_ROOT_PATH}/ssl_3/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_3.so
       ${SSL_ROOT_PATH}/ssl_3/${CMAKE_ANDROID_ARCH_ABI}/libssl_3.so)
else()
  if(NOT OPENSSL_ROOT_DIR)
    set(OPENSSL_ROOT_DIR ${SSL_ROOT_PATH}/ssl_1.1/${CMAKE_ANDROID_ARCH_ABI})
  endif()
  list(APPEND ANDROID_EXTRA_LIBS
       ${SSL_ROOT_PATH}/ssl_1.1/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_1_1.so
       ${SSL_ROOT_PATH}/ssl_1.1/${CMAKE_ANDROID_ARCH_ABI}/libssl_1_1.so)
endif()


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TranslatorU)
endif()
