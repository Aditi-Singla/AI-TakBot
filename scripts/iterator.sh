#!/bin/bash
COUNTER=0
while [  $COUNTER -lt 100 ]; do
	echo The counter is $COUNTER
	num=$[COUNTER+20000]
	timeout 120s ./learner.sh $num
	#PID=`ps -eaf | grep run1.sh | grep -v grep | awk '{print $2}'`
	#if [[ "" !=  "$PID" ]]; then
  	#	echo "killing $PID"
  	#	kill -9 $PID
	#fi
	#PID2=`ps -eaf | grep run2.sh | grep -v grep | awk '{print $2}'`
	#if [[ "" !=  "$PID2" ]]; then
  	#	echo "killing $PID2"
  	#	kill -9 $PID2
	#fi	
	let COUNTER=COUNTER+1 
done 
