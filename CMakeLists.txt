cmake_minimum_required(VERSION 3.21)
project(GenshiCraft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SRC_FILES_DIR
        ${PROJECT_SOURCE_DIR}/SDK/include/llapi/*.cpp
        ${PROJECT_SOURCE_DIR}/SDK/include/llapi/*.hpp
        ${PROJECT_SOURCE_DIR}/SDK/include/llapi/*.h
        ${PROJECT_SOURCE_DIR}/src/*.cc
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/characters/*.cc
        ${PROJECT_SOURCE_DIR}/src/characters/*.h
        ${PROJECT_SOURCE_DIR}/src/weapons/*.cc
        ${PROJECT_SOURCE_DIR}/src/weapons/*.h
        )

include_directories(SDK/include)
include_directories(SDK/include/llapi)
include_directories(src)

link_directories(.)

add_definitions(
        -D"NDEBUG"
        -D"NOMINMAX"
        -D"TEMPLATE_EXPORTS"
        -D"UNICODE"
        -D"WIN32_LEAN_AND_MEAN"
        -D"_AMD64_"
        -D"_CRT_SECURE_NO_WARNINGS"
        -D"_UNICODE"
        -D"_USRDLL"
        -D"_WINDLL"
        -D"_WINDOWS"
)

add_compile_options(
        /diagnostics:column
        /EHsc
        /FC
        /GL
        /MD
        /nologo
        /permissive-
        /sdl
        /utf-8
        /Zc:inline
        /Zi
)

add_link_options(
        /DEBUG
        /DELAYLOAD:"bedrock_server.dll"
        /DLL
        /IGNORE:4199
        /INCREMENTAL:NO
        /LTCG:INCREMENTAL
        /MANIFESTUAC:NO
        /MACHINE:X64
        /NOLOGO
        /OPT:ICF
        /OPT:REF
        /SUBSYSTEM:CONSOLE
)

add_library(plugin SHARED ${SRC_FILES_DIR})

target_link_libraries(plugin
        "${CMAKE_SOURCE_DIR}/SDK/lib/LiteLoader.lib"
        "${CMAKE_SOURCE_DIR}/SDK/lib/bedrock_server_api.lib"
        "${CMAKE_SOURCE_DIR}/SDK/lib/bedrock_server_var.lib"
        "${CMAKE_SOURCE_DIR}/SDK/lib/SymDBHelper.lib"
        )

add_library(GenshiCraft SHARED ${SRC_FILES_DIR})

add_custom_command(TARGET GenshiCraft PRE_BUILD
        COMMAND cmd /c ${PROJECT_SOURCE_DIR}/prepareLib.cmd ${PROJECT_SOURCE_DIR}
        COMMENT "Preparing Library"
        )
