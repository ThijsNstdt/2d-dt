#!/bin/bash
#SBATCH --partition=hefstud
#SBATCH --output=std_%A_%a.txt
#SBATCH --mem=300M
#SBATCH --time=168:00:00
#SBATCH --array=0-11%12
cd ~/Thesis/2d-dt || exit
./cmake-build-release/bin/2d_dt ./config/config.json repeat $SLURM_ARRAY_TASK_ID