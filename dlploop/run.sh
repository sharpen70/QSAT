#!/bin/bash

inputFile=$1
directory=${inputFile%/*}
summary='summary/'${directory//'/'/-}
maxTime=3600
sleepDelay=1

echo ${inputFile##*/} >> $summary

{ ./dlpparser $inputFile $summary -n 500; } &

pId=`ps | grep 'dlpparser' | awk '{print $1}'`

echo "Running dlpparser with $inputFile in background progress($pId)"

execTime=0

while :
do
    sleep $sleepDelay
    execTime=$(($execTime + $sleepDelay))
    # 超时
    if [ $execTime -gt $maxTime ]
    then
        echo "Time out kill process."
        kill $pId
        sleep 1
        break
    fi
    pId2=`ps | grep 'dlpparser' | awk '{print $1}'`
    # 运行完
    if [ x$pId == x"" -o x$pId2 == x"" ]
    then
        echo "done"
        break
    fi
done


