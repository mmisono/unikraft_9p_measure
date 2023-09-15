# vim: set ft=make et :

default:
    @just --choose

run:
    sudo taskset -c 0,1 qemu-system-x86_64 \
        -cpu host \
        -enable-kvm \
        -m 1G \
        -fsdev local,id=myid,path=$(pwd)/fs0,security_model=none \
        -device virtio-9p-pci,fsdev=myid,mount_tag=fs0,disable-modern=on,disable-legacy=off \
        -kernel build/unikraft_9p_measure_kvm-x86_64 -nographic

build-linux:
    mkdir -p fs_linux
    gcc -Wall \
        -O2 \
        src/main.c \
        src/helper_functions.c \
        src/measurement_scenarios.c \
        src/scenario_runners.c \
        src/time_functions.c \
        -lm -o fs_measure
