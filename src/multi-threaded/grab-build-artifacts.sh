#! /bin/bash

echo "Extracting build artifacts from image: $1:$2"

# Run buildah unshare in a subshell
buildah unshare <<EOF
container=\$(buildah from $1:$2)
mnt=\$(buildah mount \$container)

mkdir -p ./image-artifacts
cp \$mnt/app/$1.map ./image-artifacts/
cp \$mnt/app/$1 ./image-artifacts/
exit
EOF