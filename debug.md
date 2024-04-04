1. if gpu support/CUDA cannot be found, and the new setuo for local_config_cuda will fail. check the environment TF_NEED_CUDA and GPU_PLATFORM
can be solved by 
```
$ export TF_NEED_CUDA=1
$ export GPU_PLATFORM="NVIDIA"
```