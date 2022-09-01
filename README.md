# bertOS

## Table of Contents
1. [Background](#Background)
3. [Resources](#Resources)
4. [My Working Environment](#My-Working-Environment)

### Background
bertOS on the pi

### Resources
todo. see the wiki for now.

### My Working Environment
i test on qemu. currently using a Lenovo Thinkpad x1 carbon gen 9 running fedora.

### How to run
Please install the requirements listed in requirements.txt. 
On Fedora it can be done like this: `sudo dnf install $(cat requirements.txt)`
Use your operating system's appropriate package manager. 

After that, simply run `make qemu` and it will start up. I recommend redirecting stdout because I print a lot of stuff, but I'll leave that up to you to decide. 
