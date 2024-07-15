# Overview

This project is an assembler and emulator for the VC8000 computer, a fictional computer inspired by 1960s technology, specifically the IBM 1620. The assembler translates assembly language programs into machine language, while the emulator executes the programs as if they were running on the VC800 itself.

# What I Learned
This project was one of my first decently sized projects. I learned alot about the actual process of designing a large project and the mindful/logical skills needed to complete one. Apart from the soft skills, this project helped me master the debugging/analyzation process of coding due to its size.

# Features

* Assembler: Converts VC8000 assembly language instructions into machine code.
* Emulator: Simulates the execution of machine code on the VC8000.
* Error Reporting: Error handling during the assembly time.

# System Requirements
* Microsoft Visual Studio

# Installation
* Clone the repository to your local machine
* Open the solution file in VS
* Build the solution by navigating to Build -> Build Solution

# Usage
To run the assembler and emulator:

* Put your assembly language file in the designated input directory.
* Run the program thru VS. The output will be the machine code and the emulation results.

# Typical Output
The expected output is a machine language translation of the input assembly code. In case of errors, the output will detail the issues without halting the translation process.

# Project Structure
* File Handling: Manages reading and writing files.
* Parser: Breaks down input lines into labels, op codes, and operands.
* Symbol Table: Handles storage and retrieval of labels and their corresponding addresses.
* Code Generation: Converts parsed instructions into machine code.
* Error Management: Reports and logs errors encountered during the assembly process.
