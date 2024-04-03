07.03.2023
1. nlopt//:nlopt: missing input file 'external/nlopt/include/nlopt.h', owner: '@nlopt//:include/nlopt.h'
Solution: 
```
$ sudo bash docker/build/installer/install_nlopt.sh
```
2.  sudo apt update with errot W: GPG error: https://apollo-pkg-beta.bj.bcebos.com/neo/beta bionic InRelease: The following signatures couldn't be verified because the public key is not available: NO_PUBKEY A769FBD86CEFB5EA
E: The repository 'https://apollo-pkg-beta.bj.bcebos.com/neo/beta bionic InRelease' is not signed.
N: Updating from such a repository can't be done securely, and is therefore disabled by default.
N: See apt-secure(8) manpage for repository creation and user configuration details.

Solution:
```
$ wget -O - https://apollo-pkg-beta.cdn.bcebos.com/neo/beta/key/deb.gpg.key | sudo apt-key add -
$ sudo apt update
```
3. ERROR: /apollo/modules/canbus_vehicle/gem/BUILD:68:11: undeclared inclusion(s) in rule '//modules/canbus_vehicle/gem:gem_controller':
this rule is missing dependency declarations for the following files included by 'modules/canbus_vehicle/gem/gem_controller.cc':
  'bazel-out/k8-fastbuild/bin/modules/common_msgs/control_msgs/control_cmd.pb.h'
  'bazel-out/k8-fastbuild/bin/modules/common_msgs/control_msgs/input_debug.pb.h'

Solution:
add `"//modules/common_msgs/control_msgs:control_cmd_cc_proto",` to `/apollo/modules/canbus_vehicle/gem/BUILD:68:11`
03.04.2024
4. E0403 16:39:47.159559 825464 class_loader_utility.cc:218] [mainboard]LibraryLoadException: libnlopt.so.0: cannot open shared object file: No such file or directory [fake_obstacle] E0403 16:39:47.159619 825464 class_loader_utility.cc:234] [mainboard]shared library failed: /apollo/bazel-bin/modules/fake_obstacle/libfake_obstacle_component.so:
```ldconfig -p | grep libnlopt.so.0``` returns nothing, so need to install it ```sudo apt-get install libnlopt0```

