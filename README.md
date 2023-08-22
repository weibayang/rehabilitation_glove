# rehabilitation_glove
A soft-packaged and portable rehabilitation glove capable of closed-loop fine motor skills

Readme File
Project Name:A soft-packaged and portable rehabilitation glove capable of fine motor skills

Description:
This repository contains the source code and documentation for the Rehabilitation Glove Project. The Rehabilitation Glove is designed to assist individuals with hand mobility issues in their recovery process. It includes software and hardware components for hand rehabilitation exercises.
“GeneralUI” is an upper interface program developed in MATLAB. It is designed for glove debugging, experiment data collection, and analysis. This program provides a user-friendly graphical interface for interacting with the glove and conducting experiments related to rehabilitation or hand mobility research.
“controller/stm32F407ZET6” repository contains firmware for glove control, specifically designed to run on systems based on the STM32F407ZET6 micro-controller. The firmware facilitates the control and operation of the glove for various applications, including rehabilitation and hand mobility research.
“sampling” encompasses the pertinent program and procedural framework for the calibration of sensor angles with the assistance of LED indicators.
“sma_test” repository includes the necessary code and upper-level software program for conducting 10,000 repeated experiments on Shape Memory Alloys (SMA).

Platform:
The code is intended to run on the following platform(s):
- Windows
- Linux

IDE:
We recommend using the following integrated development environments (IDEs) to work with this code:
- Keil5
- Visual Studio Code
- STM32CubeIDE

Prerequisites:
Before you begin, ensure you have met the following requirements:
1.Install the Development Environment:
Install an integrated development environment (IDE) suitable for STM32 development, such as STM32CubeIDE or Keil MDK. These IDEs facilitate code writing, compilation, and debugging for STM32 micro-controllers.
2.Install Device Drivers:
Install drivers for your STM32 debugger, whether it's an ST-Link or J-Link, to ensure proper communication between the computer and the target STM32 device.
3.Acquire the STM32 Firmware Library:
Depending on your project requirements, obtain or configure the appropriate version of the STM32 firmware library.Here, we are required to configure the STM32F40x.
4.Configure the IDE:
Configure the chosen IDE to match your hardware and project settings. Set the target device model, programmer options, and compiler paths within the IDE.

Usage:
Now, you can open the open-source code available on GitHub through the IDE and directly download it to the connected microcontroller. The code will execute automatically.








