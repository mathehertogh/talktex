# Meson
Meson is a modern, speedy, platform-independent, incremental compiler.
In Meson, we use a python-like syntax to define sources, dependencies, generators etc etc.
In this Section, we explain the basics of using Meson.

>Note: For these instructions we took inspiration from the Compiler Construction project by Sebastiaan Alvarez Rodriguez and Mariska IJpelaar


## Installing Meson
Installing Meson is simple, and requires no root/sudo/su privileges.
See [mesonbuild.com](https://mesonbuild.com/SimpleStart.html#installing-meson) for installation instructions for all popular platforms.
Below, we provide information on installing Meson on Linux machines.


### Installing Meson - Linux
The easiest way to install Meson on Linux is by using pip:
```bash
pip3 install meson ninja --user
```
> **Note**: The `--user` flag installs given packages in user-mode, meaning we do not need superuser privileges.
> **Note**: we do not recommend installing using `apt-get`, as this installs a different version without the `--reconfigure` flag

After installation, the required executables (called `meson` and `ninja`) exist in `~/.local/bin`.
> **Note**: Don't forget to add `~/.local/bin` to your `$PATH` environment variable.
> By doing that, you can execute simply `meson <command>` and `ninja <command>` from your terminal.



## Instantiating Meson
We instantiate this project (only required once) by executing:
```bash
mkdir build
cd build
meson ..
```
Meson initializes itself, generates build files, executables, libraries etc in the `build/` dir.

> **Note**: At this point we expect you to have `flex` and `bison` installed


## Building with Meson
To compile this framework, execute (from `build/`):
```bash
meson
ninja
```

`meson` generates the compilation files with the default configuration options.
All configuration options are available in [`meson-options.txt`](/meson_options.txt).
Print the current configuration using:
```bash
meson configure
```

**After initial configuration**, every time you wish to change compilation options, you should regenerate the compilation files using:
```bash
meson -Doption=value... --reconfigure
```
After (re)building the compilation files, (re)compile using `ninja`.

## Executable and Library Locations
Compiled executables and libraries can be found in `build/`. The exact locations for each binary:
* grammar:
   - `build/src/grammar/compiler_grammar`
   - `build/src/grammar/libgrammar.so`
* latex-generator:
   - `build/src/latex-generator/compiler_latex_generator`
   - `build/src/latex-generator/liblatexgenerator.so`

> **Note**: generation of these executables is currently not activated, only the libraries are compiled

Of course, you have to compile these modules to make the listed binaries available.

## Cleaning
To clean up object files etcetera, use:
```bash
ninja clean
```

## IDE Integration
Meson, being the 'new cool' player in the field, is often not supported by IDE's,
which only support older, more established compilation systems such as Cmake and Make.

This is a problem easily solved.
After configuring `meson` and compiling once using `ninja`, there is a file called `build/compile_commands.json`.
Many popular IDEs (IntelIJ Clion, VSCode etc) all can open this file as a project.
When doing this, the IDE will understand the Meson project and behave normally.
There are several tricks to make it work even better!

Check out:
1. [Clion](https://blog.jetbrains.com/clion/2021/01/working-with-meson-in-clion-using-compilation-db/)
2. [Meson with Visual Studio and XCode](https://mesonbuild.com/IDE-integration.html)
3. [Meson plugin for Visual Studio](https://marketplace.visualstudio.com/items?itemName=asabil.meson)

Out of these links, we only tested and extensively used the first one