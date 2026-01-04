cmake_minimum_required(VERSION 3.19)
project(Electrical_House_Wiring_System LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(Electrical_House_Wiring_System
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    grid.h
    grid.cpp






    astar.h
    astar.cpp




    gridwidget.h
    gridwidget.cpp
    astarfan.h
    astarfan.cpp
    custom_queue.h
)

qt_add_translations(
    TARGETS Electrical_House_Wiring_System
    TS_FILES Electrical_House_Wiring_System_en_AS.ts
)

target_link_libraries(Electrical_House_Wiring_System
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Electrical_House_Wiring_System
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Electrical_House_Wiring_System
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
