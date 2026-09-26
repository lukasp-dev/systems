## Things to know about Linux

Linux directories start at `/`. `/` is the top of the whole filesystem. The directories below all sit directly inside `/`.

### `/boot`
Linux does not start the moment the computer powers on.
    1. The firmware finds the disk.
    2. It starts the bootloader, GRUB.
    3. GRUB loads the kernel into memory, and then Linux starts.

`/boot` holds the files for that process.
- `grub.cfg`: GRUB's configuration file, usually at `/boot/grub/grub.cfg`. It lists the boot menu, the wait time, and which kernel to load.
- kernel: the file is named `vmlinuz-...`, it is the core of Linux. It manages the disk, memory, and processes.
- initramfs: the file is named `initramfs-...` or `initrd-...`. It is a small temporary filesystem used in the short moment before the real disk can be opened.

### `/root`
This is the home directory of the root account. root is the account with full privileges.
`/` is the top of the filesystem. `/root` is one directory inside it, and it belongs to the root account.

### `/home`
This is the home directory for a normal user. For a user named `lukas`, it is `/home/lukas`.

### `/dev`
This directory holds device files. `dev` means device.

A file under `/home` contains stored data. Writing to a file under `/dev` does not store text in that file. The write is passed to the device behind it.
ex)
- `/dev/sda`: the first disk. A write here is written to that disk.
- `/dev/sda1`: the first partition on that disk.
- `/dev/null`: there is no device behind it. A write here is discarded.

These files are not created with `touch`. At boot, the kernel creates `/dev`, and udev fills it with the devices it finds. Plugging in a USB stick adds an entry. Removing it deletes that entry, for example.

### `/etc`
This directory holds system configuration files.
ex)
- `/etc/passwd`: the list of accounts. Passwords are in `/etc/shadow`.
- `/etc/fstab`: the list of which disk is attached to which directory at boot.
- `/etc/hostname`: this computer's name.
- `/etc/ssh/sshd_config`: SSH access settings.

### `/bin` → `/usr/bin`
This directory holds everyday commands such as `ls`, `cp`, `cat`, and `pwd`.

The arrow means a symbolic link. Opening the name `/bin` leads to the files in `/usr/bin`.

### `/sbin` → `/usr/sbin`
This directory holds system administration commands.
ex)
- `fdisk`: views and divides partitions.
- `mkfs`: creates a filesystem inside a partition.
- `reboot`: restarts the computer.

`/sbin` is often a symbolic link to `/usr/sbin`.

### `/lib` → `/usr/lib`
This directory holds shared libraries that commands load while they run. The file names end in `.so`, as in `libc.so.6`.

`strace -e open pwd` shows which files `pwd` opens. C header files (`.h`) are in `/usr/include`.

`/lib` is often a symbolic link to `/usr/lib`.

### `/opt`
This directory holds software installed separately, outside the distribution's base packages. An application often takes a whole directory, such as `/opt/program-name`.

### `/proc`
This directory shows running processes and kernel information. It is not stored on disk. It exists only in memory.
ex)
- PID: a process id. Each running program gets one number.
- `/proc/1`: the process whose PID is 1.
- `/proc/cpuinfo`: CPU information.
- `/proc/meminfo`: memory information.

It disappears when the computer shuts down. On the next boot, the kernel builds it again from the processes running then.

### `/run`
This directory is where services record that they are currently running. It exists only in memory.
ex)
- systemd: the program that starts after the kernel and starts the other services.
- udev: the program that creates files in `/dev` when it finds a device.
- PID file: a file that records a service's process id.
- socket: a connection point between programs.

It disappears when the computer shuts down. On the next boot, systemd and udev create it again.

### `/tmp`
This directory holds temporary files. It is often emptied on reboot.

### `/var`
This directory holds data that changes while the system is running. `var` means variable.
ex)
- `/var/log`: logs.
- `/var/lib`: data a program keeps over time.
- `/var/cache`: copies that can be downloaded again.
- `/var/tmp`: temporary files that usually survive a reboot.

### `/mnt`
This is a mount point an administrator uses to attach a disk by hand. It is usually empty.

A mount makes a disk, or a directory on another computer, appear inside a directory on this computer. NFS is a way to use a directory that lives on another computer.

### `/media`
This is the mount point used when removable media, such as a USB stick or a CD, is attached automatically. A USB stick shows up under `/media/lukas/device-name`.
