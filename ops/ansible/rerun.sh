#!/usr/bin/env bash

set -e

POSITIONAL=()

build(){
   ./build.sh
}


while [[ $# -gt 0 ]]
do
    key="$1"

    case $key in
        -b|--build)
        build
        shift
        ;;
    esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

ansible-playbook -i inventories/inventory playbooks/rerun_nodes.yml