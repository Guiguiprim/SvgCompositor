cmake_minimum_required(VERSION 2.8)
project(svg_compose)
find_package(qibuild)

include(qibuild/modules/qt-tools)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(".")

set(
  svg_compose_public_header
  "svg_compose/svg_compose.hpp"
  "svg_compose/svg_assemblies_list.hpp"
  "svg_compose/svg_assembly.hpp"
  "svg_compose/svg_assembly_element.hpp"
  "svg_compose/svg_assembly_size.hpp"
)

set(
  svg_compose_src
  "src/svg_compose.cpp"
  "src/svg_assemblies_list.cpp"
  "src/svg_assembly.cpp"
  "src/svg_assembly_element.cpp"
  "src/svg_assembly_size.cpp"
)

qt4_wrap_cpp(
  svg_compose_moc
  "svg_compose/svg_assemblies_list.hpp"
  "svg_compose/svg_assembly.hpp"
)

qi_create_lib(
  svg_compose
  ${svg_compose_public_header}
  ${svg_compose_src}
  ${svg_compose_moc}
  STATIC
)

qi_use_lib(
  svg_compose
  QT_QTCORE
  QT_QTXML
  svg_clean
)

qi_stage_lib(svg_compose)

qi_install_header(
  KEEP_RELATIVE_PATHS
  ${svg_compose_public_header}
)

