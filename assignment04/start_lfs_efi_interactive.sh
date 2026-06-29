#!/bin/bash

qemu-system-x86_64 \
	-enable-kvm \
	-m 28G \
	-smp cores=16,threads=2,sockets=1 \
	-cpu host \
	-net nic,model=virtio -net user,hostfwd=tcp::22222-:22 \
	-device virtio-balloon \
	-vga virtio \
	-full-screen \
	-monitor stdio \
	-device qemu-xhci \
	-drive if=pflash,format=raw,readonly=on,file=./OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=./OVMF_VARS.fd \
	-hda lfs-assignment02.qcow2
