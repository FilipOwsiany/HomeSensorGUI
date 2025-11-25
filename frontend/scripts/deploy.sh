#!/bin/bash

# Default values
isCleanBuildSet=0

isRunSet=0

buildDirPath="build_rpi5"
isBuildDirPathSet=0

targetIP="192.168.100.50"
isTargetIPSet=0

helpFunction()
{
    echo ""
    echo "Usage: $0 -c -r -p buildDirPath"
    echo -e "\t-c Clean the build directory on target before deploying"
    echo -e "\t-r Run the application on target after deploying"
    echo -e "\t-p Build directory path. Default is build_rpi5"
    echo -e "\t-h Show this help message"
    exit 1 # Exit script after printing help
}

VALID_ARGS=$(getopt -o crp:h --long clean,run,path:,help -- "$@")
if [[ $? -ne 0 ]]; then
    helpFunction
    exit 1;
fi

eval set -- "$VALID_ARGS"
while [ : ]; do
  case "$1" in
    -c | --clean)
        echo "Clean build option selected."
        isCleanBuildSet=1
        shift
        ;;
    -r | --run)
        echo "Run build option selected."
        isRunSet=1
        shift
        ;;
    -p | --path)
        echo "Build directory path option selected."
        buildDirPath="$2"
        isBuildDirPathSet=1
        shift 2
        ;;
    -h | --help)
        helpFunction
        shift
        ;;
    --) shift; 
        break 
        ;;
  esac
done

if [ $isBuildDirPathSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Build directory path not set. Using default: $buildDirPath"
fi

if [ $isTargetIPSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Target IP address not set. Using default: $targetIP"
fi

ping -c 4 $targetIP | grep "errors" > /dev/null
if [ $? -ne 0 ]; then
    echo "Target $targetIP is reachable."
else
    echo "Target $targetIP is not reachable. Please check the connection."
    exit 1
fi

# Stop any running instances of the application on the target
ssh root@"$targetIP" "
pids=\$(pidof frontend)
if [ -n \"\$pids\" ]; then
    echo 'Application is already running, stopping all instances:'
    kill -9 \$pids
    echo 'All instances stopped.'
else
    echo 'Application is not running, nothing to stop.'
fi"

# Clean build directory on target if requested
if [ $isCleanBuildSet -eq 1 ]; then
    buildDirPathToClean="/home/$buildDirPath"
    ssh root@$targetIP "if [ -d '$buildDirPathToClean' ]; then echo 'Cleaning build directory: $buildDirPathToClean'; rm -rf '$buildDirPathToClean'; else echo 'Build directory on target does not exist, nothing to clean.'; fi"
fi

# Check if local build directory exists
if [ ! -d "$buildDirPath" ]; then
    echo "Build directory does not exist!"
    exit 1  
fi

# Create necessary directories on target
buildDirPathRoot="/home/$buildDirPath"
buildDirPathBin="/home/$buildDirPath/bin"
buildDirPathLib="/home/$buildDirPath/lib"
buildDirPathLogs="/home/$buildDirPath/logs"

ssh root@"$targetIP" "if [ ! -d '$buildDirPathRoot' ]; then
    echo 'Build directory root does not exist, need to be created';
    mkdir -p '$buildDirPathRoot';
else
    echo 'Build directory root on target does exist, nothing to clean.';
fi"

ssh root@"$targetIP" "if [ ! -d '$buildDirPathBin' ]; then
    echo 'Build directory bin does not exist, need to be created';
    mkdir -p '$buildDirPathBin';
else
    echo 'Build directory bin on target does exist, nothing to clean.';
fi"

ssh root@"$targetIP" "if [ ! -d '$buildDirPathLib' ]; then
    echo 'Build directory lib does not exist, need to be created';
    mkdir -p '$buildDirPathLib';
else
    echo 'Build directory lib on target does exist, nothing to clean.';
fi"

ssh root@"$targetIP" "if [ ! -d '$buildDirPathLogs' ]; then
    echo 'Build directory logs does not exist, need to be created';
    mkdir -p '$buildDirPathLogs';
else
    echo 'Build directory logs on target does exist, nothing to clean.';
fi"

# Copy binaries and libraries to target
scp "$buildDirPath/bin/"* root@"$targetIP":"$buildDirPathBin"/
if [ $? -ne 0 ]; then
    echo "Failed to copy binaries to target."
    exit 1
fi

scp "$buildDirPath/lib/"* root@"$targetIP":"$buildDirPathLib"/
if [ $? -ne 0 ]; then
    echo "Failed to copy libraries to target."
    exit 1
fi

# Run the application on target if requested
if [ $isRunSet -eq 1 ]; then

    ssh root@"$targetIP" "appDate=\$(date +'%Y-%m-%d_%H-%M-%S'); \
        /home/$buildDirPath/bin/frontend > /home/$buildDirPath/logs/output_\$appDate.log 2>&1 &"

    if [ $? -ne 0 ]; then
        echo "Failed to start application on target."
        exit 1
    fi
fi



