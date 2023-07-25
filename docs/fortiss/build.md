## Set Up the Docker Environment

The Docker container is the simplest way to set up the build environment for Apollo.

For more information, see the detailed Docker tutorial [here](https://docs.docker.com/).

1. Please follow the [official guide to install the docker-ce 19.03+](https://docs.docker.com/install/linux/docker-ce/ubuntu).

Don't forget the [post-installation steps for Linux](https://docs.docker.com/install/linux/linux-postinstall).

2. After the installation, log out and then log back into the system to enable Docker.

3. (Optional) If you already have Docker installed (before you installed the Apollo Kernel), add the following line in `/etc/default/docker`:

    ```
    DOCKER_OPTS = "-s overlay"
    ```

4. Install latest nvidia-container-toolkit by following the [official doc](https://github.com/NVIDIA/nvidia-docker).


## Build the docker container

As we our own docker file definition, we have to create that manually.

`cd docker/build`

`./build_dev.sh ./dev_fortiss.dockerfile`

## Use docker to build apollo

Then go back to the top directory (change the setting in  https://github.com/fortiss/apollo/blob/0d6cb31708f13363f441d475469a6d5cb1955f44/docker/scripts/dev_start.sh#L27 as "no" if you use Apollo for the first time!), navigate to the 2. upper path (i.e. ~/apollo) and continue with 

`bash docker/scripts/dev_start.sh`

`bash docker/scripts/dev_into.sh`

Now, inside the docker container, build Apollo using:

 `bash apollo.sh build`

**Note: ** calling `bash apollo.sh build_cpu` instead of `bash apollo.sh build`causes some erros, as opencv cannot be found. Don't do it and stick to the plan build call.
**Note: ** If you experience experience some weird stuff when building, clean all bazel directories using `bazel clean --expunge`

## Fortiss MIQP Planner
In order to use the MIQP Planner (described in the publications [1](https://ieeexplore.ieee.org/document/9304743) and [2](https://ieeexplore.ieee.org/document/9304495), see [source code](https://github.com/bark-simulator/planner_miqp)), you need to build Apollo at the first time using 

`bash apollo.sh build_use_planner_miqp`.

Also, as it requires the CPLEX solver, you need to place the CPLEX libraries at `apollo/cplex`. For more info, see cplex/README.md

For getting the above specific files, you have to ask for our maintainer JfortissJ!

## Reset speed limit for using the planers in high-speed scenarios (Attention: all speed values in Apollo's code are using the unit 'm/s')

1. First of all, check the speed limit of the Apollo maps, for example:
https://github.com/fortiss/apollo/blob/fc62d694f5b44e1a9a72cb47487ab85b51b5a6b7/modules/map/data/guerickestrasse_assymetric_47/base_map.txt#L1239

 Actually, when you create the Apollo maps with the map generation tools (like apollo/modules/tools/map_gen/map_gen_single_lane.py), you should rewrite    the speed limit as your need: https://github.com/fortiss/apollo/blob/fc62d694f5b44e1a9a72cb47487ab85b51b5a6b7/modules/tools/map_gen/map_gen_single_lane.py#L74

2. Then, you can reset the following parameters in the "planning.conf" file under direction "apollo/modules/planning/conf/planning.conf" as your need:
https://github.com/fortiss/apollo/blob/1e58c6d68832fe1dbd01cf949d74326f4f007062/modules/planning/conf/planning.conf#L4
https://github.com/fortiss/apollo/blob/1e58c6d68832fe1dbd01cf949d74326f4f007062/modules/planning/conf/planning.conf#L11
If you use other planers, reset the above parameters in the corresponding .conf file.

3. Next, reset the parameters in apollo/modules/planning/common/planning_gflags.cc as your need:
https://github.com/fortiss/apollo/blob/1e58c6d68832fe1dbd01cf949d74326f4f007062/modules/planning/common/planning_gflags.cc#L171
https://github.com/fortiss/apollo/blob/1e58c6d68832fe1dbd01cf949d74326f4f007062/modules/planning/common/planning_gflags.cc#L325

4. `bash apollo.sh build` again

## Prerequisites

* If you want to run the Perception module and the Viszalization Components you need to install cuda and the nvidia docker toolchain:
  * Install the proprietary nvidia driver for your ubuntu: Anwendungen & Aktualisierungen -> Zusätzliche Treiber -> Search and select Select NVIDIA proprietary
  * Install utils and check afterwards: 
```
   sudo apt install mesa-utils
    glxinfo -v | grep -i opengl
    hwinfo --gfxcard --short
    nvidia-smi
    sudo lshw -C display
```
 * If nvidia docker is missing: Have a look at: https://github.com/NVIDIA/nvidia-docker. 
 * Install nvidia modprobe
 * **RESTART!!!**
 * nvblas.conf is missing but this is not a problem

## Sophos Anti-Virus

New fortiss ubuntu installations get shipped with Sophos Anti-Virus.

When Running `dev_start.sh`, I got the following error:

**<!--******************** Sophos Anti-Virus Alert ***********************-->
<!--Error scanning file-->
<!--"/run/snapd/lock/docker.lock".-->

<!--Access to the file has been denied-->

**********************************************************************
<!--Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Post http://%2Fvar%2Frun%2Fdocker.sock/v1.39/images/create?fromImage=apolloauto%2Fapollo&tag=map_volume-sunnyvale_with_two_offices-latest: dial unix /var/run/docker.sock: connect: permission denied-->

First, check the status of sophos. You need admin rights for that.
`root@for1797:/opt/sophos-av/bin# ./savdstatus`
`Sophos Anti-Virus is active and on-access scanning is running`

For now, you can disable the on access scanner via:
`root@for1797:/opt/sophos-av/bin# ./savdctl disable`

Check again:
`root@for1797:/opt/sophos-av/bin# ./savdstatus`
`Sophos Anti-Virus is active but on-access scanning is not running`

Unfortunately, it get's activated again after restarting the computer.
