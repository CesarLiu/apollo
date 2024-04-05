curl https://dl.yarnpkg.com/debian/pubkey.gpg | apt-key add - 
curl https://bazel.build/bazel-release.pub.gpg |apt-key add - 
apt-get update -y && apt-get install -y tmux htop can-utils