load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//tools/install:install.bzl", "install", "install_files", "install_src_files")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

install(
    name = "install",
    targets = ["libfake_obstacle_component.so"],
    library_dest = "fake_obstacle/lib",
    data_dest = "fake_obstacle",
    data = [
      "fake_obstacle_component_conf",
      "cyberfile.xml",
      "fake_obstacle.BUILD"
    ],
)

install(
    name = "pb_hdrs",
    data_dest = "fake_obstacle/include",
    data = [
        "//modules/fake_obstacle/proto:fake_obstacle_cc_proto",
    ],
)

cc_library(
    name = "fake_obstacle_component_lib",
    srcs = ["fake_obstacle_component.cc"],
    hdrs = ["fake_obstacle_component.h"],
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
        "//modules/map/hdmap:hdmap_util",
        "//modules/planning/reference_line:reference_line_provider",
    ],
)

cc_binary(
    name = "libfake_obstacle_component.so",
    linkshared = True,
    linkstatic = False,
    deps = [":fake_obstacle_component_lib"],
)

filegroup(
    name = "fake_obstacle_component_conf",
    srcs = glob([
        "conf/**",
    ]),
)

install_src_files(
    name = "install_src",
    deps = [
        ":install_fake_obstacle_src",
        ":install_fake_obstacle_hdrs"
    ],
)

install_src_files(
    name = "install_fake_obstacle_src",
    src_dir = ["."],
    dest = "fake_obstacle/src",
    filter = "*",
)

install_src_files(
    name = "install_fake_obstacle_hdrs",
    src_dir = ["."],
    dest = "fake_obstacle/include",
    filter = "*.h",
)

cpplint()
