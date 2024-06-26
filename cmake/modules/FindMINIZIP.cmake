#=============================================================================
# Copyright 2016-2017 Psi+ Project, Vitaly Tonkacheyev
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================
if (MINIZIP_INCLUDE_DIR AND MINIZIP_LIBRARY)
    # in cache already
    set(MINIZIP_FIND_QUIETLY TRUE)
endif ()

if ( UNIX AND NOT( APPLE OR CYGWIN ) )
    find_package( PkgConfig QUIET )
    pkg_check_modules( PC_MINIZIP QUIET minizip )
    set ( MINIZIP_DEFINITIONS
        ${PC_MINIZIP_CFLAGS}
        ${PC_MINIZIP_CFLAGS_OTHER}
    )
endif()

set ( LIBINCS
    unzip.h
)

find_path(
    MINIZIP_INCLUDE_DIR ${LIBINCS}
    HINTS
    ${MINIZIP_ROOT}/include
    ${PC_MINIZIP_INCLUDEDIR}
    ${PC_MINIZIP_INCLUDE_DIRS}
    PATH_SUFFIXES
    ""
    if ( NOT WIN32 )
    minizip
    endif()
)

find_library(
    MINIZIP_LIBRARY
    NAMES minizip
    HINTS
    ${PC_MINIZIP_LIBDIR}
    ${PC_MINIZIP_LIBRARY_DIRS}
    ${MINIZIP_ROOT}/lib
    ${MINIZIP_ROOT}/bin
    ${MINIZIP_DIR}/lib
    ${MINIZIP_DIR}/bin
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
                MINIZIP
                DEFAULT_MSG
                MINIZIP_LIBRARY
                MINIZIP_INCLUDE_DIR
)
if ( MINIZIP_FOUND )
    set ( MINIZIP_LIBRARIES ${MINIZIP_LIBRARY} )
    set ( MINIZIP_INCLUDE_DIRS ${MINIZIP_INCLUDE_DIR} )
endif()

mark_as_advanced( MINIZIP_INCLUDE_DIR MINIZIP_LIBRARY )
