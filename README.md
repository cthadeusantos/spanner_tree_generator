- [1. Preliminary Notes for 0.2.x series](#1-preliminary-notes-for-02x-series)
- [2. Important links and work performed](#2-important-links-and-work-performed)
- [3. Project Architecture](#3-project-architecture)
  - [3.1. Pre-requisites](#31-pre-requisites)
  - [3.2. External Dependencies](#32-external-dependencies)
  - [3.3. Instructions to build and execute this project](#33-instructions-to-build-and-execute-this-project)
  - [3.4. Usage](#34-usage)
    - [Usage example (running):](#usage-example-running)
    - [Usage example (running batteries):](#usage-example-running-batteries)
    - [Usage example (analisys batteries):](#usage-example-analisys-batteries)
  - [3.5. Directory Structure](#35-directory-structure)
  - [3.6. Internal Tools](#36-internal-tools)
- [4. Input file format](#4-input-file-format)
- [5. Changelog](#5-changelog)
- [6. Known Bugs](#6-known-bugs)
- [7. Authorship Information](#7-authorship-information)

# T-Admissibility Problem <!-- omit in toc -->

The $t$-admissibility is a min-max problem that concerns to determine whether a graph $G$ contains a spanning tree $T$ in which the distance between any two adjacent vertices of $G$ is at most $t$ in $T$. The stretch index of $G$, $\sigma(G)$, is the smallest $t$ for which $G$ is $t$-admissible.

This project is a collection of solutions for the T-admissibility problem. The base paper published by us is available [here](https://www.sciencedirect.com/science/article/pii/S0020019022000229?casa_token=pEzlk6qUuaMAAAAA:WvxZab2gsZnuOGo0nrXI_NUQXVHIke4LjcLzuJi0FOC0JFMYhsY8Jx0_6FsnXNWUq4ATu0kGSPXQ)). We maintain the code and the results attained by the group in this repository.

## 1. Preliminary Notes for 0.2.x series

Please note that the C++ code from a previous version (0.1.x series) will not be compatible with the current version. It is important to be aware that any code written for the previous version will require updates and modifications to ensure compatibility and proper functioning in the current version. Failing to update the code may result in errors, unexpected behavior, or even program crashes. Therefore, we strongly advise reviewing and revising your previous C++ code to ensure it aligns with the requirements and conventions of the current version.

Furthermore, please be aware that the code from the previous version will no longer receive updates or bug fixes. The previous version's codebase will be archived and stored at a designated location for reference purposes. It is important to transition to the current version and utilize its updated features, enhancements, and ongoing support for a more efficient and stable development environment.

<b>Important Notice:</b> Please be advised that in the future, the current version will become incompatible due to updates to the C++20 standard and the complete adoption of object-oriented programming throughout the project. This transition will be implemented to enhance the codebase and align with modern programming practices.

Previous version can be found here.

Link for [Release 0.1.7e](https://github.com/cthadeusantos/spanner_tree_generator/tree/0.1.7e)

Link for [Release 0.1.7d](https://github.com/cthadeusantos/spanner_tree_generator/tree/0.1.7d)

Please check Github Webpage for others branches

## 2. Important links and work performed

* Link to [this repository](https://github.com/cthadeusantos/spanner_tree_generator). 
* Related task [Kanban board](https://github.com/users/cthadeusantos/projects/1/views/1). (in portuguese)
    * Every task is represented by a card (issue). Any issue tagged as `EPIC` detail the current main goal of the project.
* Past published work:
  * [Base paper](https://www.sciencedirect.com/science/article/pii/S0020019022000229?casa_token=pEzlk6qUuaMAAAAA:WvxZab2gsZnuOGo0nrXI_NUQXVHIke4LjcLzuJi0FOC0JFMYhsY8Jx0_6FsnXNWUq4ATu0kGSPXQ) of the problem with the first solution set.

## 3. Project Architecture

This is a science project made primally to be ran in a computational environment without a graphical user interface (GUI). You should be able to build and execute this project under any linux distribution using just the terminal. Any script or application that does provide an user interface are *optional*, only designed for presentation purposes. The commands presented in this document assume that you use a Debian/Ubuntu distribution.

As a science project, each main application is built so it solves an instance in a single execution in a deterministic way. The idea is that each execution should be independent and done in a controlled environment, therefore the results are precise and replicable.

### 3.1. Pre-requisites 

The compiler used for every C/C++ component is [GCC](https://gcc.gnu.org/) at version 9.3.0. The build procedure is managed by [Make](https://www.gnu.org/software/make/). These are the only pre-requisites to build and execute everything. Use the following commands to install them:
  
```bash
sudo apt update
sudo apt install build-essential
``` 

The following bullets denote the pre-requisites to develop the project.

* The main text editor is [Visual Studio Code](https://code.visualstudio.com/) under Linux Mint 21.1  LTS codename "Vera" under Ubuntu Jammy package base or Windows 10 with [WSL2](https://docs.microsoft.com/en-us/windows/wsl/install) hosting Ubuntu 20.04 LTS.
* The configuration of the text editor is in the `.vscode` folder at the root of the repository. Anyone developing this project should install [WSL-Remote extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl) for VSCode. Optionally, the developer may use the following extensions to aid the developing process:
  * [C/C++ Microsoft extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools).
  * [Better C/C++ Syntax](https://marketplace.visualstudio.com/items?itemName=jeff-hykin.better-cpp-syntax).
  * [Markdown All in One](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one).
  * [C++ TestMate](https://marketplace.visualstudio.com/items?itemName=matepek.vscode-catch2-test-adapter).
  * [IntelliCode](https://marketplace.visualstudio.com/items?itemName=VisualStudioExptTeam.vscodeintellicode).
  * [Doxygen Document Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen).
  * [Python for VSCode](https://marketplace.visualstudio.com/items?itemName=ms-python.python).
* Any folder or file generated by your IDE, text editor or any personal tool that isn't VS code should be added to the `.gitignore` file in the root.
  * Use [this tool](https://www.toptal.com/developers/gitignore) if necessary.
* All python scripts are executed with Python3 (3.8.10 or above). Their dependencies are listed in the script file itself. You will need to install pip3 to obtain them (use virtual enviroment for install dependencies).
    * Pip3 itself: `sudo apt-get install python3-pip`
    * PrettyTable: `sudo pip3 install -U prettytable`
    * Matplotlib: `pip3 install matplotlib`
* This project uses [Doxygen](https://www.doxygen.nl/index.html) to document the source code. It is worth noting that VSCode has native support for doxygen. To install doxygen:
  * Quick install: `sudo apt-get install doxygen`
  * Doxygen may use graphviz, latex, dvips, and gs. You should install each one of these tools to build the documentation **correctly**.
    * Quick install: `sudo apt-get install graphviz texlive-full ghostscript-x`
* To ease the development of the project, the architecture comes with a bundled with a `launch.json` under VSCode to launch each application using [GDB](https://www.sourceware.org/gdb/). To use the debugger, you will need to install it:
  * `sudo apt-get install gdb`

### 3.2. External Dependencies

* LaTeX is the main tool to devise any scientific material to be published using this project. The primary platform to host such material is [Overleaf](https://www.overleaf.com/project). 

### 3.3. Instructions to build and execute this project

* The entire build setup is managed with [Make](https://www.gnu.org/software/make/) through a `makefile` in the root of this repository. 
* The build setup of each component is configured in the `makefile` at the root. To add new applications to the project, you may follow the standard of the file itself or ask for aid from another active developer if you have difficult using make.
* The following auxiliary commands are provided by the `makefile` in the root:
  * `make all` - Clean the binaries of the source code in release mode and compile them again.
  * `make build` or `make` - Build the source code in release mode.
  * `make run` - Run some basic examples with each source application in release setup.
  * `make debug` - Build the source code in debug mode.
  * `make doc` - Generate the source documentation.
  * `make clean-doc` - Clean the source documentation files.
  * `make clean` - Clean all temporary build folders.
* Additionally, you may build, run, and clean each component using the following commands:
  * `make build-X`, `make run-X`, `make clean-X` - Will build, run, or clean the component **X**, which may be `release` or `debug`.
* You may build, clean, and do other commands through the VSCode itself using the configured tasks (shortcut `ctrl + shift + b`). They will call some make rules from the makefile in the root.
* Each C/C++ component (like source, test, profile, microbenchmark) has a related `run.mk` designed to execute examples. They're not intended to be executed by themselves, instead they're called by the `makefile` in the root.
* Every `run.mk` file should provide rules and examples on how to execute each app. They are based on executions that were made through the project.
* Application usage instructions is documented in its main source file. Always check the respective `run.mk` for examples.
* Some instructions should be seen by passing `--help` in the commmand line to most applications. Also, usage may be reflected in some file at `documents/`. 
* The source code has two build setups:
  * `release` - Used to attain proper results to publish.
  * `debug` - Used through the development to test and catch bugs.
* In *release* mode, all source applications only use the `stdout` stream. Applications may generate some logging information in *debug mode* through `stderr` stream, this behaviour is controled in the `src/Debug.h` header.
* To debug the source code with VSCode and [GDB](https://www.sourceware.org/gdb/), select the *app* that you intend to debug in the upper part of the *Run and Debug* tab (shortcut `ctrl + shift + D`) and execute its rule with the shortcut `F5`. **The developer may need to alter input/output redirection in the `.vscode/launch.json` file**. The default configuration is set to use the `workspace/debug.in` file as input and output the stdout to `workspace/debug.out`. The stderr is printed to the console.
* There is a source code documentation availabe at `documents/source-doc.html`.
* To gather results to publish, each source application can be executed through the script `execution-battery.py` located in the tools folder. The script was designed to perform execution batteries while keeping the best solution found. Usage instructions are in the tool itself.


### 3.4. Usage

After compiling, you will find the executables in the **build/release/** directory. The brute force and heuristics applications can be identified as app_BF and app_HR, respectively.

The brute force executables are as follows:
* **app_BF-SEQ**, which runs sequentially using a single thread;
* **app_BF-EDGES**, which executes in parallel(multiple threads) using an edge list;
* **app_BF-ADJACENCY**, which operates in parallel(multiple threads) using an adjacency list;
* **app_BF-CYCLES**, which runs in parallel(multiple threads) utilizing an edge list and the induced cycle method.

```bash
app_name [OPTIONS] < INPUT_FILENAME [>> OUTPUT_FILENAME]
```

	OPTIONS:
      -h      | --help            This help
	          | --adjacency       Define which type file will be read. (adjacency list)[default]
	          | --edges           Define which type file will be read. (edges list)
              | --nolb            Not calculate lower bound
              | --noindex         Not calculate stretch index
	    -rt X | --running_time X  Define execution time in miliseconds until STOP! default is 0
	    -ci X | --induced_cycle X Define the number of induced cycles ! default is 1
	    -t X  | --thread X        Define the numbers of threads. X is the number of threads 
	    -v	  | --version         Which version is this app.


	  Show info:
	    -f | --file               Output at file.
	    -s | --screen             Output at screen.
	    -d | --debug              Output screen only debug mode.
	    -b | --best               Show the best tree found (default is not show). Try only with few vertices and edges.
	    You can combine file, screen and debug

    INPUT_FILENAME is mandatory
    OUTPUT_FILENAME is optional

#### Usage example (running):
	
```bash
./build/release/app_BF-SEQ -s < instances/grafos/grafo_10.txt > results/grafo10_results.txt
./build/release/app_BF-EDGES -t 32 -s < instances/grafos/grafo_10.txt >> results/grafo10_results.txt
```
#### Usage example (running batteries):
Go to tools directory and run

```bash
python3 execution-battery.py 1 < batteries/BATTERY-NAME.yaml
```
#### Usage example (analisys batteries):
After execute battery, go to tools directory and run
```bash
python3 execution-battery-analyzer.py ../workspace/BATTERY-NAME-DIRECTORY
```

### 3.5. Directory Structure

The project is structured as follows:

* `.vscode` -- Visual Studio Code common configuration files. You may find intellisense settings, recommended extensions, configurable tasks, and debugger configuration here.
* `src/` -- Contain the source code of the project. Detailed usage instruction, description, and run examples for each app can be found across the makefile, source code iteself, and in the source documentation.
  * `old/` -- Is the old source code used in the first version of this project.
  * `new/` -- The current source code being implemented for this version.
* `build/` -- Store temporary build files and any binary application generated by the compiler during code compilation.
* `tools/`  -- Internal tools used in this project. Usually, a script in python that helps visualizing instances or manipulation them. Their usage is documented inside the tool script itself. 
* `instances/` -- Contain the instance data set. You may find information about the instance format inside the `doc/` folder. It is recommended to keep each instance with unique name across the project. Internal tools or a main app itself may use these names to map solutions.
* `documents/` -- Contain some documentation about the project. You can find the instance specification format here for example. It is important to note that the documentation of each internal tool is located at the `/tools/` directory with the tool itself. Examples and intructions of any application or component usage should be in the main source code itself alongside the respective makefile.
  * `documents/doxygen/` - Constain documentation about the source code. 
  * `documents/figure/` - Any image or graphic generated specifically for the project.
  * `documents/related-papers/` - Some useful papers related to the project.
  * `documents/devised/` - Works and papers that has been done as a result of this project.
* `results/` -- Full result data and its respective output obtained through the execution of each main application that composes this project. This folder store any important execution data. See the related `results/Results.md` for a description of each folder contained there. 
  * `results/bks/` - The best known solution (bks) for each instance that was attained across every application test through this project. 
* `workspace/` -- A folder that isn't organized at all. Generally, these academic projects are managed by a few researchers (1 to 3). Most of the needs are unknown due to the nature the research project itself. Therefore, any data of unknown usefullness should be store here, like auxiliary files and temporary execution data. Any developer may alter what is inside, DO NOT backup stuff here.

### 3.6. Internal Tools

This project has internal tools to support development. Usage examples and detailed description should be inside the tool itself. The user is encouraged to read the entire description before using them. They are available at the `tools/` directory.

* `loader.py` - A module that is used by `execution-battery.py`, `execution-battery-analyser.py`, and `bks_summary.py`. Doesn't contain any script code that can run by itself. This class provides the code necessary to load the files generated for each problem related to this project (like the output and input files).
* `execution-battery.py` - This script will automize the execution of any main application through multiple independent runs with a list of instances to be solved given as input. It will maintain the problem best solution for each instance and inform if anything went wrong through.
* `execution-battery-analyzer.py` - Will analyse the log generated by the `execution-battery.py` to devise a result summary. It will iterate over each file generated in the previous script to create the final result table. The result is displayed in a way that can be easily manipulated for publishing.
* `bks_summary.py` - This script will update the summary file of the bks attained through the project summary at the results folder. It will create a file inside the `result/bks/` directory with tables that provide easy access to the bks information. 
* `list_files.py` - A simple script that receives a directory to output a list of files inside. The output will be printed at the console. May receive a filter by file extension.

## 4. Input file format

**For adjacency matrix** (a square matrix)

1st line Number of vertices (in our example 4)

2nd line 0 1 1 0

3rd line 1 0 1 1

4th line 1 1 0 1

5th line 0 1 1 0

 The adjacency matrix is a (0,1)-matrix with zeros on its diagonal.
 Please use space to separate (0,1) elements

**For edges list**

1st line Number of vertices

2nd line vertex **,** vertex

3rd line vertex **,** vertex

4th line vertex **,** vertex

..

Nth line vertex,vertex

An edge list is a data structure used to represent a graph as a list of its edges.
Each line from the 2nd represents an edge (two vertices).
Please use comma to separate the vertices.

Check the instance directory to examples.

For files with egdes list use the parameter --edges

## 5. Changelog

Link for [changelogs](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/CHANGELOG.md)

## 6. Known Bugs

Link for [known bugs webpage](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/KNOWNBUGS.md)

## 7. Authorship Information

We're a group of researchers mainly from Instituto de Computação/Universidade Federal Fluminense (IC/UFF). If you want to inquire about this project, you may e-mail any of the authors listed below.

* Luís Felipe Ignácio Cunha (lfignacio@ic.uff.br) **[Advisor]**
* Leandro Santiago de Araújo (leandro@ic.uff.br) **[Advisor]**
* Fernanda Couto (fernandavdc@ufrrj.br)
* Daniel Juvetude (deljuven@ymail.com)
* Carlos Thadeu (carlosthadeu@id.uff.br) 
* Anderson Zudio (azudio@id.uff.br)

This project is distributed with MIT license in the hope that it is usefull to anyone (see `LICENSE` at root). Although this project is distributed as free software, this fact do not isent it from being a scientific property. If this project aided your research, please do cite any referenced work from the authors above related in the first section of this file.
