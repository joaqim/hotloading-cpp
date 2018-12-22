FROM ubuntu:16.04

MAINTAINER Joaqim Planstedt "joaqimpla@gmail.com"

# USAGE: docker run -it -v ~/raspberry/hello:/build mitchallen/pi-cross-compile

#LABEL com.mitchallen.pi-cross-compile="{\"Description\":\"Cross Compile for Raspberry Pi\",\"Usage\":\"docker run -it -v ~/myprojects/mybuild:/build mitchallen/pi-cross-compile\",\"Version\":\"0.1.0\"}"

RUN apt-get update && apt-get install -y apt-utils git && apt-get install -y build-essential && apt-get install -y cmake
_

RUN git clone --progress --verbose https://github.com/raspberrypi/tools.git --depth=1 pitools

ENV BUILD_FOLDER /build

WORKDIR ${BUILD_FOLDER}

ADD build.sh /build.sh
RUN chmod +x /build.sh

CMD ["/bin/bash"]
ENTRYPOINT ["build.sh"]
# CMD ["make", "clean"]
