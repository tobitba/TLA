#!/bin/bash

if ! command -v clang-tidy &>/dev/null; then
	echo -e "\e[31mError: clang-tidy is not installed.\e[0m"
	exit 1
fi

if ! command -v clang-format &>/dev/null; then
	echo -e "\e[31mError: clang-format is not installed.\e[0m"
	exit 1
fi

git config --local core.hooksPath .githooks/
