Chip8-Disassembler
==================

Translate a chip8 rom to semi-asm.

How can I test it ?
===================

Through the next commands, you will be able to test the disassembler:

    $ make

    $ ./Chip8-Disassembler roms/rom

A line is composed as follow: 0xf229|       set v2, I

The first column : 0xf229 corresponds to the hexadecimal opcode. The registers
are represented with the character 'v' (for instance : v2 is the second
register). 'I' is the pointer onto the memory, DT the DelayTimer and ST the
SoundTimer.

