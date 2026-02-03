#  TGbot_cpp

This is a test version of the bot, written in c++, the idea is simplicity and functionality, now the bot can receive requests, send messages, and respond to messages.

I would like to note that TGbot is tested only under macOS. However, there should be support for other platforms as it has no dependencies on the operating system.

To use the bot:

After creating a bot through BotFather and copying the token to main.cpp, we take the following steps:

1) Install [Brew](https://brew.sh/)

2) Install CURL with the command: brew install curl

3) Install Json parser [Json](https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp)
 
4) mkdir ~/desktop/tgbot;

5) Place the files "main.cpp TGbot.hpp TGbot.cpp receive.cpp receive.hpp json.hpp" into a folder "~/desktop/tgbot"

6) Compile cd ~/desktop/tgbot; g++ main.cpp TGbot.cpp receive.cpp -o tgbot -std=c++17 -lcurl


Or you can assemble it with one team:

```bash
curl -O https://raw.githubusercontent.com/DiCode77/TGbot_cpp/main/build.sh && chmod +x build.sh && ./build.sh
```


Another build option has been added, using conan, which generates build files for a specific environment.

```bash
cd /TGbot_cpp/TGbotCpp
mkdir build
cd build

conan install .. --output-folder=. --build=missing -s build_type=Release
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake 
cmake  --build .
./TGbotCpp
```