cmake_minimum_required(VERSION 3.21)
project(GenshiCraft-llplugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SRC_FILES_DIR
        ${PROJECT_SOURCE_DIR}/SDK/Header/*.cpp
        ${PROJECT_SOURCE_DIR}/SDK/Header/*.hpp
        ${PROJECT_SOURCE_DIR}/SDK/Header/*.h
        ${PROJECT_SOURCE_DIR}/src/*.cc
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/characters/*.cc
        ${PROJECT_SOURCE_DIR}/src/characters/*.h
        ${PROJECT_SOURCE_DIR}/src/weapons/*.cc
        ${PROJECT_SOURCE_DIR}/src/weapons/*.h
        )

include_directories(SDK/Header)
include_directories(SDK/Header/third-party)
include_directories(src)

link_directories(.)

add_definitions(-D"NDEBUG" -D"TEMPLATE_EXPORTS" -D"WIN32_LEAN_AND_MEAN" -D"_CRT_SECURE_NO_WARNINGS" -D"_WINDOWS"
        -D"_USRDLL" -D"_AMD64_" -D"NOMINMAX" -D"_WINDLL" -D"_UNICODE" -D"UNICODE")

add_compile_options(
        /permissive- /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /sdl /Zc:inline /fp:precise /errorReport:prompt /WX-
        /Zc:forScope /Gd /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:column /utf-8
)

add_link_options(
        /MANIFEST /LTCG:incremental /NXCOMPAT /DEBUG /DLL /MACHINE:X64 /OPT:REF /INCREMENTAL:NO /SUBSYSTEM:WINDOWS
        /MANIFESTUAC:NO /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /DELAYLOAD:"bedrock_server.dll" /TLBID:1
)

add_library(GenshiCraft-llplugin SHARED ${SRC_FILES_DIR})

add_custom_command(TARGET GenshiCraft-llplugin PRE_BUILD
        COMMAND cmd /c ${PROJECT_SOURCE_DIR}/prepareLib.cmd ${PROJECT_SOURCE_DIR}
        COMMENT "Preparing Library"
        )
