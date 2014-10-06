#!/bin/bash

for i in `ls test` ; do 
    echo Running test $i:
    echo ---------------------
    bin/nbfi test/$i
    echo ---------------------
    echo End of test $i 
    echo
done
