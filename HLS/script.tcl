############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project srelease
set_top spmv
add_files HLS/adder.cpp
add_files HLS/adder.hpp
add_files HLS/control.cpp
add_files HLS/control.hpp
add_files HLS/core.cpp
add_files HLS/core.hpp
add_files HLS/merger.hpp
add_files HLS/pe.hpp
add_files HLS/util.hpp
open_solution "baseline"
set_part {xczu7ev-ffvc1156-2-e} -tool vivado
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog -version 1.0.0
source "./srelease/baseline/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -rtl verilog -format ip_catalog -version "1.0.0"
