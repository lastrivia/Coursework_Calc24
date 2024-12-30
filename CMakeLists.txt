cmake_minimum_required(VERSION 3.28)
project(Calc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.3/mingw_64")
set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Calc WIN32
        main.cpp
        rational.h
        rational.cpp
        operation.h
        expression.h
        expression.cpp
        solver.h
        solver.cpp
        interface.h
        interface_menu.h
        interface.cpp
        interface_settings.h
        interface_file.h
        interface_online_game.h
        interface_timed_game.h
        interface_poker_display.h
        interface_basic.h
        poker.h
        poker.cpp
        style.h
)

target_link_libraries(Calc
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# CMake: Add DLLs & extern resources

set(DLL_FILES
        "C:/Qt/6.7.3/mingw_64/bin/libgcc_s_seh-1.dll"
        "C:/Qt/6.7.3/mingw_64/bin/libstdc++-6.dll"
        "C:/Qt/6.7.3/mingw_64/bin/libwinpthread-1.dll"
)

add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILES} ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/img ${CMAKE_BINARY_DIR}/img
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/styles ${CMAKE_BINARY_DIR}/styles
)

add_dependencies(Calc CopyResources)



if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
