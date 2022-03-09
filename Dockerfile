ARG NB_CORES=1
FROM alpine:3.15

WORKDIR /libieml
RUN apk update && apk add --no-cache pkgconfig libuuid openjdk11 cmake git g++ make util-linux-dev

COPY . /libieml/

RUN mkdir build && \
    cd build && \
    cmake -DBUILD_TEST:BOOL=TRUE ..&& \
    cmake --build . -j $NB_CORES

CMD ["/libieml/build/test/libieml_test"] git