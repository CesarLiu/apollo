load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools/install:install.bzl", "install", "install_files", "install_src_files")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

CANBUS_COPTS = ["-DMODULE_NAME=\\\"canbus\\\""]

cc_library(
    name = "fortuna_vehicle_factory",
    srcs = [
        "fortuna_vehicle_factory.cc",
    ],
    hdrs = [
        "fortuna_vehicle_factory.h",
    ],
    alwayslink = True,
    copts = CANBUS_COPTS,
    deps = [
        ":fortuna_controller",
        ":fortuna_message_manager",
        "//modules/canbus/common:canbus_gflags",
        "//modules/common/adapters:adapter_gflags",
        "//modules/common/status",
        "//modules/canbus/vehicle:abstract_vehicle_factory",
        "//modules/drivers/canbus:sensor_canbus_lib",
    ],
)

cc_library(
    name = "fortuna_message_manager",
    srcs = [
        "fortuna_message_manager.cc",
    ],
    hdrs = [
        "fortuna_message_manager.h",
    ],
    copts = CANBUS_COPTS,
    deps = [
        "//modules/canbus/proto:canbus_proto",
        "//modules/canbus_vehicle/fortuna/proto:fortiss_hardware_status_cc_proto",
        "//modules/canbus_vehicle/fortuna/protocol:canbus_fortuna_protocol",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "fortuna_controller",
    srcs = [
        "fortuna_controller.cc",
    ],
    hdrs = [
        "fortuna_controller.h",
    ],
    copts = CANBUS_COPTS,
    deps = [
        ":fortuna_message_manager",
        "//modules/canbus/proto:canbus_conf_cc_proto",
        "//modules/common_msgs/chassis_msgs:chassis_cc_proto",
        "//modules/canbus/proto:vehicle_parameter_cc_proto",
        "//modules/canbus/vehicle:vehicle_controller_base",
        "//modules/canbus_vehicle/fortuna/protocol:canbus_fortuna_protocol",
        "//modules/common_msgs/basic_msgs:error_code_cc_proto",
        "//modules/common_msgs/control_msgs:control_cmd_cc_proto",
        "//modules/canbus_vehicle/fortuna/protocol:canbus_fortuna_protocol",
        "//modules/common/kv_db",
    ],
)

cc_test(
   name = "fortuna_controller_test",
   size = "small",
   srcs = [
       "fortuna_controller_test.cc",
   ],
   data = ["//modules/canbus:canbus_testdata"],
   deps = [
       "fortuna_controller",
       "@com_google_googletest//:gtest_main",
   ],
)

# cc_test(
#     name = "fortuna_message_manager_test",
#     size = "small",
#     srcs = [
#         "fortuna_message_manager_test.cc",
#     ],
#     deps = [
#         "//modules/canbus_vehicle/fortuna:fortuna_message_manager",
#         "@gtest//:main",
#     ],
# )

cc_test(
   name = "fortuna_vehicle_factory_test",
   size = "small",
   srcs = ["fortuna_vehicle_factory_test.cc"],
   deps = [
       ":fortuna_vehicle_factory",
       "@com_google_googletest//:gtest_main",
   ],
)
install(
    name = "install",
    library_dest = "canbus-vehicle-fortuna/lib",
    data_dest = "canbus-vehicle-fortuna",
    data = [
        ":runtime_data",
        ":cyberfile.xml",
        ":canbus-vehicle-fortuna.BUILD",
    ],
    targets = [
        ":libfortuna_vehicle_factory_lib.so",
    ],
    deps = [
        ":pb_fortuna",
        ":pb_hdrs",
    ],
)

install(
    name = "pb_hdrs",
    data_dest = "canbus-vehicle-fortuna/include",
    data = [
        "//modules/canbus_vehicle/fortuna/proto:fortiss_hardware_status_cc_proto",
    ],
)

install_files(
    name = "pb_fortuna",
    dest = "canbus-vehicle-fortuna",
    files = [
        "//modules/canbus_vehicle/fortuna/proto:fortiss_hardware_status_py_pb2",
    ],
)

filegroup(
    name = "runtime_data",
    srcs = glob([
        "testdata/**",
    ]),
)

install_src_files(
    name = "install_src",
    deps = [
        ":install_fortuna_src",
        ":install_fortuna_hdrs"
    ],
)

install_src_files(
    name = "install_fortuna_src",
    src_dir = ["."],
    dest = "canbus-vehicle-fortuna/src",
    filter = "*",
)

install_src_files(
    name = "install_fortuna_hdrs",
    src_dir = ["."],
    dest = "canbus-vehicle-fortuna/include",
    filter = "*.h",
)
cpplint()
