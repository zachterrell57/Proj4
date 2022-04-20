#!/bin/bash

file_length=$(< $1 wc -l)
./build/analysis $1 $file_length