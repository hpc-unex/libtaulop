#!/bin/bash

# Options
channels=("IB" "TCP")
sizes=(16384 131072 262144 524288 1048576 2097152)
mappings=(Sequential RoundRobin)
# Metropolis platform
P=32
Q=8
M=4
S=4096
op=sum

# Iterate over the channels and sizes
for chn in "${channels[@]}"; do

   for map in "${mappings[@]}"; do
   		
   	for size in "${sizes[@]}"; do
   
      	echo -n "suara3D_grid: chn=$chn map=$map m=$size ms=$S P=$P Q=$Q M=$M -> "

      	./suara3d_grid -P $P -Q $Q -M $M -m $size -s $S -c $chn --mapping $map -operation $op 2>/dev/null

		done
   done
done
