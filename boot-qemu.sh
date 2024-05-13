qemu-system-arm -s -S -cpu cortex-a7 \
-machine virt -nographic -m 512M \
-serial pty -bios u-boot/u-boot.bin \
-netdev user,id=net0,tftp=/tftpboot/,hostfwd=tcp::2222-:22 \
-device e1000,netdev=net0
