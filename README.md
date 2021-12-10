![TalkTeX](/images/logo-white-on-black.png?raw=true)

# Purpose

TalkTex is a tool for converting speech to Latex.

# Status

Our first version is available for usage

# Installation and Usage

## Setup 
If you have Ubuntu with root privileges then you can use the [ubuntu-setup](setup-ubuntu.sh) script to perform the setups.
```shell
./setup-ubuntu.sh
```
Otherwise, you need to find a way to manually install the packages from the [ubuntu-setup](setup-ubuntu.sh) and run [common-setup](setup-common.sh)
```shell
./setup-common.sh
```

## Build
Run the following command:
```shell
./make-all.sh
``` 

## Setup on university computers
To set up and compile TalkTex for the Leiden University computers in the computer labs, run:
```shell 
./setup-302_306.sh
```
This will install the required Ubuntu packages and Python modules (that are not already installed) in user-mode. This script also performs compilation, so after running it you are ready to start using TalkTex.  

## Usage
For the default parameters, simply run the generated `talktex` command (a symlink to the actual executable). 

# Meson
We use Meson for our build system. If you want to read about its usage, you may read our [Meson-manual](src/compiler/MESON.md) for more information.

# User guide

The [user guide](docs/guide.md) explains how a user should talk to TalkTex in
order to obtain certain Latex output. The full TalkTex grammar can be found
[here](docs/grammar.md).
