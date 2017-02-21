#!/bin/bash
# -*- ENCODING: UTF-8 -*-


./xcircular <dist/uniform_524288.txt >>pruebas/slc/hilo_circularX.txt
./xcircular <dist/uniform_1048576.txt >>pruebas/slc/hilo_circularX.txt
./xcircular <dist/uniform_2097152.txt >>pruebas/slc/hilo_circularX.txt
./xcircular <dist/uniform_4194304.txt >>pruebas/slc/hilo_circularX.txt 
./xcircular <dist/uniform_8388608.txt >>pruebas/slc/hilo_circularX.txt 
./xcircular <dist/uniform_16777216.txt >>pruebas/slc/hilo_circularX.txt
./xcircular <dist/uniform_33554432.txt >>pruebas/slc/hilo_circularX.txt
./xcircular <dist/uniform_67108864.txt >>pruebas/slc/hilo_circularX.txt
./xcircular <dist/uniform_134217728.txt >>pruebas/slc/hilo_circularX.txt

exit