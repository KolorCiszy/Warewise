cmake_minimum_required(VERSION 3.16)

project(Warewise VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Sql)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h

        dashboardWindow.cpp
        dashboardWindow.h

        user.h
        user.cpp
      )


set(UI_FILES
        dashboardWindow.ui
        mainwindow.ui
      )
 #qt_add_resources(qtres resources.qrc) #  Remove this line if you do not have a .qrc file

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(Warewise main.cpp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${UI_FILES}
        resources/WareWiseIcon.rc
        AppResources.qrc
        setNewPasswordForm.ui
        setNewPasswordForm.h setNewPasswordForm.cpp setNewPasswordForm.ui
        gdatabasehelpers.h gdatabasehelpers.cpp
        addNewArticleForm.h addNewArticleForm.cpp addNewArticleForm.ui
        Article.h
        gProjectVersion.h
        ArticleDisplayList.h ArticleDisplayList.cpp ArticleDisplayList.ui
        AccountDetails.h AccountDetails.cpp AccountDetails.ui





    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Warewise APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Warewise SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Warewise
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Warewise PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Warewise PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Warewise)
endif()
set_target_properties(Warewise PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Warewise
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Warewise)
endif()
