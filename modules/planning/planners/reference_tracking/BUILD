load("//tools:cpplint.bzl", "cpplint")
load("//tools:apollo_package.bzl", "apollo_package", "apollo_plugin", "apollo_cc_test")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "runtime_files",
    srcs = glob([
        "conf/**",
    ])
)

apollo_plugin(
    name = "libreference_tracking_planner.so",
    srcs = ["reference_tracking_planner.cc"],
    hdrs = ["reference_tracking_planner.h"],
    description = ":plugins.xml",
    copts = ["-DMODULE_NAME=\\\"planning\\\""],
    deps = [
        "//cyber",
        "//modules/common/math",
        "//modules/common/vehicle_state:vehicle_state_provider",
        "//modules/planning/planning_interface_base:apollo_planning_planning_interface_base",
        "//modules/planning/planning_base:apollo_planning_planning_base",
        "//modules/planning/planners/lattice:liblattice_planner.so",
        "//modules/planning/planners/bark_rl_wrapper/proto:planner_config_cc_proto",
        "//modules/common_msgs/planning_msgs:planning_cc_proto",
    ],
)

apollo_package()
cpplint()
