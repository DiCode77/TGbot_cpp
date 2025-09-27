//
//  main.cpp
//  TGbot_cpp
//
//  Created by DiCode77.
//

#include <iostream>
#include "TGbot.hpp"

int main(){
    TGBOT::TGbot bot("TOKEN");
    
    bot.Event().timeOut(5);
    bot.Event().getUpdate().UpdateAndisStatus();
    
    if (bot.getIsStatusBot()){
        std::cout << "The bot is active!" << std::endl;
        long mess_id(0);
        long chat_id(0);

        while (bot.Event().getUpdate().UpdateAndisStatus()){
            bot.Result().get().id().isNum(mess_id);
            if (bot.Result().Message().get().text().isCommand("/start")){
                if (bot.Result().Message().Chat().get().id().isNum(chat_id)){
                    bot.Event().send().message(chat_id, "Hi, I'm a bot");
                    bot.Event().clear().updates().First(mess_id);
                }
            }
            else{
                bot.Event().clear().updates().First(mess_id);
            }
        }
    }
    
    return 0;
}
