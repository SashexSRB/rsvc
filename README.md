# rsvc - Runit Service Control

A lightweight, modern C23 service control for **Artix Linux**

`rsvc` provides a clean and intuitive interface to manage runit services - similar to `systemctl` but designed specifically for runit.

## Features
- Simple and fast CLI
- Alphabetically sorted table list of services
- Support for `enable` / `disable` / `start` / `stop` / `restart`
- Written in modern **C23**
- No external dependencies (only standard libraries + `sv`)
## Usage
```bash
# USE WITH SUDO!
rsvc list    # You can pipe it to grep <service> to only list one service.
rsvc start   <service>
rsvc stop    <service>
rsvc restart <service>
rsvc enable  <service>
rsvc disable <service>
rsvc help    <service>
```
## Installation
```bash
git clone https://github.com/SashexSRB/rsvc.git
cd rsvc
make
sudo make install
```
## Uninstall
```bash
sudo rm /usr/local/bin/rsvc
```
This installs rsvc to `/usr/local/bin`
## Project Structure
```text
rsvc/
|- Makefile
|- README.md
|- include/
|  -- rsvc.h
|- src/
   |- main.c
   |- list.c
   |- control.c
   |- enable.c
   |- utils.c
```
## Why rsvc?
- Fast and light
- Clean output optimized for terminal use
- Designed for Artix Linux runit
- Easy to read and maintain C code
## License
This project is licensed under GPL-3.0 license
## Contributing
Contributions are very welcome!
