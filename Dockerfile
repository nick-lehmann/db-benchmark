FROM alpine:3.15

RUN apk add --no-cache alpine-sdk cmake

WORKDIR /app

COPY . .

RUN cmake -B build
RUN cmake --build build 
