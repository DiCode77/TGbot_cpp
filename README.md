#  TGbot_cpp

This is a test version of the bot, written in c++, the idea is simplicity and functionality, now the bot can receive requests, send messages, and respond to messages.
Хочу підмітити що TGbot тестується поки тільки під macOS.

To use the bot:

After creating a bot through BotFather and copying the token to main.cpp, we take the following steps:

1) Install [Brew](https://brew.sh/)

2) Install CURL with the command: brew install curl

3) Install Json parser [Json](https://github.com/nlohmann/json/releases)
 
4) mkdir ~/desktop/tgbot;

5) Place the files "main.cpp TGbot.hpp TGbot.cpp receive.cpp receive.hpp json.hpp" into a folder "~/desktop/tgbot"

6) Compile cd ~/desktop/tgbot; g++ main.cpp TGbot.cpp receive.cpp -o tgbot -std=c++17 -lcurl


