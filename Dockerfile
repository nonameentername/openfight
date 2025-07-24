FROM debian:bookworm-slim
RUN apt update -y \
    && apt install -y sudo vim-tiny build-essential cmake file pkg-config zip unzip \
    gcc-mingw-w64-i686 g++-mingw-w64-i686 \
    gcc-multilib-i686-linux-gnu g++-multilib-i686-linux-gnu \
    clang \
    libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libglu1-mesa-dev libglew-dev \
    libyaml-cpp-dev \
    xvfb
