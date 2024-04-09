#!/bin/bash
#SBATCH --partition=hefstud
#SBATCH -w cn112
#SBATCH --output=std_%A_%a.txt
#SBATCH --mem=300M
#SBATCH --time=96:00:00
#SBATCH --array=0-2%3
cd ~/Thesis/2d-dt || exit
./cmake-build-release/bin/2d_dt ./config/config.json updating ensemble $SLURM_ARRAY_TASK_ID