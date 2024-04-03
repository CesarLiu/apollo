load("//tools:apollo_package.bzl", "apollo_cc_binary", "apollo_cc_library", "apollo_cc_test", "apollo_component", "apollo_package")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "runtime_data",
    srcs = glob([
        "conf/*.txt",
        "dag/*.dag",
        "launch/*.launch",
    ]),
)

apollo_component(
    name = "libautobox_bridge_component_chassis.so",
    srcs = ["autobox_bridge_component_chassis.cc"],
    hdrs = ["autobox_bridge_component_chassis.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/control_msgs:control_cmd_cc_proto",
        "//modules/autobox_bridge/proto:autobox_chassis_cc_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_cc_proto",
    ],
)

apollo_component(
    name = "libautobox_bridge_component_control.so",
    srcs = ["autobox_bridge_component_control.cc"],
    hdrs = ["autobox_bridge_component_control.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/control_msgs:control_cmd_cc_proto",
	    "//modules/autobox_bridge/proto:autobox_control_cc_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_cc_proto",
    ],
)

apollo_component(
    name = "libautobox_bridge_component_localization.so",
    srcs = ["autobox_bridge_component_localization.cc"],
    hdrs = ["autobox_bridge_component_localization.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/localization_msgs:localization_cc_proto",
	    "//modules/autobox_bridge/proto:autobox_localization_cc_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_cc_proto",
    ],
)

apollo_component(
    name = "libautobox_bridge_component_trajectory.so",
    srcs = ["autobox_bridge_component_trajectory.cc"],
    hdrs = ["autobox_bridge_component_trajectory.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/control_msgs:control_cmd_cc_proto",
        "//modules/common_msgs/planning_msgs:planning_cc_proto",
        "//modules/common_msgs/localization_msgs:localization_cc_proto",
        "//modules/autobox_bridge/proto:autobox_trajectory_cc_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_cc_proto",
    ],
)

apollo_package()

cpplint()
