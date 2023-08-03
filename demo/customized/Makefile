DOCKER=$(shell which docker)
DC=$(shell which docker-compose)

default: test

test:
	${DC} ps

all: run

build:
	${DC} $@

run: up

up:
	${DC} $@ -d

clean: down

down:
	${DC} $@

demo:
	${DOCKER} run -d -v $(shell pwd)/src:/workspace -v $(shell pwd)/data:/data --name ${NAME} --restart=on-failure:5 ${IMAGE} tail -f /dev/null

enter: exec
exec:
	docker $@ -it ${NAME} /bin/bash

.PHONY: all build default test run up clean down demo enter exec
