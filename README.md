- [1. Preliminary Notes for 0.2.x series and higher](#1-preliminary-notes-for-02x-series-and-higher)
    - [New versioning system](#new-versioning-system)
- [2. Important links and work performed](#2-important-links-and-work-performed)
  - [2.1. Understanding the problem of t\_admissibility](#21-understanding-the-problem-of-t_admissibility)
  - [2.2. The Errata section](#22-the-errata-section)
  - [2.3. Download the code](#23-download-the-code)
  - [2.4. Download binaries](#24-download-binaries)
- [3. Project Architecture](#3-project-architecture)
  - [3.1. Pre-requisites](#31-pre-requisites)
    - [3.1.1. Instructions for Linux and WSL](#311-instructions-for-linux-and-wsl)
  - [3.2. External Dependencies](#32-external-dependencies)
  - [3.3. Instructions to build and execute this project](#33-instructions-to-build-and-execute-this-project)
  - [3.4. Usage](#34-usage)
    - [The tools:](#the-tools)
    - [How to run (with options):](#how-to-run-with-options)
    - [USAGE EXAMPLES (running):](#usage-examples-running)
    - [Usage example (Calculate the stretch factor using the sequential app with output at screen):](#usage-example-calculate-the-stretch-factor-using-the-sequential-app-with-output-at-screen)
    - [Usage example (running sequential app with output at screen and save the graph and best tree solution to yEd file):](#usage-example-running-sequential-app-with-output-at-screen-and-save-the-graph-and-best-tree-solution-to-yed-file)
    - [Usage example (Calculate the stretch factor using the edges app using 32 threads and with output at screen):](#usage-example-calculate-the-stretch-factor-using-the-edges-app-using-32-threads-and-with-output-at-screen)
    - [Usage example (Given a tree, calculate the stretch factor from its graph):](#usage-example-given-a-tree-calculate-the-stretch-factor-from-its-graph)
    - [Usage example (running batteries):](#usage-example-running-batteries)
    - [Usage example (analisys batteries):](#usage-example-analisys-batteries)
  - [3.5. Directory Structure](#35-directory-structure)
  - [3.6. Internal Tools](#36-internal-tools)
  - [3.7. For EXPERIMENTAL Series 1 (Linux and MacOS)](#37-for-experimental-series-1-linux-and-macos)
    - [Setting up BOOST:](#setting-up-boost)
- [4. Input file format](#4-input-file-format)
- [5. MacOS User Information: Key Details and Guidelines](#5-macos-user-information-key-details-and-guidelines)
  - [5.1. MacOS Compilation Instructions](#51-macos-compilation-instructions)
  - [5.2. Instructions for Running on MacOS](#52-instructions-for-running-on-macos)
- [6. Changelog](#6-changelog)
- [7. Known Bugs](#7-known-bugs)
- [8. To do](#8-to-do)
- [9. Authorship Information](#9-authorship-information)
- [APPENDIX](#appendix)
  - [Some stretch index values - tables](#some-stretch-index-values---tables)
  - [Some stretch index values - result\_summary.txt](#some-stretch-index-values---result_summarytxt)

# T-Admissibility Problem <!-- omit in toc -->

The $t$-admissibility is a min-max problem that concerns to determine whether a graph $G$ contains a spanning tree $T$ in which the distance between any two adjacent vertices of $G$ is at most $t$ in $T$. The stretch index of $G$, $\sigma(G)$, is the smallest $t$ for which $G$ is $t$-admissible.

This project is a collection of solutions for the T-admissibility problem. The base paper published by us is available [here](https://www.sciencedirect.com/science/article/pii/S0020019022000229?casa_token=pEzlk6qUuaMAAAAA:WvxZab2gsZnuOGo0nrXI_NUQXVHIke4LjcLzuJi0FOC0JFMYhsY8Jx0_6FsnXNWUq4ATu0kGSPXQ). We maintain the code and the results attained by the group in this repository.

## 1. Preliminary Notes for 0.2.x series and higher

Please note that the C++ code from a previous version (0.1.x series) will not be compatible with the current version. It is important to be aware that any code written for the previous version will require updates and modifications to ensure compatibility and proper functioning in the current version. Failing to update the code may result in errors, unexpected behavior, or even program crashes. Therefore, we strongly advise reviewing and revising your previous C++ code to ensure it aligns with the requirements and conventions of the current version.

Furthermore, please be aware that the code from the previous version will no longer receive updates or bug fixes. The previous version's codebase will be archived and stored at a designated location for reference purposes. It is important to transition to the current version and utilize its updated features, enhancements, and ongoing support for a more efficient and stable development environment.

> [!NOTE]
> 1. We are pleased to announce that, starting from version 0.2.7, the code is now available for compilation and alpha-stage execution on the MacOS operating system. We are committed to expanding compatibility and accessibility of our software, enabling MacOS users to also enjoy the latest features and functionalities. We encourage users interested in this platform to refer to the specific section for [MacOS](#5-macos-user-information-key-details-and-guidelines) in the README.md for detailed information on requirements, configurations, and potential considerations while running the software in this environment. We appreciate all users and look forward to providing an optimized experience for everyone.
> 2. In March 2024, we will begin the process of refactoring and migrating the current codebase to C++23. Expect many changes; you can track them in the series1 branch, and the use of the Boost library will be mandatory..
> 3. In April 2024, we aim to test the code from series 0.2.X on the latest version of GhostBSD, expanding support to more operating systems.
> 4. In May 2024, we plan to test the code from series 0.2.X on Windows 11.

> [!WARNING]
> <b>Important Notice:</b> Please be advised that in the future, the current version will become incompatible due to updates to the C++23 standard and the complete adoption of object-oriented programming throughout the project. This transition will be implemented to enhance the codebase and align with modern programming practices.


#### New versioning system

From version 0.2.4 onwards, we are adopting a new versioning system. We have discontinued the use of letters alongside the third digit to represent bug fixes.

| Digit | The new numbering system | Regarding digit increments /  In terms of compatibility |
| -------- | --- | --- | 
| #1 | Represents a major or significant version | Indicate a major update that introduces significant changes or may not be compatible with previous versions, be aware of this |
| #2 | Represents a minor version or additional functionality. | Indicate the addition of new features or improvements while maintaining compatibility with previous versions |
| #3 | Represents bug fixes, minor improvements or adjustments | Represents bug fixes, minor improvements, or adjustments, used for maintenance releases that do not alter functionality but address issues. |

## 2. Important links and work performed

* Link to [this repository](https://github.com/cthadeusantos/spanner_tree_generator). 
* Related task [Kanban board](https://github.com/users/cthadeusantos/projects/1/views/1). (in portuguese)
    * Every task is represented by a card (issue). Any issue tagged as `EPIC` detail the current main goal of the project.

### 2.1. Understanding the problem of t_admissibility

* Past published work:
  * [2022 pre-print](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/documents/devised/IPL_HeuristicsADM_V3%20(2).pdf)
  * [Strategies for generating tree spanners: Algorithms, heuristics and optimal graph classes / 2022](https://www.sciencedirect.com/science/article/pii/S0020019022000229?casa_token=pEzlk6qUuaMAAAAA:WvxZab2gsZnuOGo0nrXI_NUQXVHIke4LjcLzuJi0FOC0JFMYhsY8Jx0_6FsnXNWUq4ATu0kGSPXQ) Paper published in Information Processing Letters.
  * [The paper presented at the Brazilian Operational Research Symposium (SBPO) 2023](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/documents/devised/galoa-proceedings--sbpo-2023--174829.pdf) with a new set of heuristics and the use of centrality measures. If you prefer to download directly from the SBPO Website, [click here (ENGLISH)](https://proceedings.science/sbpo/sbpo-2023/trabalhos/heuristics-for-t-admissibility-with-complex-network-approach?lang=pt-br).
  * If you wish to visually understand heuristics 1, 2, 3 & 4, please refer to the PowerPoint presented at the Brazilian Operational Research Symposium 2023. [ENGLISH](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/documents/devised/Presentation-SBPO2023v5-ENG.pdf) or [PORTUGUESE](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/documents/devised/Apresentacao-SBPO2023v5-PT_BR.pdf)
  * [New parallelism and heuristic approaches for generating tree t-spanners in graphs / 2024](http://dx.doi.org/10.1002/cpe.8106) - Paper published in Concurrency and Computation: Practice and Experience Journal
  * Coming soon, link to download Carlos Thadeu's Master’s Thesis (PORTUGUESE) from IC/UFF repository.

### 2.2. The Errata section

Please note the following correction:

<b>In 2022 Base paper (SBPO):</b>

* At page 2, Algorithm 1, line 16, where it reads '_last_neighbor[i] = -1_', please read as '_last_neighbor[v] = -1_'
* At page 4, example 3, where it reads '_V=[4,7,1,3,0,2,5,6]_', please read '_V=[4,7,1,3,8,0,2,5,6]_'


<b>In 2023 paper (SBPO):</b>

* At page 6, Equation 7, where it reads '_f(v) = d<sub>G</sub>(v)-d<sub>T</sub>(v)_', please read as '_f(v) = d<sub>G</sub>(v)-A<sub>tree</sub>(v)_'

We apologize for any confusion and appreciate your understanding.

### 2.3. Download the code

[Last stable version](https://github.com/cthadeusantos/spanner_tree_generator/releases/latest)

Previous version can be found here.
Link for [Release 0.1.7e](https://github.com/cthadeusantos/spanner_tree_generator/tree/0.1.7e) (many bugs & last version for 0.1.x series) 

Please check our Github Webpage for others branches.

### 2.4. Download binaries

[Binaries for Linux](https://github.com/cthadeusantos/spanner_tree_generator/tree/main/binaries/linux) (v0.2.12 not work in WSL - W11, you need compile)
[Binaries for MacOS](https://github.com/cthadeusantos/spanner_tree_generator/tree/main/binaries/mac)

## 3. Project Architecture

This is a science project made primally to be ran in a computational environment without a graphical user interface (GUI). You should be able to build and execute this project under any linux distribution using just the terminal. Any script or application that does provide an user interface are *optional*, only designed for presentation purposes. The commands presented in this document assume that you use a Debian/Ubuntu distribution.

As a science project, each main application is built so it solves an instance in a single execution in a deterministic way. The idea is that each execution should be independent and done in a controlled environment, therefore the results are precise and replicable.

### 3.1. Pre-requisites 

#### 3.1.1. Instructions for Linux and WSL

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

| Label | Executable | Brute force description |
| ----- | -------- | --- |
| SEQUENTIAL | **app_BF-SEQ** | Runs sequentially using a single thread |
| EDGES | **app_BF-EDGES** | Executes in parallel(multiple threads) using an edge list method |
| ADJACENCY | **app_BF-ADJACENCY** | Operates in parallel(multiple threads) using an adjacency list method |
| CYCLES | **app_BF-CYCLES** | Runs in parallel(multiple threads) utilizing an adapted edge list method and the new induced cycle method |
	
The heuristics executables are as follows:

| Label | Executable | Description |
| ----- | -------- | --- |
|**H1v1**<br>Global MaxDegree Heuristic v1<br>**Heuristic 1**  |   **app_HR-H1v1**       |  Global MaxDegree Heuristic comes in two versions, namely **v1** (sorted by degree centrality, which was proposed by~\cite{COUTO2022106265}) and **v2** (sorted by degree and closeness centrality, proposed in this work); **v3** and **v4**, description coming soon   |
|**H1v2**<br>Global MaxDegree Heuristic v2<br>**Greedy Coverage Heuristic plus**   |   **app_HR-H1v2**       |  Global MaxDegree Heuristic comes in two versions, namely **v1** (sorted by degree centrality, which was proposed by~\cite{COUTO2022106265}) and **v2** (sorted by degree and closeness centrality, proposed in this work); **v3** and **v4**, description coming soon   |
|**H1v3**<br>Tiebreaker Free Greedy Coverage Heuristic   |   **app_HR-H1v3**       |   Global MaxDegree Heuristic comes in two versions, namely **v1** (sorted by degree centrality, which was proposed by~\cite{COUTO2022106265}) and **v2** (sorted by degree and closeness centrality, proposed in this work); **v3** and **v4**, description coming soon  |
|**H1v4**<br>Greedy Edge Common Coverage Heuristic   |   **app_HR-H1v4**       |  Global MaxDegree Heuristic comes in two versions, namely **v1** (sorted by degree centrality, which was proposed by~\cite{COUTO2022106265}) and **v2** (sorted by degree and closeness centrality, proposed in this work); **v3** and **v4**, description coming soon   |
|**H2v1**<br>Local MaxDegree Heuristic v1<br>**Heuristic 2**   |   **app_HR-H2v1**       |   Local MaxDegree Heuristic, the vertices of the graph $G$ are sorted in descending order based on their degree centrality (**v1**) or closeness centrality (**v2**)  |
|**H2v2**<br>Local MaxDegree Heuristic v2<br>**Local Centrality-Driven Growth Heuristic**   |   **app_HR-H2v2**       |  Local MaxDegree Heuristic, the vertices of the graph $G$ are sorted in descending order based on their degree centrality (**v1**) or closeness centrality (**v2**)  |
|**H3v1**<br>Adaptative Global MaxDegree Heuristic v1<br>**Degree Centrality-Driven Hybrid Coverage Heuristic**   |   **app_HR-H3v1**       |   Adaptive Global MaxDegree Heuristic is a combination of Global MaxDegree Heuristic and Local MaxDegree Heuristic. In the initial version (**v1**) of Adaptative Global MaxDegree Heuristic, the vertices are sorted based on their degree centrality  |
|**H3v2**<br>Adaptative Global MaxDegree Heuristic v2<br>**Closeness Centrality-Driven Hybrid Coverage Heuristic**   |   **app_HR-H3v2**       |   Adaptative Global MaxDegree Heuristic **v2** is a slight modification of **v1**. In this version, when vertices has same degree, we consider additional measures such as closeness centrality and leverage centrality to determine the most appropriate vertex for selection  |
|**H4v1**<br>Centrality Heuristic MaxDegree<br>**Higher-Degree Centrality Heuristic**   |   **app_HR-H4v1**       |  That is the Centrality Heuristic MaxDegree than utilizes the concept of degree centrality to select the vertex root. The vertex with the highest degree is chosen as the root, but in cases where multiple vertices have the same degree, ties are broken using a combination of closeness centrality and leverage centrality. Subsequently, the neighbors are sorted based on their degree centrality, enabling a systematic analysis of the network structure   |
|**H4v2r1**<br>Traveller Centrality Heuristic<br>**Traveler Centrality Heuristic**   |   **app_HR-H4v2r1**       |  The Traveller Centrality Heuristic demonstrates higher accuracy compared to Centrality Heuristic MaxDegree and Algebraic Centrality Heuristic, but at the cost of slower performance. This can be attributed to the calculation of closeness centrality, which necessitates traversing all vertices to determine the shortest path between a given vertex and all others   |
| **H4v2r3**<br>Algebraic Centrality Heuristic<br>**Speedy Algebraic Centrality Heuristic** |   **app_HR-H4v2r3**   | The algorithm for Algebraic Centrality Heuristic is essentially the same as Traveller Centrality Heuristic. While the accuracy of Algebraic Centrality Heuristic is slightly lower than that of Traveller Centrality Heuristic, it exhibits higher speed due to the adoption of an approximation method proposed by Evans_2022. Instead of traversing all vertices, we employ equations to estimate closeness centrality |

#### The tools:

The tools executables are as follows:

| Label | Executable | Brute force description |
| ----- | -------- | --- |
| FACTOR-CALCULATOR | **app_FACTOR-CALCULATOR** | Calculates the stretch factor of a spanning tree T in a graph G |
| CREATE            | **app_CREATE**            | Without description yet                                         |
| GENERATE          | **app_GENERATE**          | Without description yet                                         |

<ins>app_FACTOR-CALCULATOR</ins>

Sometimes, we need to compute the stretch factor of a spanning tree T in a graph G. To address this, we developed a new tool called app_FACTOR-CALCULATOR to compute the stretch factor for a single tree.

The tree file can be of the adjacency matrix or edge list type as described in section 4. The software will automatically detect the tree data structure type. (valid only for the tree file)

<ins>app_CREATE</ins>

Description too soon

<ins>app_GENERATE</ins>

Description too soon

<ins>Others notes</ins>

Since version 0.3.0, you can input a tree and a graph and then calculate the stretch factor of this tree (see app_FACTOR-CALCULATOR).

Also, since version 0.3.0, it is possible to generate an XML file representing the graph and the tree with the best stretch index (or the stretch factor if using app_FACTOR_CALCULATOR), which can be read with the yED software, avaliable [here](https://www.yworks.com/products/yed).


#### How to run (with options):

```bash
app_name [OPTIONS] < INPUT_FILENAME [>> OUTPUT_FILENAME]
```



	OPTIONS:
      -h  | --help              Show this help
      -v  | --version           Provides the version

    Running time:
      -rt X | --running_time X  Defines the execution time in miliseconds until STOP! default is 0
  
    Read file (brute force and heuristics):
	    --adjacency               Defines which type file will be read. (adjacency matrix)[default]
	    --edges                   Defines which type file will be read. (edges list)
    
    Read file (ONLY app_FACTOR-CALCULATOR):
      --ftree [PATH/FILE]       Defines the path and file name of the tree to be calculated.

    Save graph to XML file
      --yed                     Save the graph and your tree at XML file to be read from yEd software (https://www.yworks.com/products/yed)

    Calculate:
      --nolb                    Not calculate lower bound
      --noindex                 Not calculate stretch index
	    -ci X | --induced_cycle X Defines the number of induced cycles ! default is 1
	
    Threads:
      -t X  | --thread X        Defines the numbers of threads. X is the number of threads

    Method for calculating closeness: (NOT USED YET!)
      --alg                     Calculate closeness using algebraic method, used only in heuristics. [DEFAULT]
      --tra                     Calculate closeness using traverse method, used only in heuristics.

	  Show info:
	    -f | --file               Output at file.
	    -s | --screen             Output at screen.
	    -d | --debug              Output screen only debug mode.
	    -b | --best               Show the best tree found (default is not show). Try only with few vertices and edges.
	    You can combine file, screen and debug

    INPUT_FILENAME is mandatory
    OUTPUT_FILENAME is optional

#### USAGE EXAMPLES (running):

#### Usage example (Calculate the stretch factor using the sequential app with output at screen):	
```bash
./build/release/app_BF-SEQ -s < instances/grafos/grafo_10.txt > results/grafo10_results.txt
```
#### Usage example (running sequential app with output at screen and save the graph and best tree solution to yEd file):
```bash
./build/release/app_BF-SEQ -s --yed < instances/grafos/grafo_10.txt > results/grafo10_results.txt
```
#### Usage example (Calculate the stretch factor using the edges app using 32 threads and with output at screen):
```bash
./build/release/app_BF-EDGES -t 32 -s < instances/grafos/grafo_10.txt >> results/grafo10_results.txt
```
#### Usage example (Given a tree, calculate the stretch factor from its graph):
```bash
./build/release/app_FACTOR-CALCULATOR --ftree "mypath/mytree.txt" < mygraph.txt
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
  * `series1/` -- This directory contains the source code in development for the future version 1 of the Spanner Tree Generator.
* `build/` -- Store temporary build files and any binary application generated by the compiler during code compilation.
* `tools/`  -- Internal tools used in this project. Usually, a script in python that helps visualizing instances or manipulation them. Their usage is documented inside the tool script itself.
* `tools/analisys/`  -- Don't worry, you don't need this directory; it's only used to store the developer's personal tools.
* `tools/my_execute_bash/`  -- Don't worry, you don't need this directory; it's only used to store the developer's personal tools.
* `tools/my_scripts/`  -- Don't worry, you don't need this directory; it's only used to store the developer's personal tools.
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

### 3.7. For EXPERIMENTAL Series 1 (Linux and MacOS)


> [!NOTE]
> Disclaimer:
> 1. During Series 1 development, please go to the series1 branch
> 2. series1 will not be available in the main branch during development.

For the upcoming version 1, we will be incorporating libraries from the Boost.org project into our project. If you want to learn more about the [Boost.org project, click here](https://www.boost.org).

#### Setting up BOOST:

1. [Download the BOOST library](https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/) from Boost.org (the version used in the project is 1.84.0). 
2. Unzip the boost_X_XX_X.tar.gz file (used in the project, boost_1_84_0.tar.gz).
3. Move the unpacked file to /usr/local using the command:
```bash
sudo mv boost_1_84_0 /usr/local/
```
4. Go to /usr/local directory and execute the command:
```bash
sudo chown -R root:wheel boost_1_84_0
```
5. Edit the .bashrc file (Linux or OSX <= Catalina) or .zshrc file (MacOS >= BigSur) and insert the following lines (the last line is the most important of all):
```bash
# BOOST libraries settings
export BOOST_ROOT="/usr/local/boost_1_84_0"
export BOOST_VERSION=1.84.0
export CPLUS_INCLUDE_PATH="/usr/local/boost_1_84_0"
```
6. If you are using VSCODE, go to the .vscode directory at the root of the project and check if in the c_cpp_properties.json file there is "env" variable that contains the following line, "/usr/local/boost_1_84_0/**". If it does not exist, add it, and you should have something like this:
```bash
"env": {
    "myDefaultIncludePath": [
        "${workspaceFolder}/src/**",
        "${workspaceFolder}/src/series1/**",
        "/usr/local/boost_1_84_0/**"
    ]
}
```

## 4. Input file format

**For adjacency matrix** (a square matrix)

Put at 1st line the Number of vertices (in our example 4)
```
4
0 1 1 0
1 0 1 1
1 1 0 1
0 1 1 0
```

 The adjacency matrix is a (0,1)-matrix with zeros on its diagonal.
 Please use space to separate (0,1) elements

[Other adjacencies' list example](instances/examples/G4_FULL_edges.in)

**For edges list**

1st line are the number of vertices in the graph (in our example N).
Each line from the 2nd line represents an edge (two vertices). Please use comma to separate the vertices.

```
N
vertex , vertex
vertex , vertex
vertex , vertex
vertex , vertex
...
.
.
vertex , vertex
```
An edges' list is a data structure used to represent a graph as a list of its edges.



Please, check the instances directory to references and examples.
For files with egdes list use the parameter --edges

[Edges' list example](instances/examples/G9_E10_edges.in)

## 5. MacOS User Information: Key Details and Guidelines

### 5.1. MacOS Compilation Instructions

Please note that compiling the code for MacOS differs slightly from Linux and WSL. The compilation and execution are currently in alpha stage.

To compile, run the following command in the terminal from the project's root directory:

``````
make -f makefile_OSX [OPTION]
``````

Where OPTION can be one of the following:

* `clean` - Deletes files in the ./build/release directory.

* `sequential` - Creates the executable that uses the sequential brute-force method.

* `adjacency` - Creates the parallel executable that uses the brute-force adjacency list method.

* `cycles` - Creates the parallel executable that uses the brute-force induced cycles method.

* `edges` - Creates the parallel executable that uses the brute-force edge list method.

* `H1v1`, `H1v2`, `H2v1`, `H2v2`, `H3v1`, `H3v2`, `H4v1`, `H4v2r1`, `H4v2r2` - Creates executables for heuristics.

* `all_bruteforce` - Creates executables for all brute-force methods.

* `all_heuristics` - Creates executables for all heuristic methods.

* `generate` - Creates the executable for generating random graphs.

* `all` - Compiles all the above options.

Be aware that the code for Linux and WSL contains lines that are only used in DEBUG mode in Visual Studio Code. Unfortunately, these lines cannot be disabled at runtime on MacOS. If your objective is to obtain the graph stretch index, this is not a problem for small graphs. However, if you intend to measure runtime, it may pose a challenge. To avoid the execution of these lines, locate those starting with DEBUG and comment them out in your code.

Notably, when compiling with Makefile_OSX (not makefile), DEBUG features for use in VSCODE will not be implemented for MacOS. Debugging on this platform may be limited.

If you are experienced with makefiles and would like to assist in adapting the Linux makefile for MacOS and clang++, please contact us via email.

### 5.2. Instructions for Running on MacOS

To run the code, follow the instructions in [usage](#34-usage).

## 6. Changelog

Link for [changelogs](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/CHANGELOG.md)

## 7. Known Bugs

Link for [known bugs webpage](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/KNOWNBUGS.md)

## 8. To do

Link for [TO DO webpage](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/TODO.md)

## 9. Authorship Information

We're a group of researchers mainly from **Instituto de Computação/Universidade Federal Fluminense (IC/UFF)**(Institute of Computing/Federal Fluminense University) and Universidade Federal Rural do Rio de Janeiro. If you want to inquire about this project, you may e-mail any of the authors listed below.

**TEAM**
* Luís Felipe Ignácio Cunha (lfignacio@ic.uff.br) **[Advisor]**
* Leandro Santiago de Araújo (leandro@ic.uff.br) **[Advisor]**
* Fernanda Couto (fernandavdc@ufrrj.br) **[Advisor]**
* Daniel Juventude (deljuven@ymail.com)
* Carlos Thadeu Santos(carlosthadeu@id.uff.br) (cthadeusantos@gmail.com)
* Anderson Zudio (azudio@id.uff.br)
* Eriky Nunes Marciano (erikymarciano@id.uff.br) 

**FOUNDERS**
* Conselho Nacional de Desenvolvimento Científico e Tecnológico
* Fundação Carlos Chagas Filho de Amparo à Pesquisa do Estado do Rio de Janeiro

**ACKNOWLEDGMENT TO LABIC/UFF HARDWARE SUPPORT**
* We would like to extend our sincere gratitude to the **[LABIC/UFF](https://www.facebook.com/satoruff/)**, at the **[Institute of Computing](https://www.ic.uff.br/)** of the **Federal Fluminense University**, for generously providing their hardware resources for executing all the tests conducted for this experiment. Their hardware support has been ensuring the success of our research.

**LICENSE**
* This project is distributed with MIT license in the hope that it is usefull to anyone (see `LICENSE` at root). Although this project is distributed as free software, this fact do not isent it from being a scientific property. If this project aided your research, please do cite any referenced work from the authors above related in the first section of this file.

## APPENDIX

### Some stretch index values - tables
* [Results for grafos instances (first and original instances 10 to 20 vertices)](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/results/table_grafos_10_20.md)
* [Results for new_grafos instances](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/results/table_new_grafos.md)
* [1746 Results for Jupiter instances](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/results/table_jupiter.md)

### Some stretch index values - result_summary.txt
* [result_summary.txt for grafos instances (first and original instances 10 to 20 vertices)](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/results/grafos/result_summary.txt)
* [result_summary.txt for new_grafos instances](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/results/new_grafos/result_summary.txt)
* [result_summary.txt with 1746 Results for Jupiter instances](https://github.com/cthadeusantos/spanner_tree_generator/blob/main/results/jupiter/result_summary.txt)
