#! /bin/bash

# Run buildah unshare in a subshell
buildah unshare <<EOF
container=\$(buildah from quay.io/bryonbaker/mt-sigsegv:latest)
mnt=\$(buildah mount \$container)

cp \$mnt/app/mt-sigsegv.map ./image-artifacts/
cp \$mnt/app/mt-sigsegv ./image-artifacts/
exit
EOF