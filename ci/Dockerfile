FROM rnburn/clang

WORKDIR /3rd_party

ADD setup_build_environment.sh /3rd_party
ADD install_bazel.sh /3rd_party
ADD setup_bazel.sh /3rd_party

RUN /3rd_party/setup_build_environment.sh \
 && /3rd_party/install_bazel.sh \
 && /3rd_party/setup_bazel.sh
