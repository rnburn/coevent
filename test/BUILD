load(
    "//bazel:coevent_build_system.bzl",
    "coevent_catch_test",
    "coevent_package",
)

coevent_package()

coevent_catch_test(
    name = "endpoint_test",
    srcs = [
        "endpoint_test.cc",
    ],
)

coevent_catch_test(
    name = "detached_task_test",
    srcs = [
        "detached_task_test.cc",
    ],
)

coevent_catch_test(
    name = "task_test",
    srcs = [
        "task_test.cc",
    ],
)

coevent_catch_test(
    name = "sleep_test",
    srcs = [
        "sleep_test.cc",
    ],
)
