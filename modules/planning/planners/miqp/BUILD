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
    name = "libmiqp_planner.so",
    srcs = ["miqp_planner.cc"],
    hdrs = ["miqp_planner.h"],
    description = ":plugins.xml",
    copts = ["-DMODULE_NAME=\\\"planning\\\""],
    deps = [
        "//cyber",
        "//modules/common/math",
        "//modules/common/vehicle_state:vehicle_state_provider",
        "//modules/planning/planning_interface_base:apollo_planning_planning_interface_base",
        "//modules/planning/planning_base:apollo_planning_planning_base",
        "//modules/planning/planners/lattice:lattice_planner_lib",
        "//modules/common_msgs/planning_msgs:planning_cc_proto",
        "//modules/planning/planners/miqp/proto:planner_config_proto",
        "@miqp_planner//:miqp_c_api",
        "@cplex",
    ],
    data = ["@miqp_planner//:cplex_models_filegroup"],
)

apollo_package()
cpplint()
