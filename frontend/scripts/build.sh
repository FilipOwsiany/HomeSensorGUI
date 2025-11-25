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
    echo -e "\t-d Build destination (pc/rpi5). Default is pc"
    echo -e "\t-c Clean the build directory before building"
    echo -e "\t-h Show this help message"
    echo ""
    echo "For rpi5 build you MUST first do:"
    echo "  source /opt/poky-rpi5-sdk/environment-setup-cortexa76-poky-linux"
    exit 1
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

# Decide default build dir (if user didn't pass -p)
if [ $isBuildDirPathSet -eq 0 ]; then
    if [ "$buildDestination" == "pc" ]; then
        buildDirPath="build_pc"
    else
        buildDirPath="build_rpi5"
    fi
fi

# For rpi5 we don't support run
if [[ "$buildDestination" != "pc" ]]; then
    if [ $isRunSet -eq 1 ]; then
        echo "Run option is only supported for 'pc' build destination."
        helpFunction
        exit 1
    fi
fi

# Warnings for defaults
if [ $isBuildTypeSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Build type not set. Using default: $buildType"
fi

if [ $isBuildDestinationSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Build destination not set. Using default: $buildDestination"
fi

if [ $isBuildDirPathSet -eq 0 ]; then
    echo "************************ WARNING ************************"
    echo "Build directory path not set. Using default: $buildDirPath"
fi

# Clean build directory if requested
if [ $isCleanBuildSet -eq 1 ]; then
    if [ -d "$buildDirPath" ]; then
        echo "Cleaning build directory: $buildDirPath"
        rm -rf "$buildDirPath"
    else
        echo "Build directory does not exist, nothing to clean."
    fi
fi

# Create build dir if not exists
if [ ! -d "$buildDirPath" ]; then
    echo "$buildDirPath does not exist. Need to create it."
    mkdir -p "$buildDirPath"
fi

# Go into build dir
cd "$buildDirPath" || { echo "Failed to change directory to $buildDirPath"; exit 1; }

# --- CMake configure ---
if [ ! -d "CMakeFiles" ]; then
    echo "CMakeFiles does not exist. Need to run CMake configure."

    if [ "$buildDestination" == "pc" ]; then
        (
            cmake .. -DCMAKE_BUILD_TYPE=$buildType
        )
        if [ $? -ne 0 ]; then
            echo "CMake configuration failed for PC."
            exit 1
        fi
    else
        # rpi5 build using Yocto SDK
        # check if SDK env is sourced
        if [ -z "$OECORE_NATIVE_SYSROOT" ]; then
            echo "ERROR: OECORE_NATIVE_SYSROOT not set."
            echo "Did you run: source /opt/poky-rpi5-sdk/environment-setup-cortexa76-poky-linux ?"
            exit 1
        fi

        TOOLCHAIN_FILE="$OECORE_NATIVE_SYSROOT/usr/share/cmake/OEToolchainConfig.cmake"

        if [ ! -f "$TOOLCHAIN_FILE" ]; then
            echo "ERROR: Yocto CMake toolchain file not found at:"
            echo "  $TOOLCHAIN_FILE"
            exit 1
        fi

        (
            cmake .. \
                -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
                -DCMAKE_BUILD_TYPE=$buildType
        )
        if [ $? -ne 0 ]; then
            echo "CMake configuration failed for Raspberry Pi 5 (Yocto SDK)."
            exit 1
        fi
    fi
fi

# --- CMake build ---
cmake --build . -j
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

echo "Build completed successfully!"

# --- Run (only PC) ---
if [ $isRunSet -eq 1 ]; then
    echo "Running the built application..."
    ./bin/frontend
fi
