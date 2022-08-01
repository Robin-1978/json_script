load("@rules_cc//cc:defs.bzl", "cc_library" , "cc_binary")

cc_library(
    name = "vm",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    srcs = glob(["src/*.cc"]),

    visibility = ["//visibility:public"],
)

cc_binary(
    name = "cvm",
    srcs = ["main.cc"],
    deps = ["//vm"],
)