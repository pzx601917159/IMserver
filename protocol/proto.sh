#!/bin/bash 
arr=`ls -l ./*.proto|awk '{print $9}'`
for i in $arr;do
    #echo $i
    protoc -I=./ --cpp_out=./ $i
done
