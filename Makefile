MOUNT_POINT = /mount/cse240a-mount
NAME = prodromou87/ucsd_cse240a
TAG = latest

.PHONY: run
run:
	docker run --rm -it --entrypoint=bash \
	-v $(shell pwd):$(MOUNT_POINT) \
	-w $(MOUNT_POINT)/src \
	$(NAME):$(TAG)