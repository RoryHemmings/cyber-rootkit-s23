FROM ubuntu:jammy

RUN apt-get update && \
    apt-get -y dist-upgrade && \
    apt-get -y install gcc && \
    apt-get -y install make && \
    apt-get -y install rsyslog && \
    apt-get -y install vim && \
    apt-get -y install python3 && \
    apt-get -y install python3-pip && \
    apt-get -y install apache2 sudo && \
    apt-get -y install openssh-server sudo

RUN pip install pwn

# Disable ASLR (nvm do this in host machine)
# RUN echo 0 | tee /proc/sys/kernel/randomize_va_space

WORKDIR /vuln
COPY . .

# Configure Static Site
RUN cp ./site/* /var/www/html/

RUN make target
RUN chmod 777 target
RUN chown 0 target

# Expose port for rev shell
EXPOSE 2002

CMD ["./start.sh"]
