cmake_minimum_required(VERSION 2.8)
project(svg_compositor)

find_package(qibuild)

#include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(".")

set(
  svg_compositor_src
  "main.cpp"
  "icon.hpp"
  "icon.cpp"

  "command/assembly_command.hpp"
  "command/assembly_command.cpp"

  "controller/action.hpp"
  "controller/button_bar_controller.hpp"
  "controller/button_bar_controller.cpp"
  "controller/compositor_controller.hpp"
  "controller/compositor_controller.cpp"
  "controller/editor_controller.hpp"
  "controller/editor_controller.cpp"
  "controller/view_controller.hpp"
  "controller/view_controller.cpp"

  "gui/button_bar.hpp"
  "gui/button_bar.cpp"
  "gui/compositor_widget.hpp"
  "gui/compositor_widget.cpp"
  "gui/editor.hpp"
  "gui/editor.cpp"
  "gui/graphics_item.hpp"
  "gui/graphics_item.cpp"
  "gui/graphics_view.hpp"
  "gui/graphics_view.cpp"
  "gui/movable_svg_item.hpp"
  "gui/movable_svg_item.cpp"

  "misc/last_dir.hpp"
  "misc/last_dir.cpp"
  "misc/move_struct.hpp"
)

qt4_wrap_cpp(
  svg_compositor_moc

  "controller/button_bar_controller.hpp"
  "controller/compositor_controller.hpp"
  "controller/editor_controller.hpp"
  "controller/view_controller.hpp"

  "gui/button_bar.hpp"
  "gui/compositor_widget.hpp"
  "gui/editor.hpp"
  "gui/graphics_item.hpp"
  "gui/graphics_view.hpp"
  "gui/movable_svg_item.hpp"
)

qt4_wrap_ui(
  svg_compositor_ui
  "ui/button_bar.ui"
  "ui/mainwindow.ui"
)

qt4_add_resources(
  svg_compositor_rcc
  "res/media.qrc"
)

qi_create_bin(
  SvgCompositor
  ${svg_compositor_ui}
  ${svg_compositor_rcc}
  ${svg_compositor_moc}
  ${svg_compositor_src}
)	

qi_use_lib(
  SvgCompositor

  QT_QTCORE
  QT_QTGUI
  QT_QTXML
  QT_QTSVG
  svg_clean
  svg_compose
)
