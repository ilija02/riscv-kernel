# Basic RiscV kernel

## Introduction

This is the repository for my final project in the operating systems class. The project focuses on creating an operating system for a RiscV processor, which is emulated by QEMU. The operating system relies on lib/hw.lib which provides basic interfaces for communicating with the host operating system . The project aims to demonstrate a range of essential operating system functionalities through both C and C++ APIs.

## Operating System Functionalities

### Heap Memory Management using continuous allocation
  
- **Allocation**: The operating system allows dynamic memory allocation for various data structures.
- **Deallocation**: Proper deallocation of memory resources to prevent memory leaks.

### Threads and Multithreading

- **Multithreading & Scheduling**: The OS supports multiple threads and provides scheduling mechanisms for efficient resource utilization.
- **Synchronous Context Switching**: Threads can perform synchronous context switching for controlled execution flow.
- **Asynchronous Context Switching**: Threads can perform asynchronous context switching for handling events and interrupt-driven tasks.
- **Thread Joining**: Threads can be joined for synchronization purposes.

### Semaphores

- **General Purpose Semaphores**: The operating system supports general-purpose semaphores for synchronization.

### Timer and Threads

- **Threads Sleeping**: Threads can be paused or delayed for a specified time.

### Console I/O

- **getc**: Read character input from the console.
- **putc**: Write character output to the console.

Note: *console functions have bugs and current implementations uses the already finished methods from lib/console.lib*


## Building and running the project 

This project was built using Clion inside the Ubuntu VM provided by the University. Virtual machine can be downloaded from [here](https://drive.google.com/file/d/1edGYFcvdnV0pbKws_1G1vePtEec0qC0G/view).

Initial project setup instructions can be found [here](http://os.etf.bg.ac.rs/OS1/projekat/Projektni%20zadatak%202023.%20v1.0.pdf).
