load(
    "//bazel:coevent_build_system.bzl",
    "coevent_package",
    "coevent_cc_binary",
)

coevent_package()

coevent_cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        "//:coevent_lib",
    ],
)

coevent_cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        "//:coevent_lib",
    ],
)

sh_test(
    name = "echo_test",
    srcs = [
        "echo_test.sh",
    ],
    data = [
        ":server",
        ":client",
    ],
)
