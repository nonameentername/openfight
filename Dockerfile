FROM debian:bookworm-slim
RUN apt update -y \
    && apt install -y sudo vim-tiny build-essential cmake file pkg-config zip unzip \
    gcc-mingw-w64-i686 g++-mingw-w64-i686 \
    gcc-multilib-i686-linux-gnu g++-multilib-i686-linux-gnu \
    clang \
    libsdl1.2-dev libsdl-ttf2.0-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-net1.2-dev \
    libxml2-dev libglu1-mesa-dev