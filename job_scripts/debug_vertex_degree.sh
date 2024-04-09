#!/bin/bash
#SBATCH --partition=hefstud
#SBATCH --output=std_%A_%a.txt
#SBATCH --mem=300M
#SBATCH --time=1:00:00
cd ~/Thesis/2d-dt || exit
./cmake-build-debug/bin/2d_dt ./config/config.json initialization size 0