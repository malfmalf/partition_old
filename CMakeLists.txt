cmake_minimum_required(VERSION 2.8.2)
PROJECT(partition)

set(CIVSIM_DIRECTORY ${CMAKE_SOURCE_DIR}/../civsim)

INCLUDE(${CIVSIM_DIRECTORY}/cmake-scripts/CMakeLists-common.txt)
INCLUDE(${CIVSIM_DIRECTORY}/cmake-scripts/external/cmake-qt-${GENERATOR_SHORT_NAME}.txt)

#cambio esto qeu lo fija CMakeLists-common y qeu aqui tienen qeu ir a otro sitio
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/_bin/${GENERATOR_SHORT_NAME})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/_install/${GENERATOR_SHORT_NAME})

ADD_DEFINITIONS(-DDATA_PATH="${CMAKE_SOURCE_DIR}/data")

FIND_PACKAGE(QT4)
INCLUDE(${QT_USE_FILE})
MESSAGE("Generando Airplay workspaces...")
execute_process(COMMAND "C:/Airplay SDK/4.3/s3e/bin/mkb.bat" --buildenv=VC10 --non-interactive --no-ide PartitionAirplay.mkb WORKING_DIRECTORY airplay)
# esto no me interesa de momento , demasiado lioso : include_external_msproject(BalloonsAirplay_vc10 ${CMAKE_SOURCE_DIR}/airplay/build_balloonsairplay_vc10/BalloonsAirplay_vc10.vcxproj)
ADD_SUBDIRECTORY(game)
