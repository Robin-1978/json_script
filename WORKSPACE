workspace(name = "json_script")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_cc",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/0.0.2.tar.gz"],
    sha256 = "0e5801b1834a51c1308044e9abaeb6aaf945e4a4274866ea84fbccc50a292368",
    strip_prefix = "rules_cc-0.0.2",
)
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies", "rules_cc_toolchains")
rules_cc_dependencies()
rules_cc_toolchains()

http_archive(
    name = "rules_foreign_cc",
    sha256 = "6041f1374ff32ba711564374ad8e007aef77f71561a7ce784123b9b4b88614fc",
    strip_prefix = "rules_foreign_cc-0.8.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.8.0.tar.gz",
)
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

# fmt
http_archive(
    name = "fmt",
    build_file = "//build_file:fmt.BUILD",
    sha256 = "9a1e0e9e843a356d65c7604e2c8bf9402b50fe294c355de0095ebd42fb9bd2c5",
    strip_prefix = "fmt-9.0.0",
    urls = ["https://github.com/fmtlib/fmt/archive/9.0.0.tar.gz"],
) 