# Feature 1 - Backdoor Trigger

The first rootkit feature we will implement is a remote backdoor shell.

However, in order to actually start this shell, we need some sort of trigger mechanism that allows us to log in.
This week, you will work with your group to write such a mechanism that exploits the sshd logger.

## sshd

Our rootkit will be remotely activated using ssh. When a user normally uses ssh they type the command below

`ssh username@address`

Then they are prompted for a password, and they can log in.

However, you might not know that every time a user attempts to log in, their username is logged to /var/log/auth.log.
In order to do this, the `sshd` process needs to use the [write syscall](https://man7.org/linux/man-pages/man2/write.2.html).

## Task

Hook the write syscall so that the function `start_bind4()` is called when a user attempts to log in with the trigger username.
Don't worry about what this function actually does for right now, we will implement it next week. For now, you can just make it print something out to the console to confirm it works.

When your rootkit is installed, you should be able to run the following command

`ssh joebruin@localhost` and it should print out something to the console.

Write your code in evil.c, and use the following commands to test your code on the virtual machine.

```
make - compiles the malicious library
make install - installs the rootkit locally
make clean - uninstalls the rootkit locally
```
