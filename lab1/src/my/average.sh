#!/bin/sh
count=0
sum=0
while [ -n "$1" ]
do
sum=$[$sum+$1]
count=$[ $count + 1 ]
shift
done
echo $count
echo $sum
let "ret=sum / count"
echo $ret 