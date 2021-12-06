#!/bin/sh
bison --version | head -n 1 | awk '{print $NF}'