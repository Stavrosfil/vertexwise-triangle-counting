#!/bin/bash

#SBATCH --job-name=pds_project
#SBATCH --partition=batch
#SBATCH --ntasks=20
#SBATCH --time=00:10:00

# thread_nums=(4 8)
thread_nums=(1 2 4 8 16 20)
datasets=("belgium_osm.mtx" "com-Youtube.mtx" "mycielskian13.mtx" "dblp-2010.mtx" "NACA0015.mtx")

data_path="./data/"
datasets_prefixed="${datasets[@]/#/$data_path}"

make_options=("serial" "openmp" "cilk" "pthreads")

mkdir -p results
echo "dataset,version,library,threads,time" >>./results/results.csv

sweep_files_threads() {
    for d in ${datasets_prefixed[@]}; do
        printf "\n------------------ %25s -------------\n" "$d"
        for t in ${thread_nums[@]}; do
            export CILK_NWORKERS=$t
            export OMP_NUM_THREADS=$t
            printf "%50s threads\n" "$t"
            ./build/main $d $t
        done
    done
}

run_library() {
    for i in 1 2 3; do
        echo "#define FRAMEWORK ${i}" >./include/framework.h
        printf "\n==============================================================\n"
        printf "\n[Framework] %s" "${make_options[$i]}"

        if [ ${make_options[$i]} == "cilk" ]; then
            module purge gcc
            module load OpenCilk
        else
            module purge OpenCilk
            module load gcc
        fi

        make ${make_options[$i]}

        for i in $(seq 1 5); do
            sweep_files_threads
        done
    done
}

run_library
