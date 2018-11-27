#!/bin/sh
cd ~/Uni/COMP371CG/project/build
cmake .. &&
make &&
cd ~/Uni/COMP371CG/project/bin/test/ &&
./$1
