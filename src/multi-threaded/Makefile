# Copyright 2022 Bryon Baker

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Use the git tag to set the image version etc
VERSION := $(shell git describe --tags --dirty=-modified --always)
APP_NAME := mt-sigsegv

clean:
	rm -f bin/${APP_NAME}
	rm -f image-artifacts/*

build:
	gcc -fdiagnostics-color=always -g mt-sigsegv.c threading.c -Wl,-Map=./bin/${APP_NAME}.map -lpthread -o ./bin/${APP_NAME}

package: clean build
	podman build -t ${APP_NAME}:${VERSION} .
	podman tag ${APP_NAME}:${VERSION} ${APP_NAME}:latest
	podman tag ${APP_NAME}:${VERSION} quay.io/bryonbaker/${APP_NAME}:${VERSION}
	podman tag ${APP_NAME}:${VERSION} quay.io/bryonbaker/${APP_NAME}:latest
	./grab-build-artifacts.sh ${APP_NAME} latest
run:
	bin/${APP_NAME}

test: clean build
	echo "Tsk tsk! \"make test\" is not implemented yet."

all: clean build