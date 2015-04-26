#!/bin/bash

if [ $# -lt 2 ];
then
	echo usage $0 "start or stop"
	exit 1

fi

option=$1

if [ $option = "start" ]


