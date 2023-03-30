#!/bin/bash 

cd ..
for x in */; 
do
	if [ -d $x ] 
	then
		echo "touch $x/CATKIN_IGNORE"
		$(touch $x\CATKIN_IGNORE)
	fi
done

rm -rf ../devel ../build
