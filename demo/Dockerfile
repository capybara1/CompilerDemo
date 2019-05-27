FROM capybara1/compiler-dev:latest AS build
RUN apt-get install -y gdb \
 && apt-get clean
COPY ctx /root/CompilerDemo/
WORKDIR /root/CompilerDemo/build
ENTRYPOINT ["/bin/bash"]
