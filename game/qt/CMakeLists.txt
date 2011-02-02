PROJECT(PartitionGameQT)
FILE(GLOB SPECIFIC_FILES *.cpp *.h *.ui *.qrc)
FILE(GLOB BASE_FILES ../../base/*.cpp ../../base/*.h ../../base/qt/*.cpp ../../base/qt/*.h)

QT4_WRAP_UI(ui_PartitionGame.h PartitionGame.ui)
QT4_WRAP_CPP(MOCED_FILES PartitionGameWindow.h PartitionMainWindow.h)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../base)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../base/qt)

ADD_EXECUTABLE(PartitionGameQT 
			${SPECIFIC_FILES} 
            ${BASE_FILES} 
            ${MOCED_FILES}
           )                       

TARGET_LINK_LIBRARIES(PartitionGameQT ${QT_LIBRARIES})








