cmake_minimum_required(VERSION 3.19)
project(CBCTool-Qt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/APP/bin)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(CBCTool-Qt
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    trajectory_calculator.cpp
    trajectory_calculator.h
)

target_link_libraries(CBCTool-Qt
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_compile_definitions(CBCTool-Qt PRIVATE _USE_MATH_DEFINES)

install(TARGETS CBCTool-Qt
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CBCTool-Qt
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
