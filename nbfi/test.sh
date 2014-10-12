#!/bin/bash

for i in `ls test | grep -v fail` ; do 
    echo Running test $i:
    echo ---------------------
    bin/nbfi test/$i
    return=$?
    if [ $return != 0 ] ; then
        echo Test $i failed.
        exit 1
    fi
    echo
    echo ---------------------
    echo End of test $i 
    echo
done

for i in `ls test/*fail*` ; do 
    echo Running expected fail test $i:
    echo ---------------------
    bin/nbfi $i
    return=$?
    if [ $return != 127 ] ; then
        echo Test $i failed.
        exit 1
    fi
    echo
    echo ---------------------
    echo End of test $i 
    echo
done
