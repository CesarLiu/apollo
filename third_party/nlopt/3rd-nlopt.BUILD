load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

licenses(["notice"])

cc_library(
    name = "nlopt",
    includes = ["include"],
    hdrs = glob(["include/**/*"]),
    linkopts = [
        "-lnlopt",
    ],
    strip_include_prefix = "include",
)
