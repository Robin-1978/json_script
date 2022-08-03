load("@rules_cc//cc:defs.bzl", "cc_library" , "cc_binary")

cc_library(
    name = "vm",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    srcs = glob(["src/*.cc"]),
    deps = ["@fmt//:fmt"],
    copts = ["--std=c++17"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "cvm",
    srcs = ["main.cc"],
    copts = ["--std=c++17"],
    deps = [":vm"],
)