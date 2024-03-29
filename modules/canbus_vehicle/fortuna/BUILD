load("//tools:apollo_package.bzl", "apollo_cc_binary", "apollo_cc_library", "apollo_cc_test", "apollo_component", "apollo_package")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

CANBUS_COPTS = ["-DMODULE_NAME=\\\"canbus\\\""]

apollo_cc_library(
    name = "apollo_canbus_vehicle_fortuna",
    srcs = [
        "fortuna_controller.cc",
        "fortuna_message_manager.cc",
        "fortuna_vehicle_factory.cc",
        "protocol/assistance_2_rpt_0xe.cc",
        "protocol/motion_rpt_0x01.cc",
        "protocol/powertrain_1_rpt_0x04.cc",
        "protocol/front_object_1_rpt_0x31.cc",
        "protocol/front_object_2_rpt_0x32.cc",
        "protocol/front_object_3_rpt_0x33.cc",
        "protocol/front_object_4_rpt_0x34.cc",
        "protocol/rear_object_1_rpt_0x41.cc",
        "protocol/rear_object_2_rpt_0x42.cc",
        "protocol/rear_object_3_rpt_0x43.cc",
        "protocol/rear_object_4_rpt_0x44.cc",
        "protocol/rear_object_5_rpt_0x45.cc",
        "protocol/rear_object_6_rpt_0x46.cc",
        "protocol/steering_rpt_0x02.cc",
    ],
    hdrs = [
        "fortuna_controller.h",
        "fortuna_message_manager.h",
        "fortuna_vehicle_factory.h",
        "protocol/assistance_2_rpt_0xe.h",
        "protocol/motion_rpt_0x01.h",
        "protocol/powertrain_1_rpt_0x04.h",
        "protocol/front_object_1_rpt_0x31.h",
        "protocol/front_object_2_rpt_0x32.h",
        "protocol/front_object_3_rpt_0x33.h",
        "protocol/front_object_4_rpt_0x34.h",
        "protocol/rear_object_1_rpt_0x41.h",
        "protocol/rear_object_2_rpt_0x42.h",
        "protocol/rear_object_3_rpt_0x43.h",
        "protocol/rear_object_4_rpt_0x44.h",
        "protocol/rear_object_5_rpt_0x45.h",
        "protocol/rear_object_6_rpt_0x46.h",
        "protocol/steering_rpt_0x02.h",
    ],
    deps = [
        "//modules/canbus:apollo_canbus",
        "//modules/canbus_vehicle/fortuna/proto:fortuna_cc_proto",
        "//modules/common/adapters:adapter_gflags",
        "//modules/common/kv_db",
        "//modules/common/status",
        "//modules/common_msgs/chassis_msgs:chassis_cc_proto",
        "//modules/drivers/canbus:apollo_drivers_canbus",
        "@com_google_googletest//:gtest",
    ],
)

apollo_component(
    name = "libfortuna_vehicle_factory_lib.so",
    deps = [":apollo_canbus_vehicle_fortuna"],
)

apollo_cc_test(
    name = "fortuna_controller_test",
    size = "small",
    srcs = ["fortuna_controller_test.cc"],
    data = ["//modules/canbus:test_data"],
    deps = [
        ":apollo_canbus_vehicle_fortuna",
        "@com_google_googletest//:gtest_main",
    ],
)

# apollo_cc_test(
#     name = "fortuna_message_manager_test",
#     size = "small",
#     srcs = ["fortuna_message_manager_test.cc"],
#     deps = [
#         ":apollo_canbus_vehicle_fortuna",
#         "@com_google_googletest//:gtest_main",
#     ],
# )

apollo_cc_test(
    name = "fortuna_vehicle_factory_test",
    size = "small",
    srcs = ["fortuna_vehicle_factory_test.cc"],
    data = ["//modules/canbus:test_data"],
    linkstatic = True,
    deps = [
        ":apollo_canbus_vehicle_fortuna",
        "@com_google_googletest//:gtest_main",
    ],
)

filegroup(
    name = "runtime_data",
    srcs = glob([
        "testdata/**",
    ]),
)


apollo_package()

cpplint()
