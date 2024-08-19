# PPidSpoofer

`PPidSpoofer` is a Windows utility designed to spoof the parent process ID (PPID) of a newly created process. This tool can be useful for testing and debugging purposes where altering the parent process ID is required.

## Features

- **Spoof PPID**: Modify the parent process ID of a new process.
- **Target Process**: Default target is `svchost.exe` but can be specified by the user.

## Requirements

- Windows OS
- Visual Studio or equivalent for compiling C++ code
- Administrative privileges may be required for process manipulation

## Compilation

Compile the code using a C++ compiler that supports Windows API and standard libraries.

## Usage

Run the executable from the command line with the following options:

```sh
PPidSpoofer.exe [processname|Default]
```

### Parameters
processname - The name of the process you want to spoof the PPID for. Example: notepad.exe.
Default - Spoof the PPID of a process named svchost.exe.

### Examples
Spoof PPID for svchost.exe:
``sh
PPidSpoofer.exe Default
``
Spoof PPID for a custom process:
``sh
PPidSpoofer.exe lsass.exe
``

## Code Explanation
1.GetTarget: Finds the process ID (PID) of the specified process name.
2/PPiDSpoofing: Creates a new process with a spoofed PPID using the specified PID.
3.wmain: Entry point of the application. Parses command line arguments and invokes the spoofing functionality.

### Error Handling
If the target process cannot be found or opened, appropriate error messages are displayed.
If process creation fails, the error details are provided.

## License
This tool is provided for educational and research purposes. Use it responsibly and ensure compliance with applicable laws and regulations.

##Disclaimer
This utility is provided "as-is" without warranty of any kind. The developers are not responsible for any damages caused by the use of this tool.
