FROM ubuntu:jammy

RUN apt-get update && \
    apt-get -y dist-upgrade && \
    apt-get -y install gcc && \
    apt-get -y install make && \
    apt-get -y install openssh-server sudo

RUN service ssh start

# Disable ASLR
# RUN echo 0 | tee /proc/sys/kernel/randomize_va_space

WORKDIR /vuln

COPY . .

RUN make
RUN chmod 777 target
RUN chown 0 target

CMD ["./target"]
