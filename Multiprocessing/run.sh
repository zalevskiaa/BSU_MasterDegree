# 1 - src_path
# 2 - num_processes
# example usage: ./run.sh hw/1 4

echo $0 $1 $2

declare src_path=$1
declare build_path="bash-build/$src_path"
declare -i num_processes=1

if [ $2 ]
then
    num_processes=$2
fi

mkdir -p "$build_path"

mpic++ "$src_path/main.cpp" -o "$build_path/main"
mpiexec -n $num_processes "$build_path/main"