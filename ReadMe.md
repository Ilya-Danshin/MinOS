## Operating System

Based on [this](https://github.com/AlgorithMan-de/wyoos) tutorial. 
___
### Build
 Install packages: 

    sudo apt-get install g++ binutils libc6-dev-i386
    sudo apt-get install VirtualBox grub-common xorriso
    sudo apt install grub-pc-bin
Build:

    make build
After this u got an kernel.iso file that can be run like this:

    qemu-system-i386.exe kernel.iso

___
### Menu navigation
For navigation in menu use keys `w` and `s`. For confirm use `Enter`.