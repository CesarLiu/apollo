load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("//tools:python_rules.bzl", "py_proto_library")
package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "fake_obstacle_cc_proto",
    deps = [
        ":fake_obstacle_proto",
    ],
)

proto_library(
    name = "fake_obstacle_proto",
    srcs = [
        "fake_obstacle_conf.proto",
    ],
    deps = [
        "//modules/common_msgs/perception_msgs:perception_obstacle_proto",
    ],
)
py_proto_library(
    name = "fake_obstacle_py_pb2",
    deps = [
        ":fake_obstacle_proto",
        "//modules/common_msgs/perception_msgs:perception_obstacle_py_pb2",
    ],
)