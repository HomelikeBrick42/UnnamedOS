# Unnamed OS

# How to use

```console
cd ./gnu-efi
make bootloader
cd ../kernel
make setup
make kernel
make buildimg
```

## And then to run with qemu

```console
cd ./kernel
make run
```
