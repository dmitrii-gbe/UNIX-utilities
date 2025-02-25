#!/bin/bash

docker build . -t test
docker run -v $(pwd):/tmp -it test
