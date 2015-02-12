#!/bin/bash

for i in `find res/input/lubm-40/ -type f`; do
  echo $i >> lubm40_experiment_result
  { time ./qrepair -sat $i | ./MiniSat_v1.14_linux;} 2>> lubmu40_experiment_result
done
