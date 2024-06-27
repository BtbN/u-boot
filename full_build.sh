#!/bin/bash
set -xeo pipefail
cd "$(dirname "$0")"

DEV="sd"
[[ -z "$1" ]] || DEV="$1"

git clean -fxd
git reset --hard

echo -e "board=bpi-r4\ndevice=$DEV" > build.conf

bash build.sh importconfig
bash build.sh build
bash build.sh rename

git checkout build.conf
git checkout mtk-atf

[[ "$DEV" != "spi-nand" ]] || DEV="spim-nand"
[[ "$DEV" != "sd" ]] || DEV="sdmmc"
echo -e "board=bpi-r4\ndevice=$DEV" > build.conf

bash build.sh build
bash build.sh rename
[[ "$DEV" == "spim-nand" ]] || bash build.sh createimg non-interactive

rm build.conf
git checkout -
git checkout build.conf
