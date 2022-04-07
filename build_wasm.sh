#! /bin/sh

cd /libieml/build && \
    emcmake cmake -DBUILD_WASM:bool=True .. &&\
    emmake make -j $NB_CORES

ls /libieml/build/binding/wasm/