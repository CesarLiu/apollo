load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "vehicle_controller_base",
    hdrs = ["vehicle_controller.h"],
    copts = ["-DMODULE_NAME=\\\"canbus\\\""],
    deps = [
        "//modules/canbus/proto:canbus_conf_cc_proto",
        "//modules/common/configs:vehicle_config_helper",
        "//modules/common_msgs/basic_msgs:error_code_cc_proto",
        "//modules/control/proto:control_conf_cc_proto",
        "//modules/drivers/canbus/can_client",
        "//modules/drivers/canbus/can_comm:can_sender",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "abstract_vehicle_factory",
    srcs = ["abstract_vehicle_factory.cc"],
    hdrs = ["abstract_vehicle_factory.h"],
    copts = ["-DMODULE_NAME=\\\"canbus\\\""],
    deps = [
        "//modules/canbus/proto:canbus_conf_cc_proto",
        "//modules/canbus/vehicle:vehicle_controller_base",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
        "//modules/common_msgs/basic_msgs:error_code_cc_proto",
        "//modules/common_msgs/control_msgs:control_cmd_cc_proto",
        "//modules/common_msgs/control_msgs:input_debug_cc_proto",
    ],
)

# cc_library(
#     name = "vehicle_factory",
#     srcs = ["vehicle_factory.cc"],
#     hdrs = ["vehicle_factory.h"],
#     copts = ["-DMODULE_NAME=\\\"canbus\\\""],
#     deps = [
#         "//modules/canbus_vehicle/ch:ch_vehicle_factory",
#         "//modules/canbus_vehicle/devkit:devkit_vehicle_factory",
#         "//modules/canbus_vehicle/ge3:ge3_vehicle_factory",
#         "//modules/canbus_vehicle/gem:gem_vehicle_factory",
#         "//modules/canbus_vehicle/lexus:lexus_vehicle_factory",
#         "//modules/canbus_vehicle/lincoln:lincoln_vehicle_factory",
#         "//modules/canbus_vehicle/transit:transit_vehicle_factory",
#         "//modules/canbus_vehicle/wey:wey_vehicle_factory",
#         "//modules/canbus_vehicle/zhongyun:zhongyun_vehicle_factory",
#         "//modules/canbus_vehicle/neolix_edu:neolix_edu_vehicle_factory",
#         "//modules/canbus_vehicle/fortuna:fortuna_vehicle_factory",
#         "//modules/common/util:factory",
#     ],
# )

# cc_test(
#     name = "vehicle_factory_test",
#     size = "small",
#     srcs = ["vehicle_factory_test.cc"],
#     deps = [
#         ":vehicle_factory",
#         "@com_google_googletest//:gtest",
#     ],
# )

cpplint()
