load(
    "//bazel:coevent_build_system.bzl",
    "coevent_package",
    "coevent_cc_binary",
)

coevent_package()

coevent_cc_binary(
    name = "hello",
    srcs = ["main.cc"],
    deps = [
        "//include/coevent:task_lib",
    ],
)
