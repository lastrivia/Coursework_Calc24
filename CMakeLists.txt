cmake_minimum_required(VERSION 3.28)
project(Calc)

set(APP_VERSION "2.2")
set(APP_VERSION_COMMA "2,2")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.3/mingw_64")
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_RC_COMPILER_INIT "windres")
add_definitions(-DAPP_VERSION="${APP_VERSION}")

configure_file(
        "${CMAKE_SOURCE_DIR}/version.rc.in"
        "${CMAKE_BINARY_DIR}/version.rc"
        @ONLY
)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Calc WIN32
        "${CMAKE_BINARY_DIR}/version.rc"
        main.cpp
        algorithm/rational.h
        algorithm/rational.cpp
        algorithm/operation.h
        algorithm/operation.cpp
        algorithm/expression.h
        algorithm/solver.h
        algorithm/solver.cpp
        algorithm/utils.h
        algorithm/utils.cpp
        algorithm/random.h
        algorithm/random.cpp
        interface/interface.h
        interface/menu.h
        interface/calculator.h
        interface/file_calculator.h
        interface/timed_game.h
        interface/online_game.h
        interface/settings.h
        interface/settings.cpp
        interface/poker_display.h
        interface/style.h
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
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/latexmk-build/description_cn.pdf ${CMAKE_SOURCE_DIR}/description_cn.pdf
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
