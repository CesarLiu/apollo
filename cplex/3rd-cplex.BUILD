load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

licenses(["notice"])

cc_library(
    name = "cplex",
    srcs = [
        "lib/x86-64_linux/static_pic/libopl.a",
        "lib/x86-64_linux/static_pic/libiljs.a",
        "lib/x86-64_linux/static_pic/libilocplex.a",
        "lib/x86-64_linux/static_pic/libcp.a",
        "lib/x86-64_linux/static_pic/libconcert.a",
        "bin/x86-64_linux/libcplex12100.so",
        "bin/x86-64_linux/libicuuc.so",
        "bin/x86-64_linux/libicui18n.so",
        "bin/x86-64_linux/libicuio.so",
        "bin/x86-64_linux/libicudata.so",
        "bin/x86-64_linux/liboplnl1.so",
        "bin/x86-64_linux/libicuuc.so.55",
        "bin/x86-64_linux/libicui18n.so.55",
        "bin/x86-64_linux/libicuio.so.55",
        "bin/x86-64_linux/libicudata.so.55",
        "bin/x86-64_linux/liboplnl1.so.12",
    ],
    visibility = ["//visibility:public"],
)
