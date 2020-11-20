#!/bin/bash

EXE=./convert

for OBJ in `find ./*.txt`
do
   $EXE $OBJ
done