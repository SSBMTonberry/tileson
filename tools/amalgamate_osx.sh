#!/bin/bash -ue

TOP_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )/.." &> /dev/null && pwd )

function bin_found {
    builtin type -P "$1" &> /dev/null
}

function missing_quom {
    cat <<EOF
This script depends on the quom utility to combine the header files:
    https://github.com/Viatorus/quom

Quom is a python 3.6 (or above) utility that can be installed via pip:
    pip install quom

Or on OSX with python3 from homebrew installed:
    pip3 install quom

Ensure that the quom utility is in your PATH when running this script:
    PATH="~/Library/Python/3.10/bin:\$PATH" $0
EOF
    exit 1
}

bin_found "quom" || missing_quom
set -x
cd "$TOP_DIR"
quom --include_directory="./include" "./include/tileson.h" "./tileson.hpp"
quom --include_directory="./include" "./include/tileson_min.h" \
    "./tileson_min.hpp"
