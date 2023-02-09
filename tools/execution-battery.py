#Author: Anderson Zudio
#Version: 3.0 customizada para este trabalho no dia 9 de fevereiro de 2023
#Date: 07 june of 2022
#Dependency: loader.py and PyYAML (pip3 install pyyaml).
#Modificação: foi retirado a parte que necessita da entrada da seed. Pois, tudo é determinístico por enquanto.

#Comentários originais:
#TODO: Make a way to automatically recover from power surges.
#TODO: Make a way to automatically execute everything that has failed. Do not implement a merge procedure though, may incorporate bad behaviour.
#TODO: Put the exact command line used in every execution in the log. You'll have to alter each script that parse the log aswell.
#TODO: Allow the input to load more than one battery from the same file.

#--Description--
#This script will perform a full execution battery based on multiple independent runs of a {TARGET_PROGRAM} of the project. The {TARGET_PROGRAM} will be searched in the "../build/" directory. The result data is stored inside a {WORKING_FOLDER} named by the user.
#The {INPUT_FILE} is a YAML file made by the user containing: the {WORKING_FOLDER} name, a list of instances to be solved, the path of the binary {TARGET_PROGRAM} file, and some other parameters to customize the execution battery. This tool outputs information to stderr when running. It will create a logfile in the working folder that can be parsed by other tools.
#The script will append the date of execution to the {WORKING_FOLDER} name and will create it at the "../workspace/" directory. The script may also append an increasing number to the {WORKING_FOLDER} to identify multiple batteries made in the same day by the user. For example, if the user uses this script naming the working folder as "RUN_EXAMPLE" on 12 of december 1993, it will attempt to create the folder "RUN_EXAMPLE-12121993-01" in "../workspace/" or "RUN_EXAMPLE-12121993-02", "RUN_EXAMPLE-12121993-03", and so on.
#This script itself should be rarely edited. The user will have to program the loader.py instead to satisfy the following dependencies based on the problem at hand:
#   File extensions: the loader module should tell which file extensions to use for input, output, and error log.
#   Solution manipulation: the loader module should provide functions to read, check, and compare solutions of the problem at hand.
#   The input of the loader tool is an acronym of the problem, like "tsp" for example. The module itself should have at least one example that the user can modify for its own needs.

#--Usage--
#python3 execution-battery.py [0/1 to disable/enable bks maintenance] < {INPUT_FILE}
#{INPUT_FILE}: the YAML file passed by the user.
#The bks maintenance is activated by default.

#--Input format--
#The expected input is a YAML file (see https://yaml.org/spec/). A copy of the original input will be stored inside the working folder for archiving purposes named "script_input.yaml".
#The yaml file should be passed to the stdin and should be formatted as follows:
#---
#name: {EXECUTION_NAME}            #A name to identify the run. The script will use this name to create the "${WORKING_FOLDER}" as "{EXECUTION_NAME}-{TODAY_DATE}-{#}", where the last argument is a number that maintains the folder unique. [see below]
#problem: {PROBLEM_ACRONYM}        #An identification of the problem that is being solved. Should be passed to the loader to determine what methods should be used to compare and read solutions. This will also determine what are the extesions that should be used to map the output file, error file, and the bks file.
#exec: {EXECUTABLE_NAME}           #The executable name that should be located at the source build folder (../build/). This is the executable that will be used in every run.
#description: STRING               #Optional paremeter that has a summary of your test battery to be shown in the log file.
#default_timelimit: integer        #This will dictate the maximum time in seconds that each run can use. If this time expires, then the algorithm is taking too long. It will be denoted in the log if a premature stop is made.
#default_iterations: integer       #This should inform how many independent runs should be done for each instance. Cannot be zero.
#default_args: {FIXED_ARGS}        #Optional parameter that the user may give a list of arguments to be passed to the executable through command line for every run. This script will always append "--seed #" to these arguments, independent if this was specified or not. [see below]
#default_starting_seed: integer    #Optional parameter that will inform the default starting seed. If this isn't given by the user, then the script will start at seed "1". Each output file is named after the seed used. This script will always pass the seed to the executable. [see below]
#instance_list: sequence           #This is a sequence that should contain each instance to be solved as items in the YAML sequence in the following format: 
#- instance: {INSTANCE_NAME}       #Inform which instance should be solved. The name of the instance should be based on the instance folder of the project (../instances/). You may specify the same instance multiple times. 
#  timelimit: integer              #Optional parameter to override the default timelimit in seconds.
#  iterations: integer             #Optional parameter to override the default quantity of executions.
#  args: {ARGS}                    #Optional parameter to override the default argument list that will be passed to the executable.
#  starting_seed: integer          #Optional parameter to override the default starting seed. This will be used only for the respective instance.
#...
#The user may specify the same instance multiple times. This may be necessary when the same instance has to be executed with varying configuration. In this case, starting from the second time the instance is listed, the script will register in the log that the run has a repeated instance. Also, a number will be attached to the instance identification to separate the output/error files and to identify each test battery after the first.
#For example, if you specify the INSTANCE.txt three times, the first time it is found will be a normal execution with no changes in the logfile. The second and third time will be denoted in the log file and their indentification will be INSTANCE-02 and INSTANCE-03 respectively. This will ensure that the test battery result won't be mixed. BKS is mantained normally.
#Each instance that isn't found from the main input file is noted in the log and is ignored by the script.

#--Execution details--
#For each instance in the {INPUT_FILE}, a subfolder is made with the same name as the instance being solved to store the battery data. If the same instance is specified multiple times in the {INPUT_FILE}, the script appends an increasing number at the end of the subfolder to distinguish each setup as metioned above.
#The script will perform calls to the {TARGET_PROGRAM} to solve the instance with an arbitrary number of iterations specified in the {INPUT_FILE}. Each iteration consist of a single independent run. This script uses a seed passed through command line argument as "--seed #", it is expected that the ${TARGET_PROGRAM} knows how to handle this seed. The starting seed number may be given by the user in the ${INPUT_FILE} or left alone to be defaulted as '1'. This script increases the seed number by one for every subsequent iteration made with the ${TARGET_PROGRAM} for the instance being solved.
#Inside each subfolder, the print of the stderr and stdout made by the ${TARGET_PROGRAM} is stored as separated files for each iteration of this tool. Those files are named based on the seed used during the run. 
#For example, if the user start this script with `{WORKING_FOLDER} = RUN_EXAMPLE` on 12/12/1993 using the default starting seed to make two independent runs of instance1.txt and instance2.txt, then the following file structure will be created at "../workpace/" directory:
#  /RUN_EXAMPLE-12121993-01/
#               |----------- instance1/
#               |               |--------- 1.out
#               |               |--------- 1.err
#               |               |--------- 2.out
#               |               |--------- 2.err
#               |               |--------- 2.out
#               |----------- instance2/
#               |               |--------- 1.out
#               |               |--------- 1.err
#               |               |--------- 2.out
#               |               |--------- 2.err
#               |               |--------- 2.out
#               |----------- RUN_EXAMPLE.log
#               |----------- script_input.yaml
#
#If the "RUN_EXAMPLE-12121993-01" folder already exists, then the script will attempt to increase the appended number at the end until it is created successfully. For example: RUN_EXAMPLE-12121993-02, RUN_EXAMPLE-12121993-03, and so on.
#This tool will automatically maintain the best known solution (bks) found through the project for each instance in the "../result/bks/" folder. The {INPUT_FILE} should contain the acronym of the target problem, which is standardized across all the project. The idea is to help the user to mantain a result folder that contains the best solution found through all the runs made within the project.
#The script will look for each instances inside the "../instances/" directory. Each file must have an unique name or the script will behave incorrectly.
#This script will make a copy of the {INPUT_FILE} into the {WORKING_FOLDER} named "script_input.yaml" for archiving purposes.
#If you need to include another problem to work with this tool, you don't need to edit the code under this script. This tool should rarely be modified. The only module that should be coded is the loader module "loader.py" that should provide the respective methods needed to handle the files related to the target problem.
#It also worth noting that if the execution of the {TARGET_PROGRAM} fails by rising a non-zero return, time limit, or from an user made check, then this tool will register the occurence in the log file and the bks maintence is ignored.
#Finally, the user may see some details about the ongoing run by catching the stderr output of this script. If anything goes wrong, you may cross the information printed to stderr with the generated log file inside the {WORKING_FOLDER} to track the issue.

#--Warning--
#Make sure that the main application (binary) is prepared to receive the "--seed #" command by line argument, even if the algorithm is deterministic.
#This script will append the above argument at the beginning of every execution to guarantee that each run will generate a distinct but controlled output for each execution. If the {TARGET_PROGRAM} is well designed, then the execution will always be replicable.
#Make sure that every instance has a unique name without special characters.
#The number of iterations (independent runs) cannot be zero.
#Do not manually edit the log file or modify the folder structure unless you know what you are doing. By not following the standard made by the script, other tools that use it may break.
#Make sure that each instance has an unique name, even if they are in separated folders.

#--Tips--
#If you need to extend this script to work with another problem from the project, then you only need to edit the Loader class found in the "loader.py" module.
#If an execution battery has some failed runs, due to risen errors of the executable or power outrage for example, you may easily recover by looking at the information found in the log:
#   --> Backup everything from the folder.
#   --> Remove from the original input every instance that has been successfully executated through all iterations.
#   --> Alter each instance that has failed in the ${INPUT_FILE} so that each run that failed to execute has the remaining correct number of iterations through the "iterations" field. You should change the starting seed number aswell. !!!Be careful with non deterministic algorithms: the seed passed is based on the itaration that is being made, don't let the same seed be used twice for the same test battery or you may compromise your test data!!!
#   --> Execute the tool again in the same computer used in the original run (or your data may be compromised for a scientific work).
#   --> Merge the new execution folder with the original one. If you changed the starting seed accordingly, then the files should go nicely with each other due to the naming convention.
#   --> Manually edit the resulting original log to merge with the new one. Each text battery and iteration is logged individually, it is easy to merge the files with simple copy and paste.
#   --> The bks is managed automatically. You don't need to worry about it. If any error is risen, it won't be updated.
#   --> Run the analyze tool to devise a second log file with a pretty result summary.
#The user should use this script for any serious execution battery, as it will always maintain the bks found through your research project in the "../results/bks/" folder.
#This script doesn't make a backup of the bks folder. It is important to keep the bks folder inside a proper versioning platform (like git) so you can recover incase of bad overwrite.

import os
import sys
from datetime import datetime
import yaml
import subprocess
import shutil
import re
import platform
import loader

#Everything bellow here may be left as is
#--Globals--
#Common directories that are used in the script
workspace_dir = "../workspace/" #the workspace folder of the project
bks_dir = "../results/bks/" #where I should store the best known solution
main_dir = '' #the directory that will hold the all the files generated by this script execution
instance_base_dir = "../instances/" #The directory that has the instances
build_dir = "../build/" #The directory where the build files are

#Environment information
date = {} #current computer date for execution logging
cpu_name = "" #the current processor name of the environment
operational_system = "" #what system the environment is hosting
ram_amount = "" #How much RAM the system has available

#Script setup
maintain_bks = True #if True, then best known solution maintaince will be enabled. May be disabled through command line parameter.
problem_name = "" #Will keep the problem name given by stdin
run_name = "" #Will keep the run name given by stdin
executable_path = "" #Will keep the name of the executable
default_arguments = "" #Will keep the arguments that should be passed to the executable as command line parameters
default_timelimit = 0 #how much time in seconds each run will have at maximum to execute.
default_starting_seed = 0 #the starting seed that should be used for every test battery
default_iteration = 0 #how many times each instance will be executed. 
instance_list = [] #Will keep the yaml sequence (list) that contain each configuration that should be solved
description = "" #The user has the option to provide a summary for the test battery. This will be stored here.

#Other variables
data = {} #The original yaml data passed as input
log_file = "" #The file that will keep the log
execution_folder = "" #The name of the folder that is being worked upon

#Variables that are computed based on the problem_name. They should be loaded from the Loader class.
compare_function = None #Will keep the function that should be used to compare a solution with the bks.
read_function = None #Will keep the function that should be used to read solutions from files.
check_function = None #Will keep the function that should be used to check a solution. Should return false if the solution has some problem that should be listed in the log, true otherwise.
in_extension = "" #The extension that identifies the input file.
out_extension = "" #The extension that should be used when outputing files to stdout.
err_extension = "" #The extension that should be used when outputing files to stderr.
bks_extension = "" #The extension that identifies the bks file.

#--Script functions that does not need to be altered--
#Will check if the directories are accessible
def checkDir(): 
    global workspace_dir
    global instance_base_dir
    global build_dir

    if(not os.path.exists(workspace_dir)):
        sys.stderr.write("Workspace directory not found: " + workspace_dir + " Aborting execution. (Did you move this script? Executing outside of the tools folder?)\n")
        exit()
    if(not os.path.exists(instance_base_dir)):
        sys.stderr.write("Instance instance directory not found: " + instance_base_dir + " Aborting execution. (Did you move this script? Executing outside of the tools folder?)\n")
        exit()
    if(not os.path.exists(build_dir)):
        sys.stderr.write("Build directory not found: " + build_dir + " Aborting execution. (Did you move this script? Executing outside of the tools folder?)\n")
        exit()
    sys.stderr.write("All directories are accessible.\n")

#Will return the name of the current CPU
def getProcessorName(): 
    if platform.system() == "Windows":
        return platform.processor()
    elif platform.system() == "Linux":
        proc_info = open("/proc/cpuinfo", "r")
        for line in proc_info.readlines():
            if "model name" in line:
                proc_info.close()
                return re.sub( ".*model name.*:", "", line,1).rstrip('\n')
    return ""

#Will return the amount of physical memory this computer has
def getRAM(): 
    if platform.system() == "Linux":
        meminfo = dict((i.split()[0].rstrip(':'),int(i.split()[1])) for i in open('/proc/meminfo').readlines())
        mem_kib = int(meminfo['MemTotal'])
        return str(mem_kib/1024**2)
    else:
        return ""

#Will fill the date settings of the script
def loadSystemInformation(): 
    global date
    global cpu_name
    global ram_amount 
    global operational_system
    
    date_string = str(datetime.date(datetime.now()))
    date_list = date_string.split('-')
    date["day"] = date_list[2]
    date["month"] = date_list[1]
    date["year"] = date_list[0]
    cpu_name = getProcessorName()
    ram_amount = getRAM()
    operational_system = str(os.uname())
    sys.stderr.write("Current Date: " + date["day"] + "/" + date["month"] + "/" + date["year"] + "\n")
    sys.stderr.write("Identified CPU: " + cpu_name + "\n")
    sys.stderr.write("Total Ram: " + ram_amount + " (gb) \n")
    sys.stderr.write("OS Information: " + operational_system + '\n')

#Will fill the script run parameters
def readParam(): 
    global maintain_bks
    if len(sys.argv) > 1:
        if(sys.argv[1] == "0"):
            maintain_bks = False
            sys.stderr.write("Best known solution maintenance disabled.\n")
        else:
            sys.stderr.write("Best known solution maintenance enabled.\n")
    else:
        sys.stderr.write("No argument has been passed through command line. BKS maintenance enabled.\n")

#Will load the yaml passed as input
def loadYaml(): 
    global data
    global problem_name
    global run_name
    global executable_path
    global default_arguments
    global default_timelimit
    global default_starting_seed
    global default_iteration
    global instance_list
    global description

    data = yaml.load(sys.stdin, Loader = yaml.loader.SafeLoader)
    if(not "name" in data):
        sys.stderr.write('Yaml input is missing the \'name\' key. Aborting execution.\n')
        exit()
    else:
        run_name = data["name"]
        sys.stderr.write('Loaded run name: ' + run_name + '\n')

    if(not "problem" in data):
        sys.stderr.write('Yaml input is missing the \'problem\' key. Aborting execution.\n')
        exit()
    else:
        problem_name = data["problem"]
        sys.stderr.write('Loaded problem name: ' + problem_name + '\n')

    if(not "exec" in data):
        sys.stderr.write('Yaml input is missing the \'exec\' key. Aborting execution.\n')
        exit()
    else:
        executable_file = data["exec"]
        sys.stderr.write('Executable file name: ' + executable_file + '\n')
        if(not os.path.exists(build_dir + executable_file)):
            sys.stderr.write('Executable file not found at dir: ' + build_dir + executable_path + '. Aborting execution.\n')
            exit()
        else:
            executable_path = build_dir + executable_file 
            sys.stderr.write('Executable file found at ' + executable_path + '\n')

    if(not "default_args" in data):
        sys.stderr.write('Yaml input is missing \'default_args\'. Only seed will be passed by default.\n')
        default_arguments = ""
    else:
        default_arguments = data["default_args"]
        sys.stderr.write('Default argument line: ' + default_arguments + '\n')

    if(not "default_timelimit" in data):
        sys.stderr.write('Yaml input is missing \'default_timelimit\'. Aborting execution.\n')
        exit()
    else:
        default_timelimit = int(data["default_timelimit"])
        sys.stderr.write('Default timelimit: ' + str(default_timelimit) + ' (s)\n')

    if(not "default_starting_seed" in data):
        sys.stderr.write('Yaml input is missing \'default_starting_seed\'. Every test battery will start with seed zero by default.\n')
        default_starting_seed = 0
    else:
        default_starting_seed = int(data["default_starting_seed"])
        sys.stderr.write('Default starting seed number: ' + str(default_starting_seed) + '\n')

    if(not "default_iterations" in data):
        sys.stderr.write('Yaml input is missing \'default_iterations\'. Aborting execution.\n')
        exit()
    else:
        default_iteration = int(data["default_iterations"])
        sys.stderr.write('Default number of iterations: ' + str(default_iteration) + '\n')
        if default_iteration == 0:
            sys.stderr.write('The default number of iterations cannot be zero. Aborting execution.\n')
            exit()

    if(not "description" in data):
        sys.stderr.write('Yaml input is missing \'description\'. The user has not specified a summary for this test battery. Using blank description.\n')
        description = ""
    else:
        description = data["description"]
        sys.stderr.write('Test battery description: ' + description + '\n')

    if(not "instance_list" in data):
        sys.stderr.write('Yaml input is missing \'instance_list\'. Aborting execution.\n')
        exit()
    else:
        instance_list = data["instance_list"]
        sys.stderr.write('Length of instance list object: ' + str(len(instance_list)) + '\n')

#Will load basic problem info based on the information given by the module loader.py
def loadProblemInfo(): 
    global problem_name
    global compare_function
    global read_function
    global check_function
    global in_extension
    global out_extension
    global err_extension
    global bks_extension

    dependency = loader.load(problem_name)
    if(not bool(dependency)):
        sys.stderr.write("The problem " + problem_name + " hasn't been identified in the loader.py script. Aborting execution.\n")
        exit()
    else:
        compare_function = dependency["compare_function"] 
        read_function = dependency["read_function"] 
        check_function = dependency["check_function"] 
        in_extension = dependency["in_extension"] 
        out_extension = dependency["out_extension"] 
        err_extension = dependency["err_extension"] 
        bks_extension = dependency["bks_extension"] 
        sys.stderr.write("Dependencies for " + problem_name + " loaded successfully.\n")
        sys.stderr.write("Input file extension: " + in_extension + "\n")
        sys.stderr.write("Out file extension: " + out_extension + "\n")
        sys.stderr.write("Error file extension: " + err_extension + "\n")
        sys.stderr.write("Bks file extension: " + bks_extension + "\n")

#Creates the main execution dir with the same structure as the passed input
def makeDirectory(): 
    global main_dir
    global execution_folder
    global run_name
    global date
    attempt = 1
    while True:
        number = str(attempt)
        if(attempt < 10):
            number = '0' + number #appending zero due to better reading when in terminal environment
        execution_folder = run_name + "-" + date["day"] + date["month"] + date["year"] + "-" + number
        main_dir = workspace_dir + execution_folder + os.sep
        if os.path.exists(main_dir):
            attempt += 1
        else:
            os.mkdir(main_dir)
            break
    sys.stderr.write("Successfully created the directory: " + main_dir + "\n")

#Will load the bks directory. If it doesn't exist, then it will be created.
def loadBks(): 
    global bks_dir
    bks_dir += problem_name + os.sep
    if not os.path.exists(bks_dir):
        os.mkdir(bks_dir)
        sys.stderr.write("Best known solution folder not found. The following folder has been made: " + bks_dir + "\n")
    else:
        sys.stderr.write("Best known solution folder found: " + bks_dir + "\n")

#This will create the log file and write the initial important stuff to it
def createLogFile(): 
    global bks_dir
    global log_file
    global execution_folder
    global date
    global cpu_name
    global operational_system
    global ram_amount
    global maintain_bks
    global problem_name
    global run_name
    global executable_path
    global default_arguments
    global default_timelimit
    global default_starting_seed
    global default_iteration
    global instance_list

    log_file_dir = main_dir + run_name + ".log"
    log_file = open(log_file_dir, "w")    
    log_file.write("Starting full execution of problem " + problem_name + " using " + run_name + ' input\n')
    if(description != ""):
        log_file.write("Test battery description: " + description + '\n')
    log_file.write("Current date: " + date["day"] + '-' + date["month"] + '-' + date["year"] + '\n')
    log_file.write("Current operational system information: " + operational_system + '\n')
    log_file.write("Current CPU information: " + cpu_name + '\n')
    log_file.write("Amount of RAM (gb): " + ram_amount + '\n')
    log_file.write("Binary executable file name: " + executable_path + '\n')
    log_file.write("Default argument list: \" " + default_arguments + "\" \n")
    log_file.write("Default run time limit in seconds: " + str(default_timelimit) + '\n')
    log_file.write("Default number of runs: " + str(default_iteration) + '\n')
    log_file.write("Default starting seed: " + str(default_starting_seed) + '\n')
    log_file.write("Amount of instances given in the input: " + str(len(instance_list)) + '\n')
    log_file.write("Run folder name: " + execution_folder + '\n')
    if maintain_bks:
        log_file.write("Bks maintenance is enabled at directory: " + bks_dir + '\n')
    else:
        log_file.write("Bks maintenance is disabled.\n")
    sys.stderr.write("Log file created successfully: " + log_file_dir + "\n")

#This will store the original yaml input file into the execution folder for archive purposes
def logYamlInput(): 
    global data
    yaml_file_name = main_dir + "script_input.yaml"
    yaml_file = open(yaml_file_name, "w")
    yaml.dump(data, yaml_file, default_flow_style = False)
    sys.stderr.write("Original input yaml dumped into: " + yaml_file_name)
    yaml_file.close()

#The main execution step of the script
def execute(): 
    global log_file
    global instance_list
    global main_dir
    global executable_path
    global default_arguments
    global default_timelimit
    global default_starting_seed
    global default_iteration
    global maintain_bks
    global bks_dir
    
    for object in instance_list: #for each instance 
        sys.stderr.write("Processing object " + str(object) + '\n')
        if(not "instance" in object):
            sys.stderr.write("Object doesn't have instance name. Object process has been aborted.")
            continue
        instance_full_path = os.path.join(instance_base_dir, object["instance"])
        sys.stderr.write("Full instance path: " + instance_full_path + '\n')
        log_file.write("Processing instance: " + instance_full_path + ' -- ')
        if(not os.path.exists(instance_full_path)):
            sys.stderr.write("Instance not found. Aborting process.\n")
            log_file.write("Instance not found. Aborting process.\n")
            continue

        sys.stderr.write("Instance found.\n")
        repeated = False
        actual_dir = os.path.join(main_dir, object["instance"].replace(in_extension, '')) 
        if(os.path.exists(actual_dir)):
            sys.stderr.write("Repeated instance. Appending a number to the directory.\n")
            repeated = True
            i = 2
            while True:
                number = str(i)
                if i < 10:
                    number = '0' + str(i) #appending a zero due to better format in terminal environments
                new_dir = actual_dir + '-' + number
                if(not os.path.exists(new_dir)):
                    actual_dir = new_dir
                    break
                i = i+1
        
        actual_dir = actual_dir + os.sep
        sys.stderr.write("Creating directory: " + actual_dir)
        os.makedirs(actual_dir) #create the respective directory inside the main folder
    
        sys.stderr.write("Loading optional keys.\n")
        current_timelimit = default_timelimit
        current_run_amount = default_iteration
        current_starting_seed = default_starting_seed
        current_args = default_arguments
        if(repeated):
            log_file.write("Repeated instance. ")
        if("timelimit" in object):
            sys.stderr.write("Overriding timelimit.\n")
            current_timelimit = object["timelimit"]
            log_file.write("Overriding timelimit: " + str(current_timelimit) + ". ")
        else:
            log_file.write("Using default timelimit: " + str(current_timelimit) + ". ")
        if("iterations" in object):
            sys.stderr.write("Overriding run amount.\n")
            current_run_amount = object["iterations"]
            log_file.write("Overriding run amount: " + str(current_run_amount) + ". ")
        else:
            log_file.write("Using default iteration amount: " + str(current_run_amount) + ". ")
        if("starting_seed" in object):
            sys.stderr.write("Overriding starting seed.\n")
            current_starting_seed = object["starting_seed"]
            log_file.write("Overriding starting seed: " + str(current_starting_seed) + ". ")
        else:
            log_file.write("Using default starting seed: " + str(current_starting_seed) + ". ")
        if("args" in object):
            sys.stderr.write("Overriding argument line.\n")
            current_args = object["args"]
            log_file.write("Overriding argument line: \"" +  current_args + "\"\n")
        else:
            log_file.write("Using default args: \"" + current_args + "\".\n")

        executable_line = executable_path + ' ' + current_args #this is the full line that should be executed in the terminal
        sys.stderr.write("Starting execution with " + str(current_run_amount) + " independent runs.  Target directory: " + actual_dir.replace(main_dir, "") + ". Command line: [ " + executable_line + " ].\n")
        log_file.write("Starting execution with " + str(current_run_amount) + " independent runs. Target directory: " + actual_dir.replace(main_dir, "") + ". Command line: [ " + executable_line + " ].\n")
        for i in range(1, current_run_amount+1): #running the executable for 'n_run' iterations
            actual_seed = i + current_starting_seed - 1
            sys.stderr.write("Running iteration "  + str(i) + " of " + str(current_run_amount) + ' with seed ' + str(actual_seed) + '. ')
            log_file.write("Running iteration " + str(i) + " of " + str(current_run_amount) + ' with seed ' + str(actual_seed) + '. ')
            error = False #signals if there's an error with the actual execution

            out_full_path = actual_dir + str(actual_seed) + out_extension #this is the output full file path
            err_full_path = actual_dir + str(actual_seed) + err_extension #this is the stderr full file path
            file_in = open(instance_full_path, "r") #the input
            file_out = open(out_full_path, "w+") #the output file that will receive the solution
            file_err = open(err_full_path, "w+")  #the file that will receive the stderr
            executable_list = [i for i in executable_line.split()] #this is the line that should be executed in the terminal
            #executable_list.append("-s") #passing the seed parameter to the algorithm
            #executable_list.append(str(actual_seed)) #passing the actual seed to the algorithm

            try: #running the algorithm with the instance
                subprocess.run(executable_list, stdin = file_in, stdout = file_out, stderr = file_err, check = True, timeout = current_timelimit) #main execution line
            except subprocess.CalledProcessError: #if this passes, then it was probably a seg fault.
                error = True
                log_file.write("Run failed. Non-zero exit code has risen. ")
            except subprocess.TimeoutExpired: #if this passes, then the execution took too long
                error = True
                log_file.write("Run failed. Timelimit reached. ")
            if not error:
                file_out.seek(0) #will read the solution to check if it is OK
                actual_solution = read_function(file_out)
                if not check_function(actual_solution): #this is user made. Should check if OK.
                    sys.stderr.write("Run failed. Check function has risen an error. ")
                    log_file.write("Run failed. Check function has risen an error. ")
                    error = True

            file_in.close()
            file_out.close()
            file_err.close()
            if not error:
                sys.stderr.write("Run success. ")
                log_file.write("Run success. ")
                if maintain_bks: #in this step, if enabled, we will compare the output solution with the bks.
                    sys.stderr.write(" Maintaining the bks. ")
                    instance_name = object["instance"].split(os.sep)[-1].replace(in_extension, '')
                    bks_file_dir = os.path.join(bks_dir, object["instance"].replace(in_extension, '').replace(instance_name, '')) #the bks directory
                    bks_full_path = os.path.join(bks_file_dir, instance_name + bks_extension) #the bks full file path

                    if not os.path.exists(bks_file_dir): #creating the respective dir if necessary
                        sys.stderr.write(" Creating bks dir " + bks_file_dir + ' ')
                        os.makedirs(bks_file_dir)
                    if not os.path.exists(bks_full_path): #checking if the file itself exists
                        sys.stderr.write(" Previous bks didn't exist. Copying the file directly. ")
                        log_file.write("New bks was found. ")
                        shutil.copyfile(out_full_path, bks_full_path) #if the file didn't exist. Then just make a copy of the output
                    else: #this means that a bks exist. This is the most likely execution flow.
                        sys.stderr.write(" Bks file found. ")
                        file_bks = open(bks_full_path, "r")
                        bks = read_function(file_bks) #reading the bks 
                        file_bks.close()
                        if compare_function(actual_solution, bks): #comparing the solution from this run with the bks.
                            sys.stderr.write(" New bks was found. Overwriting the old one. ")
                            log_file.write("New bks was found. ")
                            shutil.copyfile(out_full_path, bks_full_path) #Wohoo!!! That an overall improvement! Congratulations
                        else:
                            sys.stderr.write(" Not a bks. ")

            sys.stderr.write("Done.\n")
            log_file.write("Done.\n")
            log_file.flush()
    log_file.write("Successfully Finished.\n")
    log_file.close()

#--Main-- Doesn't need to be changed
sys.stderr.write("Checking if the integrity of the directories.\n")
checkDir()
sys.stderr.write("Loading the system information.\n")
loadSystemInformation()
sys.stderr.write("Reading the command line parameters.\n")
readParam()
sys.stderr.write("Loading the input Yaml.\n")
loadYaml()
sys.stderr.write("Loading problem info.\n")
loadProblemInfo()
sys.stderr.write("Making the target directory.\n")
makeDirectory()
sys.stderr.write("Loading bks solution files based on the given problem name.\n")
loadBks()
sys.stderr.write("Creating the main log file.\n")
createLogFile()
sys.stderr.write("Writing the input yaml inside the execution folder.\n")
logYamlInput()
sys.stderr.write("Starting main execution routine.\n")
execute()
sys.stderr.write("Test battery finished successfully.\n")
