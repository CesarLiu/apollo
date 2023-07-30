load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//tools/install:install.bzl", "install", "install_files", "install_src_files")
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

install(
    name = "install",
    library_dest = "autobox_bridge/lib",
    data_dest = "autobaox_bridge",
    data = [
      ":runtime_data",
      "cyberfile.xml",
      "autobox_bridge.BUILD"
    ],
    targets = [
        ":libautobox_bridge_component_chassis.so",
        ":libautobox_bridge_component_control.so",
        ":libautobox_bridge_component_localization.so",
        ":libautobox_bridge_component_trajectory.so",
    ],
    deps = [
        "//modules/autobox_bridge/proto:autobox_bridge_conf_proto",
        "//modules/autobox_bridge/proto:autobox_chassis_proto",
        "//modules/autobox_bridge/proto:autobox_control_proto",
        "//modules/autobox_bridge/proto:autobox_localization_proto",
        "//modules/autobox_bridge/proto:autobox_trajectory_proto",
    ],
)
install(
    name = "pb_hdrs",
    data_dest = "autobox_bridge/include",
    data = [
        "//modules/autobox_bridge/proto:autobox_bridge_conf_cc_proto",
        "//modules/autobox_bridge/proto:autobox_chassis_cc_proto",
        "//modules/autobox_bridge/proto:autobox_control_cc_proto",
        "//modules/autobox_bridge/proto:autobox_localization_cc_proto",
        "//modules/autobox_bridge/proto:autobox_trajectory_cc_proto",
    ],
)

cc_binary(
    name = "libautobox_bridge_component_chassis.so",
    linkopts = ["-shared"],
    linkstatic = False,
    deps = [":autobox_bridge_component_lib_chassis"],
)

cc_library(
    name = "autobox_bridge_component_lib_chassis",
    srcs = ["autobox_bridge_component_chassis.cc"],
    hdrs = ["autobox_bridge_component_chassis.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/control_msgs:control_cmd_proto",
        "//modules/autobox_bridge/proto:autobox_chassis_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_proto",
    ],
)

cc_binary(
    name = "libautobox_bridge_component_control.so",
    linkopts = ["-shared"],
    linkstatic = False,
    deps = [":autobox_bridge_component_lib_control"],
)

cc_library(
    name = "autobox_bridge_component_lib_control",
    srcs = ["autobox_bridge_component_control.cc"],
    hdrs = ["autobox_bridge_component_control.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/control_msgs:control_cmd_proto",
	    "//modules/autobox_bridge/proto:autobox_control_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_proto",
    ],
)

cc_binary(
    name = "libautobox_bridge_component_localization.so",
    linkopts = ["-shared"],
    linkstatic = False,
    deps = [":autobox_bridge_component_lib_localization"],
)

cc_library(
    name = "autobox_bridge_component_lib_localization",
    srcs = ["autobox_bridge_component_localization.cc"],
    hdrs = ["autobox_bridge_component_localization.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/localization_msgs:localization_proto",
	    "//modules/autobox_bridge/proto:autobox_localization_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_proto",
    ],
)

cc_binary(
    name = "libautobox_bridge_component_trajectory.so",
    linkopts = ["-shared"],
    linkstatic = False,
    deps = [":autobox_bridge_component_lib_trajectory"],
)

cc_library(
    name = "autobox_bridge_component_lib_trajectory",
    srcs = ["autobox_bridge_component_trajectory.cc"],
    hdrs = ["autobox_bridge_component_trajectory.h"],
    copts = ['-DMODULE_NAME=\\"autobox_bridge\\"'],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common_msgs/control_msgs:control_cmd_proto",
        "//modules/common_msgs/planning_msgs:planning_cc_proto",
        "//modules/common_msgs/localization_msgs:localization_proto",
        "//modules/autobox_bridge/proto:autobox_trajectory_proto",
        "//modules/autobox_bridge/proto:autobox_bridge_conf_proto",
    ],
)

install_src_files(
    name = "install_src",
    deps = [
        ":install_autobox_bridge_src",
        ":install_autobox_bridge_hdrs"
    ],
)

install_src_files(
    name = "install_autobox_bridge_src",
    src_dir = ["."],
    dest = "bridge/src",
    filter = "*",
)

install_src_files(
    name = "install_autobox_bridge_hdrs",
    src_dir = ["."],
    dest = "bridge/include",
    filter = "*.h",
)

cpplint()
