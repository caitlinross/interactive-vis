#!/bin/bash

for batch in 2 4 8 16 32 64 128
do
    mpirun -np 8 ./model-net-synthetic --synch=3 --extramem=32000 --gvt-interval=1024 --batch=$batch --stats-filename=dragonfly-batch/batch$batch -- ~/codes/src/network-workloads/conf/modelnet-synthetic-dragonfly.conf
done

for gvt in 16 32 64 128 256 512 1024 2048 4096
do
    mpirun -np 8 ./model-net-synthetic --synch=3 --extramem=32000 --gvt-interval=$gvt --batch=2 --stats-filename=dragonfly-gvt/gvt$gvt -- ~/codes/src/network-workloads/conf/modelnet-synthetic-dragonfly.conf
done
