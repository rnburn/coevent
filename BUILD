load(
    "//bazel:coevent_build_system.bzl",
    "coevent_cc_library",
    "coevent_package",
)

coevent_package()

coevent_cc_library(
    name = "coevent_lib",
    hdrs = glob(["include/coevent/*.h"]),
    srcs = glob(["src/*.cc", "src/coevent/*.h"]),
    strip_include_prefix = "include",
)

