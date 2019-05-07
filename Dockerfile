FROM archlinux/base

RUN pacman -Syyu --noconfirm \
  clang llvm python2 rapidjson \
  git gtest make && \
  pacman -Sc

RUN git clone https://github.com/JasonL9000/ib.git /opt/ib
RUN chmod +x /opt/ib/ib
RUN ln -s /opt/ib/ib /usr/local/bin/ib

RUN mkdir -p /usr/bin
RUN ln -s /usr/bin/python2 /usr/bin/python
ADD . /opt/yourhtml
WORKDIR /opt/yourhtml

RUN ./scripts/test.sh && ./scripts/build.sh
