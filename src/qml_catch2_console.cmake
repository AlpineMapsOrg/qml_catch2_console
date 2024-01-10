#############################################################################
# QML Catch2 console
# Copyright (C) 2023 Adam Celarek <family name at cg tuwien ac at>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#############################################################################

set(qml_catch2_console_dir ${CMAKE_CURRENT_LIST_DIR})

if (NOT TARGET Catch2)
    alp_add_git_repository(catch2 URL https://github.com/catchorg/Catch2.git COMMITISH v3.5.1)
endif()

if (NOT TARGET fmt)
    alp_add_git_repository(fmt URL https://github.com/fmtlib/fmt.git COMMITISH 10.1.1)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL Quick QuickControls2)
qt_standard_project_setup()

function(add_qml_catch2_console_unittests target_name)
    qt_add_executable(${target_name}
        ${qml_catch2_console_dir}/qml_catch2_console/main.cpp
        ${qml_catch2_console_dir}/qml_catch2_console/StdBuffer.h ${qml_catch2_console_dir}/qml_catch2_console/StdBuffer.cpp
        ${ARGN}
    )

    target_link_libraries(${target_name} PUBLIC Catch2::Catch2 fmt Qt::Core Qt::Gui Qt6::Quick Qt6::QuickControls2)

    if (EMSCRIPTEN)
        target_link_options(${target_name} PUBLIC -sASYNCIFY -Os)
    endif()

    set_source_files_properties(${qml_catch2_console_dir}/qml_catch2_console/console.qml PROPERTIES
         QT_RESOURCE_ALIAS qml_catch2_console/console.qml
    )

    qt_add_qml_module(${target_name}
        URI src
        VERSION 1.0
        RESOURCE_PREFIX /qt/qml
        QML_FILES
            ${qml_catch2_console_dir}/qml_catch2_console/console.qml
    )
endfunction() 
