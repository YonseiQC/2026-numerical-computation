ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

print_makefile_dir:
	@echo "$(ROOT_DIR)"

format:
	@./bin/format_cpp.sh
