FROM capybara1/compiler-dev AS build
RUN apt-get install -y nano git pkg-config uuid-dev \
 && git config --global user.email "john.doe@example.com" \
 && git config --global user.name "John Doe"
COPY ctx /root/CompilerDemo/
WORKDIR /root/CompilerDemo/build
ENTRYPOINT ["/usr/bin/bash"]
