# openfight

![openfight](./images//openfight-1.png)
![openfight](./images//openfight-2.png)
![openfight](./images//openfight-3.png)
![openfight](./images//openfight-4.png)

https://github.com/user-attachments/assets/1d26f60d-91d8-4dae-ac71-6a7d8bc14b7e




This is an open source fighting game written in C++ using SDL.

The project can be built using cmake.


To build in ubuntu, install the required packages:

    sudo apt install build-essential libxml2-dev libsdl2-dev libsdl-image2-dev libglu1-mesa-dev libglew-dev

Create a directory that is used for the build and generates the platform-specific makefiles:

    cmake -Bbuild
    cmake --build build
    

The project can then be compiled using the generated makefiles:

    ./openfight
