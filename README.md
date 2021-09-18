# QUno

An Uno-like card game, implemented like it's 1995.

## Requirements

* Windows
* [Windows SDK](https://developer.microsoft.com/en-US/windows/downloads/windows-10-sdk/)
* Your favorite editor (my favorite editor is [Visual Studio Code](https://code.visualstudio.com/))
* [Debugging Tools for Windows](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/)

## How To Play

Create a shortcut to `QUNOCLASSIC.EXE` on your desktop, double-click it, then use 
your mouse or keyboard to navigate the menus and dialog boxes.

## Developer Notes

This repository includes a nostalgic implementation of QUno in (mostly) C using 
only what is available in the Windows SDK. It was written in a text editor, 
built on the command line, and debugged line-by-line in the way programs were written 
before IDEs and WWWs.

* `QUnoLib` contains the game engine. This is a straightforward Windows dynamic link library 
with a C-style interface. Include `QUnoLib.h` and link to `QUnoLib.lib` and make sure 
`QUnoLib.dll` is in the path of the calling executable.
* `QUnoClassic` contains a Win32-style desktop application implemented using C that 
follows classic Petzold-style techniques.
