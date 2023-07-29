load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("//tools:python_rules.bzl", "py_proto_library")
package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "autobox_bridge_conf_proto",
    deps = [
        ":autobox_bridge_conf_proto_lib",
    ],
)

proto_library(
    name = "autobox_bridge_conf_proto_lib",
    srcs = [
        "autobox_bridge_conf.proto"
    ],
)

# Delete the suff from here on if the repacking of protobuf messages is obsolete!

cc_proto_library(
    name = "autobox_trajectory_proto",
    deps = [
        ":autobox_trajectory_proto_lib",
    ],
)

proto_library(
    name = "autobox_trajectory_proto_lib",
    srcs = [
        "autobox_trajectory.proto",
    ],
    deps = [
        "//modules/common_msgs/basic_msgs:pnc_point_proto",
        "//modules/common_msgs/basic_msgs:header_proto",
    ],
)

cc_proto_library(
    name = "autobox_localization_proto",
    deps = [
        ":autobox_localization_proto_lib",
    ],
)

proto_library(
    name = "autobox_localization_proto_lib",
    srcs = [
        "autobox_localization.proto",
    ],
    deps = [
        "//modules/common_msgs/localization_msgs:pose_proto",
        "//modules/common_msgs/localization_msgs:localization_proto",
        "//modules/common_msgs/basic_msgs:header_proto",
    ],
)


cc_proto_library(
    name = "autobox_control_proto",
    deps = [
        ":autobox_control_proto_lib",
    ],
)

proto_library(
    name = "autobox_control_proto_lib",
    srcs = [
        "autobox_control.proto",
    ],
    deps = [
        "//modules/common_msgs/chassis_msgs:chassis_proto",
        "//modules/common_msgs/basic_msgs:header_proto",
    ],
)

cc_proto_library(
    name = "autobox_chassis_proto",
    deps = [
        ":autobox_chassis_proto_lib",
    ],
)

proto_library(
    name = "autobox_chassis_proto_lib",
    srcs = [
        "autobox_chassis.proto",
    ],
    deps = [
        "//modules/common_msgs/basic_msgs:header_proto",
    ],
)

