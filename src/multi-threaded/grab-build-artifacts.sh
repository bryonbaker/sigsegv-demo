#! /bin/bash

# Usage:
# ./grab-build-artifacts.sh <image> <tag>

echo "Extracting build artifacts from image: $1:$2"

# Run buildah unshare in a subshell and pass the command sequence 
# into the subshell.
# Then reate the target directory on the build host and copy the 
# files from the mounted image to the host filesystem.
buildah unshare <<EOF
container=\$(buildah from $1:$2)
mnt=\$(buildah mount \$container)

mkdir -p ./image-artifacts
cp \$mnt/app/$1.map ./image-artifacts/
cp \$mnt/app/$1 ./image-artifacts/
exit
EOF