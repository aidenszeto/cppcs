# The C++ Command Shell
The C++ Command Shell (CPPCS) is a light-weight command line interface built for C++ unit testing and scripting. It provides streamlined C++ file importing (both header and source) to improve the testing of large, chained codebases. For detailed information, feel free to reach out [here](mailto:aidenszeto@g.ucla.edu).

*_Note: In order to use CPPCS, you must have an adequate distribution g++ installed. For more information, see [Installing g++ on Windows](https://www3.cs.stonybrook.edu/~alee/g++/g++.html)_*

![cppcs](https://user-images.githubusercontent.com/66970460/125360043-1b374e00-e320-11eb-87c5-78c4853d912a.png)

## Installation
The CPPCS executable can be downloaded [here](https://github.com/aidenszeto/cppcs/raw/master/cppcs.exe). This executable contains the full command line interface and is ready for use immediately upon download. Alternatively, you can navigate to the root directory of this folder and download `cppcs.exe`.

If you would like to comntribute to CPPCS, you may clone/download this repository and make changes to the code.

## Getting Started
The following commands are supported on CPPCS:
- **CTRL-L**: Import a file into the CLI. This will allow you to utilize code (classes, functions, etc) from the imported file when scripting.
- **CTRL-R**: Reset the CLI. This clears all commands previously called during the session and resets the editor window.
- **CTRL-X**: Exits the CLI. The commands from the most recent session will be available in `cache.cpp`, which is located in the same directory as the executable.
- **CTRL-Z**: Undos the most recent change. The undo stack is cleared every command, so previous commands cannot be undone. To undo a command, reset the CLI.
- **Arrow Keys + Home/End**: Navigate around the CLI. The keys will exhibit the typical behavior of a command line interface.

## Adding CPPCS to Path
If you would like to make CPPCS accessible from any terminal location, please add the folder containing the downloaded `cppcs.exe` executable to your PATH. For more information on how to add environment variables to your Windows PATH, see [this tutorial](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/). Once CPPCS is added to your path, running `cppcs` on any command line should bring up The C++ Command Shell.
