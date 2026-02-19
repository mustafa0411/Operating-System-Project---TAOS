# Preparing Windows for MPX Development

Windows is not suited for native MPX development. Instead, you will need to
set up a Linux distribution in a either virtual machine or using the Windows
Subsystem for Linux (WSL).

## Virtual Machine
The recommended virtual machine is LOUD, the LCSEE Optimized Ubuntu
Distribution. Follow the directions at
<https://lcseesystems.wvu.edu/services/loud>. If you encounter issues where
the virtual machine hangs at a black screen, or is unusably slow (indicated
by a turtle icon in the bottom-right hand corner of the VirtualBox window),
consider WSL instead.

## WSL
WSL is an optional component of Windows 10 and later. First, you will need
to ensure that WSL itself is enabled, and that a distribution is installed.
Open an elevated Command Prompt or PowerShell window by pressing the Windows
Key + X, and choosing "Command Prompt (Admin)" or "PowerShell (Admin)". In
this window, run:

    wsl --install -d ubuntu

This will enable WSL if it isn't already, and install Ubuntu along with it.
If WSL wasn't already installed, you may need to reboot before you can launch
an Ubuntu window. The first time you open an Ubuntu window, you'll be
prompted to create a username and password. This will become the local
account within the Ubuntu environment, and the password will become the one
you need later to run commands with `sudo`. Once the account is set up,
follow the steps for Ubuntu below.


# Preparing Ubuntu and Other Debian Derivatives for MPX Development

Ubuntu is the primary development environment for MPX and the basis for LOUD,
so no extensive preparation is needed. Simply open a terminal window and run
the following commands:

    sudo apt update
    sudo apt install -y clang make nasm git binutils-i686-linux-gnu qemu-system-x86 gdb


# Preparing macOS for MPX Development

All commands need to be run from a Terminal. You should be able to find
the Terminal application in the `/Applications` folder of your system's
internal disk. Alternatively, pressing Command+Space and typing `Terminal`
should bring it up.

## Install XCode Tools
First, you need to install the XCode development tools. This includes
the compiler, clang, and GNU make. This command will open a pop-up window
for confirmation. Once confirmed, this may take some time to complete.

    xcode-select --install

## Install Homebrew
Next, install the Homebrew package manager from <https://brew.sh>. There
should be a command under the label "Install Homebrew" that you can copy
and paste into your Terminal window. Note that this makes use of the
XCode tools installed in the first step, so that **must** be complete prior
to this step.

It is likely that installing Homebrew will prompt you for your password
so that it can elevate privileges using `sudo`. This is the same password
you use to unlock your account when you turn on your system.

Note also that once the command you paste from the web site completes, there
are a few additional steps you need to take to finalize the installation of
Homebrew. In your terminal window, there will be some output beginning with
the bold words **`==> Next steps:`**. You must follow the instructions in your
Terminal window to complete the Homebrew installation.

## Install Remaining Tools
Once Homebrew is installed, you can easily install NASM, QEMU, the
cross-linker, and cross-debugger.

    brew install nasm qemu i686-elf-binutils i386-elf-gdb

If you get an error here, make sure that you followed the
**`==> Next steps:`** portion of the Homebrew installation process. You may
need to open a new Terminal window for the changes to take effect.
