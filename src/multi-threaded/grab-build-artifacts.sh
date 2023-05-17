#! /bin/bash

echo "Extracting build artifacts from image: $1"

# Run buildah unshare in a subshell
buildah unshare <<EOF
container=\$(buildah from $1)
mnt=\$(buildah mount \$container)

cp \$mnt/app/mt-sigsegv.map ./image-artifacts/
cp \$mnt/app/mt-sigsegv ./image-artifacts/
exit
EOF