load("//tools:apollo_package.bzl", "apollo_cc_binary", "apollo_cc_library", "apollo_cc_test", "apollo_component", "apollo_package")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

apollo_component(
    name = "libfake_obstacle_component.so",
    srcs = [
        "fake_obstacle_component.cc",
    ],
    hdrs = [
        "fake_obstacle_component.h",
    ],
    copts = [
        "-DMODULE_NAME=\\\"fake_obstacle\\\"",
    ],
    deps = [
        "//cyber",
        "//modules/common/monitor_log",
        "//modules/common/adapters:adapter_gflags",
        "//modules/common/util:message_util",
        "//modules/common_msgs/localization_msgs:localization_cc_proto",
        "//modules/fake_obstacle/proto:fake_obstacle_cc_proto",
        "//modules/common_msgs/routing_msgs:routing_cc_proto",
        "//modules/common_msgs/perception_msgs:perception_obstacle_cc_proto",
        "//modules/map:apollo_map",
        "//modules/planning/planning_base:apollo_planning_planning_base",
    ],
)



filegroup(
    name = "runtime_data",
    srcs = glob([
        "conf/*.txt",
        "dag/*.dag",
        "launch/*.launch",
    ]),
)

apollo_package()

cpplint()