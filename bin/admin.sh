#!/bin/bash

if [ $# -lt 1 ];
then
	echo usage $0 "start or stop"
	exit 1
fi

curdir=`dirname $0`
cd $curdir

if [ $1 = "start" ]
then
	pidnum=`ps -ef|grep "\./spider-server"|grep -v grep|wc -l`
	if [ $pidnum -lt 1 ]
	then
        ./spider-server 8888
	else
		for pid in `ps -ef|grep "./spider-server"|grep -v grep|awk '{print $2}'`
		do
			target_exe=`readlink /proc/$pid/exe | awk '{print $1}'`
			if [ -n "$target_exe" ]
			then
				local_exe=`pwd`"/spider-server"
				if [ $target_exe == $local_exe ]
				then
					echo "program already started."
					exit
				else
        			./spider-server 8888
				fi
			else
        			./spider-server 8888
				break
			fi	
		done
	fi
fi

if [ $1 = "stop" ]
then
	pidnum=`ps -ef|grep "./spider-server"|grep -v grep|wc -l`
	if [ $pidnum -lt 1 ]
	then
		echo "no program killed."
	else
		for pid in `ps -ef|grep "./spider-server"|grep -v grep|awk '{print $2}'`
		do
			target_exe=`readlink /proc/$pid/exe | awk '{print $1}'`
			if [ -n "$target_exe" ]
			then
				local_exe=`pwd`"/spider-server"
				if [ $target_exe == $local_exe ]
				then
					kill -10 $pid
				fi
			fi
		done
		echo "spider-server stopped."
	fi
fi

