#===============================================================
# Filename : Dockerfile - base image
# Purpose  : Multiplayer, networked Checkers game for CS451.
# Authors  : Corwin Belser <cmb539@drexel.edu>
#            Zach Brennan  < zab37@drexel.edu>
#            Kris Horsey   < kth37@drexel.edu>
#            Zach van Rijn < zwv23@drexel.edu>
# License  : MIT/X (excl. ext. libs; see respective licenses).
# Revision : 20170826
#===============================================================

FROM alpine:latest
MAINTAINER Zach van Rijn <me@zv.io>

#---------------------------------------------------------------
# README.

# (1) This Dockerfile builds all dependencies and tools required
#     by the Checkers project. It produces a 'base image' which
#     can be used as a development environment. This base image
#     only needs to be built once, unless other dependencies are
#     required.
#
# (2) A second Dockerfile is provided, which fetches the latest
#     version of the Checkers game from the repository, and then
#     builds/tests it. It should produce portable, static 64-bit
#     binaries which can be run on most of the systems we use,
#     including Tux where it might be graded.
#
# (3) Usage instructions are provided in the next section.

#---------------------------------------------------------------
# Build Instructions.

# Prerequisites
# -------------
#
# (1) You need root privileges on the system onto which you wish
#     to install or use Docker (sudo is OK). Docker is available
#     for Windows, too, and the steps will be roughly the same.
#
# (2) Platform-specific instructions for installing Docker:
#     https://www.docker.com/community-edition#/download
#
# Base Image
# ----------
#
# (1) Make sure you have the latest version of the project:
#
#     $ git clone git@git.zv.io:me/CS451-Checkers.git
#     $ cd CS451-Checkers
#
# (2) Build the base image
#
#     $ sudo docker build -f Dockerfile . -t alpine:checkers
#
# (3) To view the images on your system,
#
#     $ sudo docker images
#     REPOSITORY   TAG        IMAGE ID       CREATED      SIZE
#     alpine       checkers   2134cafe53ed   6 days ago   442MB
#
# (4) To create a container (an instance of this image):
#
#     $ A=$(sudo docker create -it alpine:checkers "/bin/bash")
#     $ sudo docker start $A && sudo docker attach $A
#
# (5) You are 'root' inside the container. You can type 'exit'
#     to detach from it. To re-enter, repeat the *second* line
#     above (start and attach, not create).
#
# (6) Other Docker usage can be found on their website.
#
# Checkers Game
# -------------
#
# (1) Previous version of this file included SSH credentials to
#     a limited-access user with permission to clone the repo.
#     This has since been removed, and replaced with a runtime
#     bind mount in Docker, under the assumption that containers
#     are launched via the Makefile in the root of the project.
#
# (2) As of yet, we have not decided if the server and client
#     will be standalone executables or if they will be combined
#     and run in a particular mode '--mode={client,server}' by
#     providing some flags.

#---------------------------------------------------------------
# Configuration.

# We use a mirror for some of the larger files due to bandwidth
# constraints; the original URLs are provided as comments where
# applicable.

ARG MIRROR=http://451.zv.io

ARG TMPDIR=/tmp

#---------------------------------------------------------------
# Base Utilities.

RUN echo >> /etc/apk/repositories \
    "http://dl-cdn.alpinelinux.org/alpine/edge/community"

RUN apk update && apk add --no-cache \
    bash      \
    cmake     \
    coreutils \
    g++       \
    gcc       \
    make      \
    sed       \
    upx       \
    vim       \
    wget

#---------------------------------------------------------------
# Dependencies.

# ncurses
ARG NCUR_VER=ncurses-6.0
RUN cd ${TMPDIR} \
    && wget -q ${MIRROR}/${NCUR_VER}.tar.gz \
    && tar -xzf ${NCUR_VER}.tar.gz \
    && cd ${NCUR_VER} \
    && ./configure \
        --datadir=$HOME \
        --disable-leaks \
    && make -j$(nproc) \
    && make install \
    && cd ../ \
    && rm -rf ${NCUR_VER}*

# libcheck
ARG LCHK_VER=check-0.11.0
RUN cd ${TMPDIR} \
    && wget -q ${MIRROR}/${LCHK_VER}.tar.gz \
    && tar -xzf ${LCHK_VER}.tar.gz \
    && cd ${LCHK_VER} \
    && ./configure \
        --enable-gcov \
        --enable-timeout-tests \
        --enable-fork \
        --enable-snprintf-replacement \
        --enable-timer-replacement \
        --disable-shared \
        --enable-static \
    && make -j$(nproc) \
    && make install \
    && cd ../ \
    && rm -rf ${LCHK_VER}*

# freetype
ARG TYPE_VER=freetype-2.8
RUN cd ${TMPDIR} \
    && wget -q ${MIRROR}/${TYPE_VER}.tar.gz \
    && tar -xzf ${TYPE_VER}.tar.gz \
    && cd ${TYPE_VER} \
    && ./configure \
        --with-zlib=no \
        --with-bzip2=no \
        --with-png=no \
        --with-harfbuzz=np \
        --disable-shared   \
        --enable-static \
    && make -j$(nproc) \
    && make install \
    && cd ../ \
    && rm -rf ${TYPE_VER}*

# (other tools)
RUN apk add --no-cache \
    openssh \
    pcre-dev \
    python \
    valgrind

# valgrind -- provided by package manager (see above)
#ARG VALG_VER=valgrind-3.13.0
#RUN cd ${TMPDIR} \
#    && wget -q ${MIRROR}/${VALG_VER}.tar.bz2 \
#    && tar -xjf ${VALG_VER}.tar.bz2 \
#    && cd ${VALG_VER} \
#    && ./configure \
#    && make -j$(nproc) \
#    && make install \
#    && cd ../ \
#    && rm -rf ${VALG_VER}*

# cppcheck
ARG CCHK_VER=cppcheck-1.79
RUN cd ${TMPDIR} \
    && wget -q ${MIRROR}/${CCHK_VER}.tar.gz \
    && tar -xzf ${CCHK_VER}.tar.gz \
    && cd ${CCHK_VER} \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make -j$(nproc) \
    && make install \
    && cd ../../ \
    && rm -rf ${CCHK_VER}*

#---------------------------------------------------------------
# Miscellaneous.

WORKDIR /home
