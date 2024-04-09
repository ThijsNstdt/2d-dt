#!/bin/bash
#SBATCH --partition=hefstud
#SBATCH --output=std_%A_%a.txt
#SBATCH --mem=300M
#SBATCH --time=4:00:00
#SBATCH --array=0-3%3
cd ~/Thesis/2d-dt || exit
./cmake-build-release/bin/2d_dt ./config/config.json initialization size $SLURM_ARRAY_TASK_ID