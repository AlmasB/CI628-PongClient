# CI628-PongClient
A simple multiplayer pong client in C++.

### Build

The project is configured to be developed on Windows, so it only includes Windows-specific SDL versions.

To build, you will need:

* [CMake](https://cmake.org/) 3.6+
* Visual Studio 2019

1. Download this repository by clicking "Clone or download" -> Download zip.
2. Extract the zip file and rename `CI628-PongClient-master` to `CI628`.

Open the command line (via Git Bash):

```
cd CI628
mkdir build
cd build

// Note: if running on Uni machines, it's /c/Program\ Files/CMake/bin/cmake rather than just cmake

cmake .. -G "Visual Studio 16 2019"

```

After this, a ".sln" will be generated in `build/`.

1. Open the ".sln" file with Visual Studio 2019.
2. Right click on MyGame in project explorer and select as startup project.
3. Right click on MyGame again and select Properties.
4. In the properties window, Linker -> System and change SubSystem to Console.
5. Copy the `res/` directory to the `build` directory.

Before running the demo, ensure that the [CI628-server application](https://github.com/AlmasB/CI628-PongServer) is running. You can now run the demo from Visual Studio via Local Windows Debugger.