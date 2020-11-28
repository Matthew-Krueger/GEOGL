#!/bin/bash

while getopts "da" flag; do
	case "${flag}" in
	  d) dodoxygen="TRUE";;
	  a) doarchive="TRUE";;
	  b) build="TRUE";;
  esac
done


# Setup
echo "Ensuring Submodules are cloned."
git submodule update --init --recursive || exit

# CMake
echo "Configuring CMake."
rm -rf Build || exit
mkdir -p Build
cd Build || exit
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../_install .. || exit
echo "Configuring CMake done."

if [ "$build" == "TRUE" ]; then
  # C Compiling
  echo "Compiling the engine and example meta package"
  make GEOGL_Example
  echo "Engine and example meta package compiled"
fi

#mkdir -p bin || exit
#cp Sources/*/lib* bin/ || exit
#cp Sources/Examples/*/Example* bin/ || exit

if [ "$dodoxygen" == "TRUE" ]; then
  # Doxygen
  echo "Cleaning Old Doxygen"
  rm -rf ../Docs || exit
  echo "Making Doxygen"
  make GEOGL_Doxygen

  # Archive Doxygen
  echo "Archive Doxygen"
  tar cvzf bin/Docs.tar.gz ../Docs
fi

if [ "$doarchive" == "TRUE" ]; then
  echo "Archiving Artifacts"
  mkdir -p bin/artifacts

  if [ "$build" == "TRUE" ]; then
    echo "Archiving Libraries"
    cd bin || exit
    tar cvzf artifacts/Libraries.tar.gz lib*
    cd ..
  fi

  echo "Archiving Doxygen"
  if [ -e bin/Docs.tar.gz ]
    then
    cp bin/Docs.tar.gz bin/artifacts
  fi

  if [ "$build" == "TRUE" ]; then
    echo "Archiving Examples"
    cd bin/Examples || exit
    tar -cvzf "../artifacts/Examples.tar.gz" *;
    cd ../..
  fi
fi