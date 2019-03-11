load(
    "//bazel:coevent_build_system.bzl",
    "coevent_cc_library",
    "coevent_package",
)

coevent_package()

# From commit a575536abe20b58c48863600f8a71e93b4052b81
coevent_cc_library(
    name = "catch2",
    hdrs = [
        "catch.hpp",
    ],
    is_3rd_party = True,
)

coevent_cc_library(
    name = "main_lib",
    srcs = [
        "catch_main.cpp",
    ],
    deps = [
        ":catch2",
    ],
    is_3rd_party = True,
)
