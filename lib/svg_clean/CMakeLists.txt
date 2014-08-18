cmake_minimum_required(VERSION 2.8)
project(svg_clean)
find_package(qibuild)

include(qibuild/modules/qt-tools)

include_directories(".")

set(
  svg_clean_public_header
  "svg_clean/svg_clean.hpp"
)

set(
  svg_clean_src
  "src/svg_clean.cpp"
)

qi_create_lib(
  svg_clean
  ${svg_clean_public_header}
  ${svg_clean_src}
)	

qi_use_lib(
  svg_clean
  QT_QTCORE
  QT_QTXML
)

qi_stage_lib(svg_clean)

qi_install_header(
  KEEP_RELATIVE_PATHS
  ${svg_clean_public_header}
)

# standalone
qi_create_bin(
  SvgCleaner
  "standalone/main.cpp"
  DEPENDS
  svg_clean
)


