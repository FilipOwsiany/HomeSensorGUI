#!/bin/bash

# Default values
isCleanBuildSet=0

isRunSet=0

buildType="Debug"
isBuildTypeSet=0

buildDestination="pc"
isBuildDestinationSet=0

buildDirPath="build_pc"
isBuildDirPathSet=0

helpFunction()
{
    echo ""
    echo "Usage: $0 -b buildType -d buildDestination"
    echo -e "\t-r Run the built application after building (only for pc build destination)"
    echo -e "\t-p Build directory path. Default is build_pc or build_rpi5 based on destination"
    echo -e "\t-b Build type (Release/Debug). Default is Debug"
    echo -e "\t-d Build destination (pc/android). Default is pc"
    echo -e "\t-c Clean the build directory before building"
    echo -e "\t-h Show this help message"
    exit 1 # Exit script after printing help
}

VALID_ARGS=$(getopt -o crb:d:p:h --long clean,run,build:,dest:,path:,help -- "$@")
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
    -b | --build)
        echo "Build type option selected."
        buildType="$2"
        isBuildTypeSet=1
        shift 2
        ;;
    -d | --dest)
        echo "Build destination option selected."
        buildDestination="$2"
        isBuildDestinationSet=1
        shift 2
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

# Clean build directory if requested
if [ $isCleanBuildSet -eq 1 ]; then
    if [ -d "$buildDirPath" ]; then
        echo "Cleaning build directory: $buildDirPath"
        rm -rf "$buildDirPath"
    else
        echo "Build directory does not exist, nothing to clean."
    fi
fi

# Validate build type
if [[ "$buildType" != "Release" && "$buildType" != "Debug" ]]; then
    echo "Invalid build type. Allowed values are 'Release' or 'Debug'."
    helpFunction
    exit 1
fi

# Validate build destination
if [[ "$buildDestination" != "pc" && "$buildDestination" != "rpi5" ]]; then
    echo "Invalid build destination. Allowed values are 'pc' or 'rpi5'."
    helpFunction
    exit 1
fi

# Create build directory if it doesn't exist
if [ $isBuildDirPathSet -eq 0 ]; then
    if [ "$buildDestination" == "pc" ]; then
        buildDirPath="build_pc"
    elif [ "$buildDestination" == "rpi5" ]; then
        buildDirPath="build_rpi5"
    fi
fi

if [[ "$buildDestination" != "pc" ]]; then
    if [ $isRunSet -eq 1 ]; then
        echo "Run option is only supported for 'pc' build destination."
        helpFunction
        exit 1
    fi
fi

if [ $isBuildTypeSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Build type not set. Using default: Release"
fi

if [ $isBuildDestinationSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Build destination not set. Using default: pc"
fi

if [ $isBuildDirPathSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Build directory path not set. Using default: $buildDirPath"
fi

if [ $? -ne 0 ]; then
    echo "Failed to change directory to $buildDirPath"
    exit 1
fi

if [ ! -d "$buildDirPath" ]; then
    echo "$buildDirPath does not exist. Need to create it."
    mkdir "$buildDirPath"
fi

cd "$buildDirPath"

if [ ! -d "CMakeFiles" ]; then
    echo "CMakeFiles does not exist. Need to create it."

    if [ "$buildDestination" == "pc" ]; then
        (
            cmake .. -DCMAKE_BUILD_TYPE=$buildType
        )
        if [ $? -ne 0 ]; then
            echo "CMake configuration failed for PC."
            exit 1
        fi
    elif [ "$buildDestination" == "rpi5" ]; then
        (
            cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-aarch64-rpi5.cmake -DCMAKE_BUILD_TYPE=$buildType ..
        )
        if [ $? -ne 0 ]; then
            echo "CMake configuration failed for Raspberry Pi 5."
            exit 1
        fi
    else
        echo "Unsupported build destination: $buildDestination"
        exit 1
    fi

fi

(
    cmake --build . -j
)

if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

echo "Build completed successfully!"

if [ $isRunSet -eq 1 ]; then
    echo "Running the built application..."
    ./bin/displayApplication
fi




