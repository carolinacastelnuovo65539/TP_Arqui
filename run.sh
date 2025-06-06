#!/bin/bash

# qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512

# qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker


# este sudo lo tengo que hacer cada vez desp de compilar, sino no puedo correrlo sin hacer: [sudo ./run.sh]
# si no hago el sudo, y directamente lo corro asi: [sudo ./run.sh] corre todo bien pero no anda el sonido 
# sudo chown $USER:$USER Image/x64BareBonesImage.qcow2

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
