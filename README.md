To run this code in Code::Blocks, you need to follow these steps:

1. **Install Code::Blocks:**
   - Download and install Code::Blocks from the official website: [Code::Blocks Download](http://www.codeblocks.org/downloads)
   - Make sure to download the version that suits your operating system.

2. **Set up SFML:**
   - Download the SFML library from the official website: [SFML Download](https://www.sfml-dev.org/download.php)
   - Follow the installation instructions for your operating system.

3. **Create a new project in Code::Blocks:**
   - Open Code::Blocks and go to `File > New > Project...`
   - Choose "Console application" and click "Go."
   - Select "C++" as the language and click "Next."
   - Choose a project title and location, then click "Next."
   - Select "Console application" again and click "Next."
   - Choose a compiler. Usually, the default GNU GCC Compiler works fine. Click "Finish."

4. **Add SFML to the project:**
   - Open the project you just created.
   - Right-click on the project name in the "Management" pane and select "Build Options."
   - In the "Build Options" window, go to the "Linker Settings" tab.
   - Click "Add" and add the SFML library files (e.g., `sfml-graphics`, `sfml-window`, `sfml-system`, etc.).
   - Click "OK" to close the "Build Options" window.

5. **Copy and paste the provided code:**
   - Copy the entire code you provided.
   - Create a new source file in Code::Blocks (right-click on "Sources" in the "Management" pane, choose "Add Files...").
   - Paste the code into the new source file.

6. **Adjust file paths:**
   - Make sure that the file paths for your assets (images, sounds, etc.) in the code are correct relative to your project directory.

7. **Build and Run:**
   - Build your project by clicking the "Build" button (usually a hammer icon).
   - Run your project by clicking the "Run" button (usually a green play icon).
   - If everything is set up correctly, the Flappy Bird game should run in a new window.

Remember to check the console for any error messages if the program doesn't run as expected. If there are any missing dependencies or incorrect file paths, the console output will provide useful information.
