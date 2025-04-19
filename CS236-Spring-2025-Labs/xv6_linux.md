# xv6 Installation Instructions for Linux

1. **Install qemu** 
   ```bash
   sudo apt-get install qemu-system
   ```

   To download and clone from git 
   ```
   git clone https://gitlab.com/qemu-project/qemu.git
   cd qemu
   ./configure
   make
   ```

   Refer website for more details [click here.](https://www.qemu.org/download/#source)

2. **Download the xv6 Mac Tarball from the course webpage.**
   

3. **Compile xv6:**
   ```bash
   make clean
   make
   ```

4. **Start the QEMU emulator:**
   ```bash
   make qemu
   ```

   or

   ```bash
   make qemu-nox
   ```
   
   The latter will not pop up an extra window.

5. **User tests:** (for testing)

   A separate window will pop up (displaying the qemu emulator window). Run the following command:
   ```bash
   usertests
   ```
   If the following command displays "All tests passed", then you are good to go.


---

It is recommended that you create separate copies of this xv6 folder for every problem you solve (so that you do not disturb the original files). If applicable, directly copy the templates files using the `mv` command into a fresh copy of the xv6 folder. 
