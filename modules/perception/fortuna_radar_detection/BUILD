load("//tools:apollo_package.bzl", "apollo_cc_binary", "apollo_cc_library", "apollo_cc_test", "apollo_component", "apollo_package")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "fortuna_radar_detection_files",
    srcs = glob([
        "conf/**",
        "dag/**",
        "data/**",
        "launch/**",
    ]),
)

apollo_component(
    name = "libfortuna_radar_detection.so",
    srcs = [
        "fortuna_radar_detection_component.cc",
    ],
    hdrs = [
        "fortuna_radar_detection_component.h",
    ],
    deps = [
        "//cyber",
        "//modules/common/util:util_tool",
        "//modules/perception/common/algorithm:apollo_perception_common_algorithm",
        "//modules/perception/common/base:apollo_perception_common_base",
        "//modules/perception/common/hdmap:apollo_perception_common_hdmap",
        "//modules/perception/common/lib:apollo_perception_common_lib",
        "//modules/perception/common/onboard:apollo_perception_common_onboard",
        "//modules/perception/fortuna_radar_detection/proto:fortuna_radar_component_config_cc_proto",
        "//modules/canbus_vehicle/fortuna/proto:fortuna_cc_proto",
        "//modules/perception/radar_detection:apollo_perception_radar_detection",
    ],
)

apollo_package()

cpplint()