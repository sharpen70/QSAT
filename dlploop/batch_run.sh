#!/bin/bash

for i in `find Benchmarks/ -type f`; do
	./run.sh $i
done
