#
# This file is part of the KDAB State Machine Editor Library.
#
# SPDX-FileCopyrightText: 2014-2020 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
# Author: Kevin Funk <kevin.funk@kdab.com>
#
# SPDX-License-Identifier: LGPL-2.1-only OR LicenseRef-KDAB-KDStateMachineEditor
#
# Licensees holding valid commercial KDAB State Machine Editor Library
# licenses may use this file in accordance with the KDAB State Machine Editor
# Library License Agreement provided with the Software.
#
# Contact info@kdab.com if any conditions of this licensing are not clear to you.
#

set(LIB_SRCS
  command/command.cpp
  command/commandfactory.cpp
  command/createelementcommand.cpp
  command/deleteelementcommand.cpp
  command/layoutsnapshotcommand.cpp
  command/modifyinitialstatecommand.cpp
  command/modifydefaultstatecommand.cpp
  command/modifyelementcommand.cpp
  command/modifypropertycommand.cpp
  command/modifytransitioncommand.cpp
  command/reparentelementcommand.cpp
  command/changestatemachinecommand.cpp

  quick/qopengl2pexvertexarray.cpp # Copy because QtGui doesn't export all symbols
  quick/quickmaskedmousearea.cpp
  quick/quickpainterpath.cpp
  quick/quickpen.cpp
  quick/quickprimitiveitem.cpp
  quick/quickkdsmeglobal.cpp
  quick/quickrecursiveinstantiator.cpp
  quick/quicksceneitem.cpp

  widgets/codeeditor.cpp
  widgets/propertyeditor.cpp
  widgets/statemachinetoolbar.cpp
  widgets/statemachineview.cpp
  widgets/statemachinepalettewidget.cpp

  abstractscene.cpp
  statemachinescene.cpp
  abstractcontroller.cpp
  commandcontroller.cpp
  debug.cpp
  editcontroller.cpp
  semanticzoommanager.cpp
)

set(LIB_UIS
  widgets/statepropertyeditor.ui
  widgets/transitionpropertyeditor.ui
)

qt5_wrap_ui(LIB_SRCS ${LIB_UIS})

set(LIB_QRCS kdsme.qrc)
qt5_add_resources(LIB_SRCS ${LIB_QRCS})

# Make the files show up in Qt Creator
set(QML_FILES
  qml/util/ChannelizedDropArea.qml
  qml/util/DragPointGroup.qml
  qml/util/SquareButton.qml
  qml/util/DragPoint.qml
  qml/util/BirdEyeView.qml
  qml/util/DragPointDelegate.qml
  qml/util/ToolTip.qml
  qml/util/ArrowHead.qml
  qml/SceneItemFactory.qml
  qml/StateMachineView.qml
  qml/StateMachineViewToolBar.qml
  qml/uml/UmlCompositeStateDelegate.qml
  qml/uml/UmlTransition.qml
  qml/uml/UmlPseudoState.qml
  qml/uml/UmlFinalState.qml
  qml/uml/UmlHistoryState.qml
  qml/uml/UmlStateMachine.qml
  qml/uml/UmlState.qml
  qml/uml/UmlSimpleStateDelegate.qml
  qml/Scene.qml
)

add_library(kdstatemachineeditor_view SHARED ${LIB_SRCS} ${LIB_QRCS} ${QML_FILES})
add_library(KDSME::View ALIAS kdstatemachineeditor_view)
target_link_libraries(kdstatemachineeditor_view
  LINK_PUBLIC
    Qt5::Quick
    Qt5::QuickWidgets
    Qt5::Widgets
    kdstatemachineeditor_core
)

set(build_iface_dirs
  ${CMAKE_CURRENT_SOURCE_DIR}/command
  ${CMAKE_CURRENT_SOURCE_DIR}/widgets
)

set_target_properties(kdstatemachineeditor_view PROPERTIES
  SOVERSION ${KDSME_SOVERSION}
  VERSION ${KDSME_SOVERSION}
  EXPORT_NAME View
  INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
)
#version libraries on Windows
if(WIN32)
  set(postfix ${KDSME_SOVERSION})
  set(CMAKE_RELEASE_POSTFIX ${postfix})
  set_target_properties(kdstatemachineeditor_view PROPERTIES RELEASE_POSTFIX ${CMAKE_RELEASE_POSTFIX})
  #append 'd' to debug libraries
  string(CONCAT postfix ${postfix} "d")
  set(CMAKE_DEBUG_POSTFIX ${postfix})
  set_target_properties(kdstatemachineeditor_view PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif()

generate_export_header(kdstatemachineeditor_view EXPORT_FILE_NAME kdsme_view_export.h BASE_NAME KDSME_VIEW)
target_include_directories(kdstatemachineeditor_view
  # FIXME: It is recommended to use a relative path here.
  # Doesn't work with CMake 2.8.11, though, probably needs 2.8.12(?)
  PUBLIC
    "$<BUILD_INTERFACE:${build_iface_dirs}>"
    # FIXME: It is recommended to use a relative path here.
    # Doesn't work with CMake 2.8.11, though, probably needs 2.8.12(?)
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
target_include_directories(kdstatemachineeditor_view
  SYSTEM PRIVATE ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

install(TARGETS kdstatemachineeditor_view EXPORT KDSME_TARGETS ${INSTALL_TARGETS_DEFAULT_ARGS})
if(MSVC)
  install(FILES "$<TARGET_PDB_FILE_DIR:kdstatemachineeditor_view>/$<TARGET_PDB_FILE_NAME:kdstatemachineeditor_view>" DESTINATION ${LIB_INSTALL_DIR} CONFIGURATIONS Debug RelWithDebInfo)
endif()
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kdsme_view_export.h

  abstractcontroller.h
  abstractscene.h
  commandcontroller.h
  editcontroller.h
  statemachinescene.h

  widgets/propertyeditor.h
  widgets/statemachinepalettewidget.h
  widgets/statemachineview.h
  widgets/statemachinetoolbar.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/view
)

ecm_generate_pri_file(BASE_NAME KDSMEView
                      LIB_NAME kdstatemachineeditor_view
                      DEPS "core gui qml quickwidgets widgets KDSMECore"
                      FILENAME_VAR PRI_FILENAME
                      INCLUDE_INSTALL_DIR ${INCLUDE_INSTALL_DIR}/view
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
