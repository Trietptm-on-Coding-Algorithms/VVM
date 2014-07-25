#!/bin/bash

for i in `ls test` ; do 
    bin/nbfi test/$i
done
