load(
    "//bazel:coevent_build_system.bzl",
    "coevent_cc_library",
    "coevent_package",
)

coevent_package()

coevent_cc_library(
    name = "coevent_lib",
    hdrs = glob(["include/coevent/*.h"]),
    srcs = glob(["src/*.cc", "src/*.h"]),
    strip_include_prefix = "include",
    external_deps = [
        "@com_github_libevent_libevent//:libevent",
    ],
)

