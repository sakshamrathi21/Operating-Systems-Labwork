# xv6 Installation Instructions for Mac

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install Required Packages:** (this step might take time)
   ```bash
   brew install qemu gcc
   ```

3. **Tap the repository and install the packages:** (this step might take time)
   ```bash
   brew tap nativeos/i386-elf-toolchain
   brew install nativeos/i386-elf-toolchain/i386-elf-binutils
   brew install nativeos/i386-elf-toolchain/i386-elf-gcc
   ```

4. **Clone the xv6 repository:**
   ```bash
   git clone https://github.com/mit-pdos/xv6-public
   cd xv6-public
   rm Makefile mp.c
   ```

5. **Patches**

   Overwrite ``Makefile`` and ``mp.c`` inside the xv6 folder with the patched versions of the files provided to you.

6. **Compile xv6:**
   ```bash
   make clean
   make
   ```

7. **Start the QEMU emulator:**
   ```bash
   make qemu
   ```

   or 

   ```bash
   make qemu-nox
   ```
   
   The latter will not pop up an extra window.

8. **User tests:** (for testing)

   A separate window will pop up (displaying the qemu emulator window). Run the following command:
   ```bash
   usertests
   ```
   If the following command displays "All tests passed", then you are good to go.


---

It is recommended that you create separate copies of this xv6 folder for every problem you solve (so that you do not disturb the original files).


In case you face any issues in the above steps, then please contact: Saksham Rathi (Mail: 22b1003@iitb.ac.in ; WhatsApp: +91-6375-66-3539). In the worst case, you will have to use lab machines through ssh for using xv6 (which isn't really tough).