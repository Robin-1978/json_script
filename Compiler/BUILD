load("@rules_cc//cc:defs.bzl", "cc_library" , "cc_binary")

cc_library(
    name = "compiler",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    deps = ["@fmt//:fmt", "@magic_enum//:magic_enum", "@lexy//:lexy"],
    copts = ["--std=c++17"],
    linkstatic = True,
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "jsc",
    srcs = ["jsc.cc"],
    copts = ["--std=c++17"],
    deps = [":compiler"],
)