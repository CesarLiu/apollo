## Install pyKOSIbuffer package from gitlab

The repository: https://gitlab.lrz.de/kosi/wp6/runtime_protocol/pykosibuffer
 
### Enter the docker container: 
in apollo directory:
```
bash docker/scripts/dev_start.sh
bash docker/scripts/dev_into.sh
```

### Clone and install kosibuffer repository:
```
git clone https://gitlab.lrz.de/kosi/wp6/runtime_protocol/pykosibuffer.git
pip3 install -e pykosibuffer/
```


### To import the package in your script use:
```
from kosibuffer import kosi_base_pb2
```
