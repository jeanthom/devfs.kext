# An example macOS kext creating a file in /dev

Based on [this boilerplate code](https://github.com/droe/example.kext) just to show that it is possible to create a `/dev/` on macOS in a kext.

## Install

```
make
sudo chown -R root:wheel devfs.kext
sudo kextload devfs.kext
```

## Uninstall

```
sudo kextunload devfs.kext
sudo rm -rf devfs.kext
```
