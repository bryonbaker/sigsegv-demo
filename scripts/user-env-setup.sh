#! /bin/bash

export KUBECONFIG=$HOME/.kube/config-general
export PS1="\[$(tput setaf 2)\]USER \u@\h\[$(tput setaf 7)\]:\$(parse_git_branch)\[$(tput setaf 6)\]\W\\$ \[$(tput setaf 7)\]\[$(tput sgr0)\]"
