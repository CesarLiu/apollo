"""Loads the paddlelite library"""

# Sanitize a dependency so that it works correctly from code that includes
# Apollo as a submodule.
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def clean_dep(dep):
    return str(Label(dep))

def repo():
    http_archive(
        name = "centerpoint_infer_op-x86_64",
        sha256 = "9a8e95e0e71d4fbf6369c21541bbcaaf96581b8df1b1623e7fbf6049fda69306",
        strip_prefix = "centerpoint_infer_op",
        urls = ["https://apollo-pkg-beta.cdn.bcebos.com/archive/centerpoint_infer_op_cu111.tar.gz"],
    )

    http_archive(
        name = "centerpoint_infer_op-aarch64",
        strip_prefix = "centerpoint_infer_op",
        url = "file:///opt/apollo/centerpoint_infer_op-aarch64.tar.gz",
    )

