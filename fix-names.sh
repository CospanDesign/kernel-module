#! /bin/bash

NAME=mymodule

echo "Fix all the names of the repository"

if [[ $# != 1 ]]; then
  printf "Please enter the name of the module\n\n"
  printf "%s <name module>\n\n" $0
  printf "Example: %s demo\n" $0
  exit -1
fi

NAME=$1

echo "grep -rInl 'mymodule' | xargs sed -i 's/mymodule/${NAME}/g'"
