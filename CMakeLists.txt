cmake_minimum_required(VERSION 3.15)
project(BongoCat)

include(GenerateG1A)
include(GenerateG3A)
include(GenerateHH2Bin)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/main.c
)

set(ASSETS
)
set(ASSETS_fx
)
set(ASSETS_cg
  assets-cg/bongocat.jpg
  assets-cg/bongocat-r.jpg
  assets-cg/bongocat-l.jpg
  assets-cg/bongocat-lr.jpg
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(bongocat ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(bongocat PRIVATE -Wall -Wextra -Os -g -flto)
target_link_libraries(bongocat Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET bongocat OUTPUT "BongoCat.g1a"
    NAME "BongoCat" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET bongocat OUTPUT "BongoCat.g3a"
    NAME "BongoCat" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G_G3A)
  generate_g3a(TARGET bongocat OUTPUT "BongoCat-fx.g3a"
    NAME "BongoCat-fx" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCP)
  generate_hh2_bin(TARGET bongocat OUTPUT "BongoCat-hh2.bin")
endif()
