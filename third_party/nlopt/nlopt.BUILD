load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

licenses(["notice"])

cc_library(
    name = "nlopt",
    hdrs = ["include/nlopt.h", "include/nlopt.hpp"], 
    includes = ["."],
    linkopts = [
      "-lnlopt",
    ],
)
