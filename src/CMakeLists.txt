cmake_minimum_required(VERSION 3.21)
project(qml_catch2_console_lib LANGUAGES CXX)


include(FetchContent)
FetchContent_Declare(catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.3.2
    SOURCE_DIR 		${CMAKE_SOURCE_DIR}/3rdparty/Catch2
)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git)
FetchContent_MakeAvailable(catch2 fmt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL Quick QuickControls2)

qt_add_library(qml_catch2_console STATIC
    qml_catch2_console/main.cpp
)
target_link_libraries(qml_catch2_console PUBLIC Catch2::Catch2 PRIVATE fmt Qt::Core Qt::Gui Qt6::Quick Qt6::QuickControls2)


qt_add_qml_module(qml_catch2_console
    URI src
    VERSION 1.0
    RESOURCE_PREFIX /qt/qml
    QML_FILES
        qml_catch2_console/console.qml
)

#set_target_properties(qml_catch2_console PROPERTIES
#    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
#)

#if (ANDROID OR EMSCRIPTEN)
#    target_compile_definitions(qml_catch2_console PUBLIC ALP_QML_SOURCE_DIR="qrc:///qt/qml/qml_catch2_console/")
#else()
#    target_compile_definitions(qml_catch2_console PUBLIC ALP_QML_SOURCE_DIR="file:/${CMAKE_CURRENT_SOURCE_DIR}/")
#endif()

#if (ANDROID)
#    install(TARGETS alpineapp
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
#endif()

#if (EMSCRIPTEN)
    #set(ALP_INSTALL_FILES
        #"$<TARGET_FILE_DIR:alpineapp>/alpineapp.js"
        #"$<TARGET_FILE_DIR:alpineapp>/alpineapp.wasm"
        #"$<TARGET_FILE_DIR:alpineapp>/alpineapp.html"
        #"$<TARGET_FILE_DIR:alpineapp>/qtloader.js"
    #)

    #if (ALP_ENABLE_THREADING)
        #list(APPEND ALP_INSTALL_FILES "$<TARGET_FILE_DIR:alpineapp>/alpineapp.worker.js")
    #endif()
    #install(FILES ${ALP_INSTALL_FILES} DESTINATION ${ALP_WWW_INSTALL_DIR})
#endif()
