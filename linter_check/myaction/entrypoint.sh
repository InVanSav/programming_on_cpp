#!/usr/bin/env bash

folders=("include" "includes" "src" "source" "sources" "test" "tests") 
for folder in "${folders[@]}"
do
    echo ${folder}
    if [[ -d ${folder} ]]; then
        command="cpplint --recursive ${folder}/*"
        echo ${command}
        if ${command} | grep "error"; then
            echo "::error ::Review your code"
        fi
    fi
done
