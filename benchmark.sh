#!/bin/bash

#SBATCH --job-name=pds_project
#SBATCH --partition=batch
#SBATCH --ntasks=20
#SBATCH --time=00:10:00

# module load OpenCilk

# thread_nums=(1 2 4 8 16 20)
thread_nums=(4 8)
# datasets=("belgium_osm.mtx" "com-Youtube.mtx" "mycielskian13.mtx" "dblp-2010.mtx" "NACA0015.mtx")
datasets=("belgium_osm.mtx" "dblp-2010.mtx")
data_path="./data/"
# data_path="/mnt/apps/prebuilt/pdlab/data"

datasets_prefixed="${datasets[@]/#/$data_path}"

sweep_files_threads() {
    for d in ${datasets_prefixed[@]}; do
        printf "\n------------------ %25s -------------\n" "$d"
        for t in ${thread_nums[@]}; do
            export CILK_NWORKERS=$t
            printf "%50s threads\n" "$t"
            ./build/main $d $t
        done
    done
}

make_options=("serial" "openmp" "cilk" "pthreads")

for i in ${!make_options[@]}; do
    export FRAMEWORK=$i
    printf "\n==============================================================\n"
    printf "\n[Framework] %s" "${make_options[$i]}"
    make ${make_options[$i]}
    sweep_files_threads
done
