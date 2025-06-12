# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Codes/C/wlib/thirdparty/openssl"
  "D:/Codes/C/wlib/build/openssl_project-prefix/src/openssl_project-build"
  "D:/Codes/C/wlib/build/openssl_project-prefix"
  "D:/Codes/C/wlib/build/openssl_project-prefix/tmp"
  "D:/Codes/C/wlib/build/openssl_project-prefix/src/openssl_project-stamp"
  "D:/Codes/C/wlib/build/openssl_project-prefix/src"
  "D:/Codes/C/wlib/build/openssl_project-prefix/src/openssl_project-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Codes/C/wlib/build/openssl_project-prefix/src/openssl_project-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Codes/C/wlib/build/openssl_project-prefix/src/openssl_project-stamp${cfgdir}") # cfgdir has leading slash
endif()
