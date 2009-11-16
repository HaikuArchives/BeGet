cmake_minimum_required(VERSION 2.6)

project(BEGET)

set(CMAKE_MODULE_PATH "${BEGET_SOURCE_DIR}/CMakeModules")

include(UseHaiku)

find_library(LIBSGB sgb PATHS "${BEGET_SOURCE_DIR}/libs/Santa")

find_library(LIBTOOLBAR Toolbar PATHS "${BEGET_SOURCE_DIR}/libs/Toolbar")

find_library(LIBUTILS Utils PATHS "${BEGET_SOURCE_DIR}/libs/Utils")

find_library(LIBLOCALE locale PATHS "${BEGET_SOURCE_DIR}/libs/liblocale" NO_DEFAULT_PATH)

include_directories("${BEGET_SOURCE_DIR}/libs/Santa" "${BEGET_SOURCE_DIR}/libs/Toolbar"
					"${BEGET_SOURCE_DIR}/libs/Utils" "${BEGET_SOURCE_DIR}/libs/liblocale")

add_subdirectory(src)

