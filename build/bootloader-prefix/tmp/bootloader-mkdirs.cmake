# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/sousou04/esp/esp-idf/components/bootloader/subproject"
  "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader"
  "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader-prefix"
  "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader-prefix/tmp"
  "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader-prefix/src"
  "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/sousou04/Documents/Armageddon v3.0.0/Armageddon_V3.0.0/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
