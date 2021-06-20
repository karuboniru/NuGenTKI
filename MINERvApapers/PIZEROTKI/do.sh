#!/bin/bash

mkdir -p outplot/final

for aa in mcchi2 draw  
do

    mkexe.sh ${aa} -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style
    
    ./${aa} > see${aa}.log

done


