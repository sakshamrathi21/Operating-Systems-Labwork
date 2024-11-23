FROM ubuntu:20.04

ENV TZ=Asia/Kolkata

RUN apt update && apt upgrade -y 

ENV DEBIAN_FRONTEND=noninteractive
RUN apt install build-essential qemu-system -y

CMD ["/bin/bash"]
