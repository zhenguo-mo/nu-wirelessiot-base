# Northwestern Wireless IoT Base

Base files for Northwestern course CS397/497: Wireless Protocols for IoT. This
repository has applications and board initialization that connects to the build
system provided by [nrf52x-base](https://github.com/lab11/nrf52x-base).


## Requirements
 
 1. Various tools
 
    * build-essential (make)
    * python3
    * git

 2. ARM cross-compiler toolchain: [gcc-arm-none-eabi toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)

    On Ubuntu:

        cd /tmp \
        && wget -c https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
        && tar xjf gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
        && sudo mv gcc-arm-none-eabi-9-2020-q2-update /opt/gcc-arm-none-eabi-9-2020-q2-update \
        && rm gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 \
        && sudo ln -s /opt/gcc-arm-none-eabi-9-2020-q2-update/bin/* /usr/local/bin/.

    On MacOS:

        brew tap ArmMbed/homebrew-formulae
        brew install arm-none-eabi-gcc

 3. JTAG programming tools: [Segger JLink](https://www.segger.com/jlink-software.html)

    Pick the correct package for your OS under "Software and documentation pack".


## Installation

 * Clone the repository
 * Change directory into the repository
 * `git submodule update --init --recursive` (this will take a couple minutes to complete)


## Building and loading applications

To build an application, use `make` inside of the application's directory.

Two other targets will be particularly helpful:

 * `make flash` uploads the application to the board using JTAG
 * `make rtt` opens a debug window with print output from the board

