FROM debian:latest
RUN apt update
RUN apt install -y make
RUN apt install -y gcc
RUN apt install -y valgrind
RUN apt install -y bash
COPY docker_run.sh /usr/local/bin/docker_run.sh
RUN chmod "+x" /usr/local/bin/docker_run.sh
ENTRYPOINT ["docker_run.sh"]
