# SpMV_ICCAD

Shield: [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg

The code for our ICCAD work "Optimized Data Reuse via Reordering for Sparse Matrix-Vector Multiplication on FPGAs"

## Experimental Setup
We evaluate the design on Xilinx ZCU106 under 100MHz and the Vivado toolchain's version is 2018.3.

## HLS
The folder contains the HLS code and script.

## Preprocessing
The folder contains the Python code of preprocessing. The output binary files which contains the sparse matrix elements can be found in "Preprocessing/bino/{benchmark}" and then you can copy the files into your SD card.

## Vivado_SDK
The folder contains the tcl file of Vivado block design and the testbench of Vivado SDK. Note that you need to change some values in the testbench following the README.md in the folder.
