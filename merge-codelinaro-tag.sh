#!/bin/bash
#
# Copyright (C) 2022 TheStrechh (Carlos Arriaga)
#
# Basic scrip for merge CodeLinaro
#

# Colors
red=$'\e[1;31m'
grn=$'\e[1;32m'
blu=$'\e[1;34m'
end=$'\e[0m'

echo "${grn} Initiating... ${end}"
echo

# Fetch kernel version from user
read -p "<Enter your Kernel version:> " VERSION
echo

sleep 1

# Fetch CAF tag from user
read -p "<Enter the CodeLinaro tag you want to merge:> " TAG
echo

sleep 1

# Merging
echo "${blu} ||===Starting merge===|| ${end}"
echo

    git fetch https://git.codelinaro.org/clo/la/platform/vendor/qcom-opensource/wlan/qcacld-3.0 $TAG &&
	git merge -X subtree=drivers/staging/qcacld-3.0 FETCH_HEAD

    git fetch https://git.codelinaro.org/clo/la/platform/vendor/qcom-opensource/wlan/fw-api $TAG &&
	git merge -X subtree=drivers/staging/fw-api FETCH_HEAD

    git fetch https://git.codelinaro.org/clo/la/platform/vendor/qcom-opensource/wlan/qca-wifi-host-cmn $TAG &&
	git merge -X subtree=drivers/staging/qca-wifi-host-cmn FETCH_HEAD

    git fetch https://git.codelinaro.org/clo/la/platform/vendor/opensource/audio-kernel $TAG &&
	git merge -X subtree=techpack/audio FETCH_HEAD

    git fetch https://git.codelinaro.org/clo/la/kernel/msm-$VERSION $TAG &&
	git merge FETCH_HEAD

sleep 1

echo
echo "${red} Merge successfull... ${end}"
