#!/bin/bash

sudo chown $USER:$USER Image/x64BareBonesImage.qcow2

if [ "$(uname)" == "Darwin" ]; then
    audio_driver="coreaudio"
else
    audio_driver="pa"
fi

qemu-system-x86_64 \
  -hda Image/x64BareBonesImage.qcow2 \
  -m 512 \
  -audiodev ${audio_driver},id=speaker \
  -machine pcspk-audiodev=speaker
