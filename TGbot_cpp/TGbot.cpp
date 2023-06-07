//
//  TGbot.cpp
//  TGbot
//
//  Created by DiCode77.
//

#include "TGbot.hpp"


// Method
TGBOT::METHOD::METHOD(){};
TGBOT::METHOD::METHOD(VARIABLE &var){
    this->var = &var;
    this->size_result = 0;
}

bool TGBOT::METHOD::getStatusBot(){
    if (this->var->status[0]){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js.count("ok")){
            if (js["ok"].get<bool>()){
                this->var->status[1] = true;
                return true;
            }
            else{
                this->var->status[0] = false;
            }
        }
        else{
            this->var->status[0] = false;
            this->var->status[1] = false;
        }
    }
    else{
        this->var->status[1] = false;
    }
    return false;
}

bool TGBOT::METHOD::getIsStatusBot(){
    return (this->var->status[0] && this->var->status[1]) ? true : false;
}

long TGBOT::METHOD::getSizeResult(){
    if (this->getIsStatusBot()){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (!js["result"].empty()){
            return js["result"].size();
        }
        else{
            return 0;
        }
    }
    return -1;
}

void TGBOT::METHOD::strClear(std::string &str){
    if (str.length() > 0){
        str.clear();
    }
}

bool TGBOT::METHOD::getUpdateStatus(std::string &str){
    if (this->getIsStatusBot()){
        JsonHpp js = JsonHpp::parse(str);
        if (js.count("ok")){
            if (js["ok"].get<bool>()){
                return true;
            }
        }
    }
    return false;
}

// ***************** EVENT ******************* // 
TGBOT::EVENT_UPDATE::EVENT_UPDATE(){}
TGBOT::EVENT_UPDATE::EVENT_UPDATE(VARIABLE &var, METHOD &met, CURL_UPDATES &curl_upd){
    this->var = &var;
    this->method = &met;
    this->curl_update = &curl_upd;
}

void TGBOT::EVENT_UPDATE::Update(){
    isUpdate();
}

bool TGBOT::EVENT_UPDATE::UpdateAndisStatus(){
    isUpdate();
    return this->method->getStatusBot();
}

void TGBOT::EVENT_UPDATE::isUpdate(){
    std::string url;
    url.append(PROTOCOL);
    url.append(DOMIN_NAME);
    url.append(PATH);
    url.append(this->var->token);
    url.append(GET_UPDATES);
    
    CURLcode err = this->curl_update->getCurlUpdates(url, this->var->updates);
    if (err == CURLE_OK && this->var->updates.length()){
        this->var->status[0] = true;
    }
    else{
        this->var->status[0] = false;
    }
}

TGBOT::SEND_EVENT::SEND_EVENT(){};
TGBOT::SEND_EVENT::SEND_EVENT(VARIABLE &var, METHOD &met,  CURL_UPDATES &curl_upd){
    this->var = &var;
    this->method = &met;
    this->curl_update = &curl_upd;
}

bool TGBOT::SEND_EVENT::sendMessage(long chat_id, std::string text){
    std::string url;
    url.append(PROTOCOL);
    url.append(DOMIN_NAME);
    url.append(PATH);
    url.append(this->var->token);
    url.append(SEND_MESSAGE);
    url.append(CHAT_ID);
    url.append(std::to_string(chat_id));
    
    std::string txt;
    txt.append(TEXTS);
    txt.append(text);
    
    
    std::string upd;
    CURLcode err = this->curl_update->sendHttpMessage(url, txt, upd);
    if (err == CURLE_OK){
        return this->method->getUpdateStatus(upd);
    }
    else{
        std::cerr << "curl failed: " << curl_easy_strerror(err) << std::endl;
        return false;
    }
    
    return true;
}

bool TGBOT::SEND_EVENT::sendReplyMessage(long chat_id, long message_id, std::string text){
    std::string url;
    url.append(PROTOCOL);
    url.append(DOMIN_NAME);
    url.append(PATH);
    url.append(this->var->token);
    url.append(SEND_MESSAGE);
    url.append(CHAT_ID);
    url.append(std::to_string(chat_id));
    url.append(REPLY_MESSAGE_ID);
    url.append(std::to_string(message_id));
    
    std::string txt;
    txt.append(TEXTS);
    txt.append(text);
    
    
    std::string upd;
    CURLcode err = this->curl_update->sendHttpMessage(url, txt, upd);
    if (err == CURLE_OK){
        return this->method->getUpdateStatus(upd);
    }
    else{
        std::cerr << "curl failed: " << curl_easy_strerror(err) << std::endl;
        return false;
    }
    
    return false;
}

TGBOT::CLEAR_UPDATE_EVENT::CLEAR_UPDATE_EVENT(){};
TGBOT::CLEAR_UPDATE_EVENT::CLEAR_UPDATE_EVENT(VARIABLE &var, METHOD &met, CURL_UPDATES &curl_update){
    this->var = &var;
    this->method = &met;
    this->curl_update = &curl_update;
}

bool TGBOT::CLEAR_UPDATE_EVENT::First(long data){
    if (data && this->method->getSizeResult()){
        std::string url;
        url.append(PROTOCOL);
        url.append(DOMIN_NAME);
        url.append(PATH);
        url.append(this->var->token);
        url.append(GET_UPDATES);
        url.append(OFFSET);
        url.append(std::to_string(data +1));
        
        std::string upd;
        CURLcode err = this->curl_update->getCurlEvent(url, upd);
        if (err == CURLE_OK){
            return this->method->getUpdateStatus(upd);
        }
        else{
            std::cerr << "curl failed: " << curl_easy_strerror(err) << std::endl;
            return false;
        }
    }
    
    return false;
}

bool TGBOT::CLEAR_UPDATE_EVENT::All(long data){
    if (data && this->method->getSizeResult()){
        std::string url;
        url.append(PROTOCOL);
        url.append(DOMIN_NAME);
        url.append(PATH);
        url.append(this->var->token);
        url.append(GET_UPDATES);
        url.append(OFFSET);
        url.append(std::to_string(data + this->method->getSizeResult()));
        
        std::string upd;
        CURLcode err = this->curl_update->getCurlEvent(url, upd);
        if (err == CURLE_OK){
            return this->method->getUpdateStatus(upd);
        }
        else{
            std::cerr << "curl failed: " << curl_easy_strerror(err) << std::endl;
            return false;
        }
    }
    
    return false;
}

TGBOT::CLEAR_EVENT::CLEAR_EVENT(){};
TGBOT::CLEAR_EVENT::CLEAR_EVENT(VARIABLE &var, METHOD &met,  CURL_UPDATES &curl_update) : clear_update_event(var, met, curl_update){
    this->var = &var;
    this->method = &met;
}

TGBOT::CLEAR_UPDATE_EVENT TGBOT::CLEAR_EVENT::updates(){
    return this->clear_update_event;
}

TGBOT::EVENT::EVENT(){};
TGBOT::EVENT::EVENT(VARIABLE &var, METHOD &met) :
event_update(var, met, this->curl_update),
send_event(var, met, this->curl_update),
clear_event(var, met, this->curl_update){
    this->var = &var;
    this->method = &met;
}

void TGBOT::EVENT::timeOut(int time){
    this->curl_update.timeout(time);
}

void TGBOT::EVENT::sleep(int sleep){
    this->curl_update.sleep(sleep);
}

TGBOT::EVENT_UPDATE TGBOT::EVENT::getUpdate(){
    return this->event_update;
}

TGBOT::SEND_EVENT TGBOT::EVENT::send(){
    return this->send_event;
}

TGBOT::CLEAR_EVENT TGBOT::EVENT::clear(){
    return this->clear_event;
}
// EVENT

// ***************** ENTITIES MESSAGE ******************* //
TGBOT::OFFSET_ENTITIES_MESSAGE::OFFSET_ENTITIES_MESSAGE(){}
TGBOT::OFFSET_ENTITIES_MESSAGE::OFFSET_ENTITIES_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
    this->index = 0;
}

bool TGBOT::OFFSET_ENTITIES_MESSAGE::GetOffset(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["entities"].count("offset")){
            data = js["result"][index]["message"]["entities"]["offset"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void TGBOT::OFFSET_ENTITIES_MESSAGE::getIndex(long isIndex){
    this->index = isIndex;
}

bool TGBOT::OFFSET_ENTITIES_MESSAGE::First(long &data){
    return GetOffset(data, 0, true);
}

bool TGBOT::OFFSET_ENTITIES_MESSAGE::Last(long &data){
    return GetOffset(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::OFFSET_ENTITIES_MESSAGE::Index(long &data, long index){
    return GetOffset(data, index, (this->method->getSizeResult() > index));
}

TGBOT::LENGTH_ENTITIES_MESSAGE::LENGTH_ENTITIES_MESSAGE(){}
TGBOT::LENGTH_ENTITIES_MESSAGE::LENGTH_ENTITIES_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
    this->index = 0;
}

bool TGBOT::LENGTH_ENTITIES_MESSAGE::GetLength(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["entities"].count("length")){
            data = js["result"][index]["message"]["entities"]["length"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void TGBOT::LENGTH_ENTITIES_MESSAGE::getIndex(long isIndex){
    this->index = isIndex;
}

bool TGBOT::LENGTH_ENTITIES_MESSAGE::First(long &data){
    return GetLength(data, 0, true);
}

bool TGBOT::LENGTH_ENTITIES_MESSAGE::Last(long &data){
    return GetLength(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::LENGTH_ENTITIES_MESSAGE::Index(long &data, long index){
    return GetLength(data, index, (this->method->getSizeResult() > index));
}

TGBOT::TYPE_ENTITIES_MESSAGE::TYPE_ENTITIES_MESSAGE(){}
TGBOT::TYPE_ENTITIES_MESSAGE::TYPE_ENTITIES_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
    this->index = 0;
}

bool TGBOT::TYPE_ENTITIES_MESSAGE::GetType(std::string &str, long index, long index_type, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["entities"][index_type].count("type")){
            str = js["result"][index]["message"]["entities"][index_type]["type"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void TGBOT::TYPE_ENTITIES_MESSAGE::getIndex(long isIndex){
    this->index = isIndex;
}

bool TGBOT::TYPE_ENTITIES_MESSAGE::First(std::string &str){
    return GetType(str, 0, this->index, true);
}

bool TGBOT::TYPE_ENTITIES_MESSAGE::Last(std::string &str){
    return GetType(str, this->method->getSizeResult() -1, this->index, true);
}

bool TGBOT::TYPE_ENTITIES_MESSAGE::Index(std::string &str, long index){
    return GetType(str, index, this->index, (this->method->getSizeResult() > index));
}

TGBOT::GET_ENTITIES_MESSAGE::GET_ENTITIES_MESSAGE(){};
TGBOT::GET_ENTITIES_MESSAGE::GET_ENTITIES_MESSAGE(VARIABLE &var, METHOD &met) :
offset_entities(var, met),
length_entities(var, met),
type_entities(var, met){
    this->var = &var;
    this->met = &met;
}

TGBOT::OFFSET_ENTITIES_MESSAGE TGBOT::GET_ENTITIES_MESSAGE::offset(long index){
    this->offset_entities.getIndex(index);
    return this->offset_entities;
}

TGBOT::LENGTH_ENTITIES_MESSAGE TGBOT::GET_ENTITIES_MESSAGE::length(long index){
    this->length_entities.getIndex(index);
    return this->length_entities;
}

TGBOT::TYPE_ENTITIES_MESSAGE TGBOT::GET_ENTITIES_MESSAGE::type(long index){
    this->type_entities.getIndex(index);
    return type_entities;
}


TGBOT::ENTITIES_MESSAGE::ENTITIES_MESSAGE(){}
TGBOT::ENTITIES_MESSAGE::ENTITIES_MESSAGE(VARIABLE &var, METHOD &met) : get_entities_mess(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_ENTITIES_MESSAGE TGBOT::ENTITIES_MESSAGE::get(){
    return this->get_entities_mess;
}

// ENTITIES MESSAGE

// ***************** FROM REPLY MESSAGE ******************* //

TGBOT::ID_FROM_REPLY::ID_FROM_REPLY(){};
TGBOT::ID_FROM_REPLY::ID_FROM_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
};

bool TGBOT::ID_FROM_REPLY::GetFromId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["from"].count("id")){
            data = js["result"][index]["message"]["reply_to_message"]["from"]["id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_FROM_REPLY::First(long &data){
    return GetFromId(data, 0, true);
}

bool TGBOT::ID_FROM_REPLY::Last(long &data){
    return GetFromId(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::ID_FROM_REPLY::Index(long &data, long index){
    return GetFromId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::IS_BOT_FROM_REPLY::IS_BOT_FROM_REPLY(){};
TGBOT::IS_BOT_FROM_REPLY::IS_BOT_FROM_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::IS_BOT_FROM_REPLY::GetIsBotFrom(bool &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["from"].count("is_bot")){
            data = js["result"][index]["message"]["reply_to_message"]["from"]["is_bot"].get<bool>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::IS_BOT_FROM_REPLY::First(bool &data){
    return GetIsBotFrom(data, 0, true);
}

bool TGBOT::IS_BOT_FROM_REPLY::Last(bool &data){
    return GetIsBotFrom(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::IS_BOT_FROM_REPLY::Index(bool &data, long index){
    return GetIsBotFrom(data, index, (this->method->getSizeResult() > index));
}

TGBOT::FIRST_NAME_FROM_REPLY::FIRST_NAME_FROM_REPLY(){};
TGBOT::FIRST_NAME_FROM_REPLY::FIRST_NAME_FROM_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FIRST_NAME_FROM_REPLY::GetFirstNameFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["from"].count("first_name")){
            str = js["result"][index]["message"]["reply_to_message"]["from"]["first_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FIRST_NAME_FROM_REPLY::First(std::string &str){
    return GetFirstNameFrom(str, 0, true);
}

bool TGBOT::FIRST_NAME_FROM_REPLY::Last(std::string &str){
    return GetFirstNameFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FIRST_NAME_FROM_REPLY::Index(std::string &str, long index){
    return GetFirstNameFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::LAST_NAME_FROM_REPLY::LAST_NAME_FROM_REPLY(){};
TGBOT::LAST_NAME_FROM_REPLY::LAST_NAME_FROM_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::LAST_NAME_FROM_REPLY::GetLastNameFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["from"].count("last_name")){
            str = js["result"][index]["message"]["reply_to_message"]["from"]["last_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::LAST_NAME_FROM_REPLY::First(std::string &str){
    return GetLastNameFrom(str, 0, true);
}

bool TGBOT::LAST_NAME_FROM_REPLY::Last(std::string &str){
    return GetLastNameFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::LAST_NAME_FROM_REPLY::Index(std::string &str, long index){
    return GetLastNameFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::USERNAME_FROM_REPLY::USERNAME_FROM_REPLY(){};
TGBOT::USERNAME_FROM_REPLY::USERNAME_FROM_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::USERNAME_FROM_REPLY::GetUserNameFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["from"].count("username")){
            str = js["result"][index]["message"]["reply_to_message"]["from"]["username"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::USERNAME_FROM_REPLY::First(std::string &str){
    return GetUserNameFrom(str, 0, true);
}

bool TGBOT::USERNAME_FROM_REPLY::Last(std::string &str){
    return GetUserNameFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::USERNAME_FROM_REPLY::Index(std::string &str, long index){
    return GetUserNameFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::LENG_CODE_FROM_REPLY::LENG_CODE_FROM_REPLY(){};
TGBOT::LENG_CODE_FROM_REPLY::LENG_CODE_FROM_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::LENG_CODE_FROM_REPLY::GetLengCodeFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["from"].count("language_code")){
            str = js["result"][index]["message"]["reply_to_message"]["from"]["language_code"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::LENG_CODE_FROM_REPLY::First(std::string &str){
    return GetLengCodeFrom(str, 0, true);
}

bool TGBOT::LENG_CODE_FROM_REPLY::Last(std::string &str){
    return GetLengCodeFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::LENG_CODE_FROM_REPLY::Index(std::string &str, long index){
    return GetLengCodeFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_FROM_REPLY_MESSAGE::GET_FROM_REPLY_MESSAGE(){};
TGBOT::GET_FROM_REPLY_MESSAGE::GET_FROM_REPLY_MESSAGE(VARIABLE &var, METHOD &met) :
id_from_reply(var, met),
is_bot_from_reply(var, met),
first_name_from_reply(var, met),
last_name_from_reply(var, met),
username_from_reply(var, met),
leng_code_from_reply(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::ID_FROM_REPLY TGBOT::GET_FROM_REPLY_MESSAGE::id(){
    return this->id_from_reply;
}

TGBOT::IS_BOT_FROM_REPLY TGBOT::GET_FROM_REPLY_MESSAGE::is_bot(){
    return this->is_bot_from_reply;
}

TGBOT::FIRST_NAME_FROM_REPLY TGBOT::GET_FROM_REPLY_MESSAGE::first_name(){
    return this->first_name_from_reply;
}

TGBOT::LAST_NAME_FROM_REPLY TGBOT::GET_FROM_REPLY_MESSAGE::last_name(){
    return this->last_name_from_reply;
}

TGBOT::USERNAME_FROM_REPLY TGBOT::GET_FROM_REPLY_MESSAGE::username(){
    return this->username_from_reply;
}

TGBOT::LENG_CODE_FROM_REPLY TGBOT::GET_FROM_REPLY_MESSAGE::language_code(){
    return this->leng_code_from_reply;
}

TGBOT::FROM_REPLY_MESSAGE::FROM_REPLY_MESSAGE(){};
TGBOT::FROM_REPLY_MESSAGE::FROM_REPLY_MESSAGE(VARIABLE &var, METHOD &met) : get_from_reply_message(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_FROM_REPLY_MESSAGE TGBOT::FROM_REPLY_MESSAGE::get(){
    return this->get_from_reply_message;
}
// FROM REPLY MESSAGE

// ***************** CHAT REPLY MESSAGE ******************* //
TGBOT::ID_CHAT_REPLY::ID_CHAT_REPLY(){}
TGBOT::ID_CHAT_REPLY::ID_CHAT_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::ID_CHAT_REPLY::GetChatId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["chat"].count("id")){
            data = js["result"][index]["message"]["reply_to_message"]["chat"]["id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_CHAT_REPLY::First(long &data){
    return GetChatId(data, 0, true);
}

bool TGBOT::ID_CHAT_REPLY::Last(long &data){
    return GetChatId(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::ID_CHAT_REPLY::Index(long &data, long index){
    return GetChatId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::FIRST_NAME_CHAT_REPLY::FIRST_NAME_CHAT_REPLY(){}
TGBOT::FIRST_NAME_CHAT_REPLY::FIRST_NAME_CHAT_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FIRST_NAME_CHAT_REPLY::GetFirstNameChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["chat"].count("first_name")){
            str = js["result"][index]["message"]["reply_to_message"]["chat"]["first_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FIRST_NAME_CHAT_REPLY::First(std::string &str){
    return GetFirstNameChat(str, 0, true);
}

bool TGBOT::FIRST_NAME_CHAT_REPLY::Last(std::string &str){
    return GetFirstNameChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FIRST_NAME_CHAT_REPLY::Index(std::string &str, long index){
    return GetFirstNameChat(str, index, (this->method->getSizeResult() > index));
}

TGBOT::LAST_NAME_CHAT_REPLY::LAST_NAME_CHAT_REPLY(){}
TGBOT::LAST_NAME_CHAT_REPLY::LAST_NAME_CHAT_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::LAST_NAME_CHAT_REPLY::GetLastNameChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["chat"].count("last_name")){
            str = js["result"][index]["message"]["reply_to_message"]["chat"]["last_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::LAST_NAME_CHAT_REPLY::First(std::string &str){
    return GetLastNameChat(str, 0, true);
}

bool TGBOT::LAST_NAME_CHAT_REPLY::Last(std::string &str){
    return GetLastNameChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::LAST_NAME_CHAT_REPLY::Index(std::string &str, long index){
    return GetLastNameChat(str, index, (this->method->getSizeResult() > index));
}

TGBOT::USERNAME_CHAT_REPLY::USERNAME_CHAT_REPLY(){}
TGBOT::USERNAME_CHAT_REPLY::USERNAME_CHAT_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::USERNAME_CHAT_REPLY::GetUsernameChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["chat"].count("username")){
            str = js["result"][index]["message"]["reply_to_message"]["chat"]["username"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::USERNAME_CHAT_REPLY::First(std::string &str){
    return GetUsernameChat(str, 0, true);
}

bool TGBOT::USERNAME_CHAT_REPLY::Last(std::string &str){
    return GetUsernameChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::USERNAME_CHAT_REPLY::Index(std::string &str, long index){
    return GetUsernameChat(str, index, (this->method->getSizeResult() > index));
}

TGBOT::TYPE_CHAT_REPLY::TYPE_CHAT_REPLY(){}
TGBOT::TYPE_CHAT_REPLY::TYPE_CHAT_REPLY(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::TYPE_CHAT_REPLY::GetTypeChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"]["chat"].count("type")){
            str = js["result"][index]["message"]["reply_to_message"]["chat"]["type"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::TYPE_CHAT_REPLY::First(std::string &str){
    return GetTypeChat(str, 0, true);
}

bool TGBOT::TYPE_CHAT_REPLY::Last(std::string &str){
    return GetTypeChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::TYPE_CHAT_REPLY::Index(std::string &str, long index){
    return GetTypeChat(str, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_CHAT_REPLY_MESSAGE::GET_CHAT_REPLY_MESSAGE(){};
TGBOT::GET_CHAT_REPLY_MESSAGE::GET_CHAT_REPLY_MESSAGE(VARIABLE &var, METHOD &met) :
id_chat_reply(var, met),
first_name_chat_reply(var, met),
last_name_chat_reply(var, met),
username_chat_reply(var, met),
type_chat_reply(var, met){
    this->var = &var;
    this->method = &met;
}
TGBOT::ID_CHAT_REPLY TGBOT::GET_CHAT_REPLY_MESSAGE::id(){
    return this->id_chat_reply;
}

TGBOT::FIRST_NAME_CHAT_REPLY TGBOT::GET_CHAT_REPLY_MESSAGE::first_name(){
    return this->first_name_chat_reply;
}

TGBOT::LAST_NAME_CHAT_REPLY TGBOT::GET_CHAT_REPLY_MESSAGE::last_name(){
    return this->last_name_chat_reply;
}

TGBOT::USERNAME_CHAT_REPLY TGBOT::GET_CHAT_REPLY_MESSAGE::username(){
    return username_chat_reply;
}

TGBOT::TYPE_CHAT_REPLY TGBOT::GET_CHAT_REPLY_MESSAGE::type(){
    return this->type_chat_reply;
}

TGBOT::CHAT_REPLY_MESSAGE::CHAT_REPLY_MESSAGE(){};
TGBOT::CHAT_REPLY_MESSAGE::CHAT_REPLY_MESSAGE(VARIABLE &var, METHOD &met) : get_chat_reply_message(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_CHAT_REPLY_MESSAGE TGBOT::CHAT_REPLY_MESSAGE::get(){
    return this->get_chat_reply_message;
}
// CHAT REPLY MESSAGE

// ***************** REPLY_TO_MESSAGE ******************* //
TGBOT::ID_REPLY_MESSAGE::ID_REPLY_MESSAGE(){}
TGBOT::ID_REPLY_MESSAGE::ID_REPLY_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::ID_REPLY_MESSAGE::GetMessageId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"].count("message_id")){
            data = js["result"][index]["message"]["reply_to_message"]["message_id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_REPLY_MESSAGE::First(long &data){
    return GetMessageId(data, 0, true);
}

bool TGBOT::ID_REPLY_MESSAGE::Last(long &data){
    return GetMessageId(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::ID_REPLY_MESSAGE::Index(long &data, long index){
    return GetMessageId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::DATE_REPLY_MESSAGE::DATE_REPLY_MESSAGE(){}
TGBOT::DATE_REPLY_MESSAGE::DATE_REPLY_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::DATE_REPLY_MESSAGE::GetMessageData(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"].count("date")){
            data = js["result"][index]["message"]["reply_to_message"]["date"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::DATE_REPLY_MESSAGE::First(long &data){
    return GetMessageData(data, 0, true);
}

bool TGBOT::DATE_REPLY_MESSAGE::Last(long &data){
    return GetMessageData(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::DATE_REPLY_MESSAGE::Index(long &data, long index){
    return GetMessageData(data, index, (this->method->getSizeResult() > index));
}

TGBOT::TEXT_REPLY_MESSAGE::TEXT_REPLY_MESSAGE(){}
TGBOT::TEXT_REPLY_MESSAGE::TEXT_REPLY_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::TEXT_REPLY_MESSAGE::GetMessageText(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["reply_to_message"].count("text")){
            str = js["result"][index]["message"]["reply_to_message"]["text"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::TEXT_REPLY_MESSAGE::First(std::string &str){
    return GetMessageText(str, 0, true);
}

bool TGBOT::TEXT_REPLY_MESSAGE::Last(std::string &str){
    return GetMessageText(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::TEXT_REPLY_MESSAGE::Index(std::string &str, long index){
    return GetMessageText(str, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_REPLY_TO_MESSAGE::GET_REPLY_TO_MESSAGE(){};
TGBOT::GET_REPLY_TO_MESSAGE::GET_REPLY_TO_MESSAGE(VARIABLE &var, METHOD &met) : id_message(var, met), date_message(var, met), text_message(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::ID_REPLY_MESSAGE TGBOT::GET_REPLY_TO_MESSAGE::id(){
    return this->id_message;;
}

TGBOT::DATE_REPLY_MESSAGE TGBOT::GET_REPLY_TO_MESSAGE::date(){
    return this->date_message;
}

TGBOT::TEXT_REPLY_MESSAGE TGBOT::GET_REPLY_TO_MESSAGE::text(){
    return this->text_message;
}

TGBOT::REPLY_TO_MESSAGE::REPLY_TO_MESSAGE(){}
TGBOT::REPLY_TO_MESSAGE::REPLY_TO_MESSAGE(VARIABLE &var, METHOD &met) :
from_reply_message(var, met),
chat_reply_message(var, met),
get_reply_to_message(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_REPLY_TO_MESSAGE TGBOT::REPLY_TO_MESSAGE::get(){
    return this->get_reply_to_message;
}

TGBOT::FROM_REPLY_MESSAGE TGBOT::REPLY_TO_MESSAGE::from(){
    return this->from_reply_message;
}

TGBOT::CHAT_REPLY_MESSAGE TGBOT::REPLY_TO_MESSAGE::chat(){
    return this->chat_reply_message;
}
// REPLY_TO_MESSAGE

// ***************** VOICE ******************* //

TGBOT::DURATION_VOICE::DURATION_VOICE(){};
TGBOT::DURATION_VOICE::DURATION_VOICE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::DURATION_VOICE::GetDuration(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["voice"].count("duration")){
            data = js["result"][index]["message"]["voice"]["duration"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::DURATION_VOICE::First(long &data){
    return GetDuration(data, 0, true);
}

bool TGBOT::DURATION_VOICE::Last(long &data){
    return GetDuration(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::DURATION_VOICE::Index(long &data, long index){
    return GetDuration(data, index, (this->method->getSizeResult() > index));
}

TGBOT::MIME_TYPE_VOICE::MIME_TYPE_VOICE(){};
TGBOT::MIME_TYPE_VOICE::MIME_TYPE_VOICE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::MIME_TYPE_VOICE::GetMimeType(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["voice"].count("mime_type")){
            str = js["result"][index]["message"]["voice"]["mime_type"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::MIME_TYPE_VOICE::First(std::string &str){
    return GetMimeType(str, 0, true);
}

bool TGBOT::MIME_TYPE_VOICE::Last(std::string &str){
    return GetMimeType(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::MIME_TYPE_VOICE::Index(std::string &str, long index){
    return GetMimeType(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_ID_VOICE::FILE_ID_VOICE(){};
TGBOT::FILE_ID_VOICE::FILE_ID_VOICE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_ID_VOICE::GetFileId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["voice"].count("file_id")){
            str = js["result"][index]["message"]["voice"]["file_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_ID_VOICE::First(std::string &str){
    return GetFileId(str, 0, true);
}

bool TGBOT::FILE_ID_VOICE::Last(std::string &str){
    return GetFileId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_ID_VOICE::Index(std::string &str, long index){
    return GetFileId(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_UNIQUE_ID_VOICE::FILE_UNIQUE_ID_VOICE(){};
TGBOT::FILE_UNIQUE_ID_VOICE::FILE_UNIQUE_ID_VOICE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_UNIQUE_ID_VOICE::GetFileUniqueId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["voice"].count("file_unique_id")){
            str = js["result"][index]["message"]["voice"]["file_unique_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_UNIQUE_ID_VOICE::First(std::string &str){
    return GetFileUniqueId(str, 0, true);
}

bool TGBOT::FILE_UNIQUE_ID_VOICE::Last(std::string &str){
    return GetFileUniqueId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_UNIQUE_ID_VOICE::Index(std::string &str, long index){
    return GetFileUniqueId(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_SIZE_VOICE::FILE_SIZE_VOICE(){};
TGBOT::FILE_SIZE_VOICE::FILE_SIZE_VOICE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_SIZE_VOICE::GetFileSize(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["voice"].count("file_size")){
            data = js["result"][index]["message"]["voice"]["file_size"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_SIZE_VOICE::First(long &data){
    return GetFileSize(data, 0, true);
}

bool TGBOT::FILE_SIZE_VOICE::Last(long &data){
    return GetFileSize(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_SIZE_VOICE::Index(long &data, long index){
    return GetFileSize(data, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_VOICE::GET_VOICE(){};
TGBOT::GET_VOICE::GET_VOICE(VARIABLE &var, METHOD &met) :
duration_voice(var, met),
mime_type_voice(var, met),
file_id_voice(var, met),
file_unique_id_voice(var, met),
file_size_voice(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::DURATION_VOICE TGBOT::GET_VOICE::duration(){
    return this->duration_voice;
}

TGBOT::MIME_TYPE_VOICE TGBOT::GET_VOICE::mime_type(){
    return this->mime_type_voice;
}

TGBOT::FILE_ID_VOICE TGBOT::GET_VOICE::file_id(){
    return this->file_id_voice;
}

TGBOT::FILE_UNIQUE_ID_VOICE TGBOT::GET_VOICE::file_unique_id(){
    return this->file_unique_id_voice;
}

TGBOT::FILE_SIZE_VOICE TGBOT::GET_VOICE::file_size(){
    return this->file_size_voice;
}

TGBOT::VOICE::VOICE(){};
TGBOT::VOICE::VOICE(VARIABLE &var, METHOD &met) : get_voice(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_VOICE TGBOT::VOICE::get(){
    return this->get_voice;
}

// VOICE

// ***************** thumbnail document ******************* //

TGBOT::FILE_ID_THUMBNAIL_DOCUMENT::FILE_ID_THUMBNAIL_DOCUMENT(){}
TGBOT::FILE_ID_THUMBNAIL_DOCUMENT::FILE_ID_THUMBNAIL_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_ID_THUMBNAIL_DOCUMENT::GetFileId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumbnail"].count("file_id")){
            str = js["result"][index]["message"]["document"]["thumbnail"]["file_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_ID_THUMBNAIL_DOCUMENT::First(std::string &str){
    return GetFileId(str, 0, true);
}

bool TGBOT::FILE_ID_THUMBNAIL_DOCUMENT::Last(std::string &str){
    return GetFileId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_ID_THUMBNAIL_DOCUMENT::Index(std::string &str, long index){
    return GetFileId(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT(){}
TGBOT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT::GetUniqueId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumbnail"].count("file_unique_id")){
            str = js["result"][index]["message"]["document"]["thumbnail"]["file_unique_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT::First(std::string &str){
    return GetUniqueId(str, 0, true);
}

bool TGBOT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT::Last(std::string &str){
    return GetUniqueId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT::Index(std::string &str, long index){
    return GetUniqueId(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_SIZE_THUMBNAIL_DOCUMENT::FILE_SIZE_THUMBNAIL_DOCUMENT(){}
TGBOT::FILE_SIZE_THUMBNAIL_DOCUMENT::FILE_SIZE_THUMBNAIL_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_SIZE_THUMBNAIL_DOCUMENT::GetFileSize(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumbnail"].count("file_size")){
            data = js["result"][index]["message"]["document"]["thumbnail"]["file_size"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_SIZE_THUMBNAIL_DOCUMENT::First(long &data){
    return GetFileSize(data, 0, true);
}

bool TGBOT::FILE_SIZE_THUMBNAIL_DOCUMENT::Last(long &data){
    return GetFileSize(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_SIZE_THUMBNAIL_DOCUMENT::Index(long &data, long index){
    return GetFileSize(data, index, (this->method->getSizeResult() > index));
}

TGBOT::WIDTH_THUMBNAIL_DOCUMENT::WIDTH_THUMBNAIL_DOCUMENT(){}
TGBOT::WIDTH_THUMBNAIL_DOCUMENT::WIDTH_THUMBNAIL_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::WIDTH_THUMBNAIL_DOCUMENT::GetWidth(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumbnail"].count("width")){
            data = js["result"][index]["message"]["document"]["thumbnail"]["width"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::WIDTH_THUMBNAIL_DOCUMENT::First(long &data){
    return GetWidth(data, 0, true);
}

bool TGBOT::WIDTH_THUMBNAIL_DOCUMENT::Last(long &data){
    return GetWidth(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::WIDTH_THUMBNAIL_DOCUMENT::Index(long &data, long index){
    return GetWidth(data, index, (this->method->getSizeResult() > index));
}

TGBOT::HEIGHT_THUMBNAIL_DOCUMENT::HEIGHT_THUMBNAIL_DOCUMENT(){}
TGBOT::HEIGHT_THUMBNAIL_DOCUMENT::HEIGHT_THUMBNAIL_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::HEIGHT_THUMBNAIL_DOCUMENT::GetHeight(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumbnail"].count("height")){
            data = js["result"][index]["message"]["document"]["thumbnail"]["height"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::HEIGHT_THUMBNAIL_DOCUMENT::First(long &data){
    return GetHeight(data, 0, true);
}

bool TGBOT::HEIGHT_THUMBNAIL_DOCUMENT::Last(long &data){
    return GetHeight(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::HEIGHT_THUMBNAIL_DOCUMENT::Index(long &data, long index){
    return GetHeight(data, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_THUMBNAIL_DOCUMENT::GET_THUMBNAIL_DOCUMENT(){}
TGBOT::GET_THUMBNAIL_DOCUMENT::GET_THUMBNAIL_DOCUMENT(VARIABLE &var, METHOD &met) :
file_id_thumbnail_doc(var, met),
file_uique_id_thu_doc(var, met),
file_size_thu_doc(var, met),
width_thu_doc(var, met),
height_thu_doc(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::FILE_ID_THUMBNAIL_DOCUMENT TGBOT::GET_THUMBNAIL_DOCUMENT::file_id(){
    return this->file_id_thumbnail_doc;
}

TGBOT::FILE_UIQUE_ID_THUMBNAIL_DOCUMENT TGBOT::GET_THUMBNAIL_DOCUMENT::file_unique_id(){
    return this->file_uique_id_thu_doc;
}

TGBOT::FILE_SIZE_THUMBNAIL_DOCUMENT TGBOT::GET_THUMBNAIL_DOCUMENT::file_size(){
    return this->file_size_thu_doc;
}

TGBOT::WIDTH_THUMBNAIL_DOCUMENT TGBOT::GET_THUMBNAIL_DOCUMENT::width(){
    return this->width_thu_doc;
}

TGBOT::HEIGHT_THUMBNAIL_DOCUMENT TGBOT::GET_THUMBNAIL_DOCUMENT::height(){
    return this->height_thu_doc;
}

TGBOT::THUMBNAIL_DOCUMENT::THUMBNAIL_DOCUMENT(){}
TGBOT::THUMBNAIL_DOCUMENT::THUMBNAIL_DOCUMENT(VARIABLE &var, METHOD &met) : get_thumbnail_doc(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_THUMBNAIL_DOCUMENT TGBOT::THUMBNAIL_DOCUMENT::get(){
    return this->get_thumbnail_doc;
}

// thumbnail document

// ***************** thumb document ******************* //

TGBOT::FILE_ID_THUMB_DOCUMENT::FILE_ID_THUMB_DOCUMENT(){}
TGBOT::FILE_ID_THUMB_DOCUMENT::FILE_ID_THUMB_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_ID_THUMB_DOCUMENT::GetFileId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumb"].count("file_id")){
            str = js["result"][index]["message"]["document"]["thumb"]["file_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_ID_THUMB_DOCUMENT::First(std::string &str){
    return GetFileId(str, 0, true);
}

bool TGBOT::FILE_ID_THUMB_DOCUMENT::Last(std::string &str){
    return GetFileId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_ID_THUMB_DOCUMENT::Index(std::string &str, long index){
    return GetFileId(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_UNIQUE_ID_THUMB_DOCUMENT::FILE_UNIQUE_ID_THUMB_DOCUMENT(){}
TGBOT::FILE_UNIQUE_ID_THUMB_DOCUMENT::FILE_UNIQUE_ID_THUMB_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_UNIQUE_ID_THUMB_DOCUMENT::GetUniqueId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumb"].count("file_unique_id")){
            str = js["result"][index]["message"]["document"]["thumb"]["file_unique_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_UNIQUE_ID_THUMB_DOCUMENT::First(std::string &str){
    return GetUniqueId(str, 0, true);
}

bool TGBOT::FILE_UNIQUE_ID_THUMB_DOCUMENT::Last(std::string &str){
    return GetUniqueId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_UNIQUE_ID_THUMB_DOCUMENT::Index(std::string &str, long index){
    return GetUniqueId(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_SIZE_THUMB_DOCUMENT::FILE_SIZE_THUMB_DOCUMENT(){}
TGBOT::FILE_SIZE_THUMB_DOCUMENT::FILE_SIZE_THUMB_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_SIZE_THUMB_DOCUMENT::GetFileSize(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumb"].count("file_size")){
            data = js["result"][index]["message"]["document"]["thumb"]["file_size"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_SIZE_THUMB_DOCUMENT::First(long &data){
    return GetFileSize(data, 0, true);
}

bool TGBOT::FILE_SIZE_THUMB_DOCUMENT::Last(long &data){
    return GetFileSize(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_SIZE_THUMB_DOCUMENT::Index(long &data, long index){
    return GetFileSize(data, index, (this->method->getSizeResult() > index));
}

TGBOT::WIDTH_THUMB_DOCUMENT::WIDTH_THUMB_DOCUMENT(){}
TGBOT::WIDTH_THUMB_DOCUMENT::WIDTH_THUMB_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::WIDTH_THUMB_DOCUMENT::GetWidth(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumb"].count("width")){
            data = js["result"][index]["message"]["document"]["thumb"]["width"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::WIDTH_THUMB_DOCUMENT::First(long &data){
    return GetWidth(data, 0, true);
}

bool TGBOT::WIDTH_THUMB_DOCUMENT::Last(long &data){
    return GetWidth(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::WIDTH_THUMB_DOCUMENT::Index(long &data, long index){
    return GetWidth(data, index, (this->method->getSizeResult() > index));
}

TGBOT::HEIGHT_THUMB_DOCUMENT::HEIGHT_THUMB_DOCUMENT(){}
TGBOT::HEIGHT_THUMB_DOCUMENT::HEIGHT_THUMB_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::HEIGHT_THUMB_DOCUMENT::GetHeight(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"]["thumb"].count("height")){
            data = js["result"][index]["message"]["document"]["thumb"]["height"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::HEIGHT_THUMB_DOCUMENT::First(long &data){
    return GetHeight(data, 0, true);
}

bool TGBOT::HEIGHT_THUMB_DOCUMENT::Last(long &data){
    return GetHeight(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::HEIGHT_THUMB_DOCUMENT::Index(long &data, long index){
    return GetHeight(data, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_THUMB_DOCUMENT::GET_THUMB_DOCUMENT(){}
TGBOT::GET_THUMB_DOCUMENT::GET_THUMB_DOCUMENT(VARIABLE &var, METHOD &met) :
file_id_thumb_doc(var, met),
file_unique_id_thumb_doc(var, met),
file_size_thumb_doc(var, met),
width_thumb_doc(var, met),
height_thumb_doc(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::FILE_ID_THUMB_DOCUMENT TGBOT::GET_THUMB_DOCUMENT::file_id(){
    return this->file_id_thumb_doc;
}

TGBOT::FILE_UNIQUE_ID_THUMB_DOCUMENT TGBOT::GET_THUMB_DOCUMENT::file_unique_id(){
    return this->file_unique_id_thumb_doc;
}

TGBOT::FILE_SIZE_THUMB_DOCUMENT TGBOT::GET_THUMB_DOCUMENT::file_size(){
    return this->file_size_thumb_doc;
}

TGBOT::WIDTH_THUMB_DOCUMENT TGBOT::GET_THUMB_DOCUMENT::width(){
    return this->width_thumb_doc;
}

TGBOT::HEIGHT_THUMB_DOCUMENT TGBOT::GET_THUMB_DOCUMENT::height(){
    return this->height_thumb_doc;
}

TGBOT::THUMB_DOCUMENT::THUMB_DOCUMENT(){}
TGBOT::THUMB_DOCUMENT::THUMB_DOCUMENT(VARIABLE &var, METHOD &met) : get_thumb_doc(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_THUMB_DOCUMENT TGBOT::THUMB_DOCUMENT::get(){
    return this->get_thumb_doc;
}

// thumb document

// ***************** document ******************* //

TGBOT::FILE_NAME_DOCUMENT::FILE_NAME_DOCUMENT(){}
TGBOT::FILE_NAME_DOCUMENT::FILE_NAME_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_NAME_DOCUMENT::GetFileName(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"].count("file_name")){
            str = js["result"][index]["message"]["document"]["file_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}


bool TGBOT::FILE_NAME_DOCUMENT::First(std::string &str){
    return GetFileName(str, 0, true);
}

bool TGBOT::FILE_NAME_DOCUMENT::Last(std::string &str){
    return GetFileName(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_NAME_DOCUMENT::Index(std::string &str, long index){
    return GetFileName(str, index, (this->method->getSizeResult() > index));
}

TGBOT::MIME_TYPE_DOCUMENT::MIME_TYPE_DOCUMENT(){}
TGBOT::MIME_TYPE_DOCUMENT::MIME_TYPE_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::MIME_TYPE_DOCUMENT::GetMimeType(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"].count("mime_type")){
            str = js["result"][index]["message"]["document"]["mime_type"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}


bool TGBOT::MIME_TYPE_DOCUMENT::First(std::string &str){
    return GetMimeType(str, 0, true);
}

bool TGBOT::MIME_TYPE_DOCUMENT::Last(std::string &str){
    return GetMimeType(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::MIME_TYPE_DOCUMENT::Index(std::string &str, long index){
    return GetMimeType(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_ID_DOCUMENT::FILE_ID_DOCUMENT(){}
TGBOT::FILE_ID_DOCUMENT::FILE_ID_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_ID_DOCUMENT::GetFileId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"].count("file_id")){
            str = js["result"][index]["message"]["document"]["file_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}


bool TGBOT::FILE_ID_DOCUMENT::First(std::string &str){
    return GetFileId(str, 0, true);
}

bool TGBOT::FILE_ID_DOCUMENT::Last(std::string &str){
    return GetFileId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_ID_DOCUMENT::Index(std::string &str, long index){
    return GetFileId(str, index, (this->method->getSizeResult() > index));
}

TGBOT::FILE_UNIQUE_ID_DOCUMENT::FILE_UNIQUE_ID_DOCUMENT(){}
TGBOT::FILE_UNIQUE_ID_DOCUMENT::FILE_UNIQUE_ID_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_UNIQUE_ID_DOCUMENT::GetFileUniqueId(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"].count("file_unique_id")){
            str = js["result"][index]["message"]["document"]["file_unique_id"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}


bool TGBOT::FILE_UNIQUE_ID_DOCUMENT::First(std::string &str){
    return GetFileUniqueId(str, 0, true);
}

bool TGBOT::FILE_UNIQUE_ID_DOCUMENT::Last(std::string &str){
    return GetFileUniqueId(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_UNIQUE_ID_DOCUMENT::Index(std::string &str, long index){
    return GetFileUniqueId(str, index, (this->method->getSizeResult() > index));
}


TGBOT::FILE_SIZE_DOCUMENT::FILE_SIZE_DOCUMENT(){}
TGBOT::FILE_SIZE_DOCUMENT::FILE_SIZE_DOCUMENT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FILE_SIZE_DOCUMENT::GetFileSize(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["document"].count("file_size")){
            data = js["result"][index]["message"]["document"]["file_size"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FILE_SIZE_DOCUMENT::First(long &data){
    return GetFileSize(data, 0, true);
}

bool TGBOT::FILE_SIZE_DOCUMENT::Last(long &data){
    return GetFileSize(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::FILE_SIZE_DOCUMENT::Index(long &data, long index){
    return GetFileSize(data, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_DOCUMENT::GET_DOCUMENT(){}
TGBOT::GET_DOCUMENT::GET_DOCUMENT(VARIABLE &var, METHOD &met) :
file_name_doc(var, met),
mime_type_doc(var, met),
file_id_doc(var, met),
file_unique_id_doc(var, met),
file_size_doc(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::FILE_NAME_DOCUMENT TGBOT::GET_DOCUMENT::file_name(){
    return this->file_name_doc;
}

TGBOT::MIME_TYPE_DOCUMENT TGBOT::GET_DOCUMENT::mime_type(){
    return this->mime_type_doc;
}

TGBOT::FILE_ID_DOCUMENT TGBOT::GET_DOCUMENT::file_id(){
    return this->file_id_doc;
}

TGBOT::FILE_UNIQUE_ID_DOCUMENT TGBOT::GET_DOCUMENT::file_unique_id(){
    return this->file_unique_id_doc;
}

TGBOT::FILE_SIZE_DOCUMENT TGBOT::GET_DOCUMENT::file_size(){
    return this->file_size_doc;
}

TGBOT::DOCUMENT::DOCUMENT(){};
TGBOT::DOCUMENT::DOCUMENT(VARIABLE &var, METHOD &met) : get_document(var, met), thumbnail(var, met), thumb(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_DOCUMENT TGBOT::DOCUMENT::get(){
    return this->get_document;
}

TGBOT::THUMBNAIL_DOCUMENT TGBOT::DOCUMENT::Thumbnail(){
    return this->thumbnail;
}

TGBOT::THUMB_DOCUMENT TGBOT::DOCUMENT::Thumb(){
    return this->thumb;
}

// document

// ***************** FORWARD_FROM ******************* //

TGBOT::ID_FORWARD_FROM::ID_FORWARD_FROM(){};
TGBOT::ID_FORWARD_FROM::ID_FORWARD_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
};

bool TGBOT::ID_FORWARD_FROM::GetForwardId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["forward_from"].count("id")){
            data = js["result"][index]["message"]["forward_from"]["id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_FORWARD_FROM::First(long &data){
    return GetForwardId(data, 0, true);
}

bool TGBOT::ID_FORWARD_FROM::Last(long &data){
    return GetForwardId(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::ID_FORWARD_FROM::Index(long &data, long index){
    return GetForwardId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::IS_BOT_FORWARD_FROM::IS_BOT_FORWARD_FROM(){};
TGBOT::IS_BOT_FORWARD_FROM::IS_BOT_FORWARD_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
};

bool TGBOT::IS_BOT_FORWARD_FROM::GetIsBotForward(bool &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["forward_from"].count("is_bot")){
            data = js["result"][index]["message"]["forward_from"]["is_bot"].get<bool>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::IS_BOT_FORWARD_FROM::First(bool &data){
    return GetIsBotForward(data, 0, true);
}

bool TGBOT::IS_BOT_FORWARD_FROM::Last(bool &data){
    return GetIsBotForward(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::IS_BOT_FORWARD_FROM::Index(bool &data, long index){
    return GetIsBotForward(data, index, (this->method->getSizeResult() > index));
}

TGBOT::FIRST_NAME_FORWARD_FROM::FIRST_NAME_FORWARD_FROM(){};
TGBOT::FIRST_NAME_FORWARD_FROM::FIRST_NAME_FORWARD_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
};

bool TGBOT::FIRST_NAME_FORWARD_FROM::GetFirstNameForward(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["forward_from"].count("first_name")){
            str = js["result"][index]["message"]["forward_from"]["first_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FIRST_NAME_FORWARD_FROM::First(std::string &str){
    return GetFirstNameForward(str, 0, true);
}

bool TGBOT::FIRST_NAME_FORWARD_FROM::Last(std::string &str){
    return GetFirstNameForward(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FIRST_NAME_FORWARD_FROM::Index(std::string &str, long index){
    return GetFirstNameForward(str, index, (this->method->getSizeResult() > index));
}

TGBOT::LAST_NAME_FORWARD_FROM::LAST_NAME_FORWARD_FROM(){};
TGBOT::LAST_NAME_FORWARD_FROM::LAST_NAME_FORWARD_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
};

bool TGBOT::LAST_NAME_FORWARD_FROM::GetLastNameForward(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["forward_from"].count("last_name")){
            str = js["result"][index]["message"]["forward_from"]["last_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::LAST_NAME_FORWARD_FROM::First(std::string &str){
    return GetLastNameForward(str, 0, true);
}

bool TGBOT::LAST_NAME_FORWARD_FROM::Last(std::string &str){
    return GetLastNameForward(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::LAST_NAME_FORWARD_FROM::Index(std::string &str, long index){
    return GetLastNameForward(str, index, (this->method->getSizeResult() > index));
}

TGBOT::USERNAME_FORWARD_FROM::USERNAME_FORWARD_FROM(){};
TGBOT::USERNAME_FORWARD_FROM::USERNAME_FORWARD_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
};

bool TGBOT::USERNAME_FORWARD_FROM::GetUserNameForward(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["forward_from"].count("username")){
            str = js["result"][index]["message"]["forward_from"]["username"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::USERNAME_FORWARD_FROM::First(std::string &str){
    return GetUserNameForward(str, 0, true);
}

bool TGBOT::USERNAME_FORWARD_FROM::Last(std::string &str){
    return GetUserNameForward(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::USERNAME_FORWARD_FROM::Index(std::string &str, long index){
    return GetUserNameForward(str, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_FORWARD_FROM::GET_FORWARD_FROM(){};
TGBOT::GET_FORWARD_FROM::GET_FORWARD_FROM(VARIABLE &var, METHOD &met) :
id_rorward_from(var, met),
is_bot_forward_from(var, met),
first_name_forward_from(var, met),
last_name_forward_wrom(var, met),
username_forward_from(var, met){
    this->var = &var;
    this->method = &met;
};

TGBOT::ID_FORWARD_FROM TGBOT::GET_FORWARD_FROM::id(){
    return this->id_rorward_from;
}

TGBOT::IS_BOT_FORWARD_FROM TGBOT::GET_FORWARD_FROM::is_bot(){
    return this->is_bot_forward_from;
}

TGBOT::FIRST_NAME_FORWARD_FROM TGBOT::GET_FORWARD_FROM::first_name(){
    return this->first_name_forward_from;
}

TGBOT::LAST_NAME_FORWARD_FROM TGBOT::GET_FORWARD_FROM::last_name(){
    return this->last_name_forward_wrom;
}

TGBOT::USERNAME_FORWARD_FROM TGBOT::GET_FORWARD_FROM::username(){
    return this->username_forward_from;
}

TGBOT::FORWARD_FROM::FORWARD_FROM(){};
TGBOT::FORWARD_FROM::FORWARD_FROM(VARIABLE &var, METHOD &met) : get_forward_from(var, met){
    this->var = &var;
    this->method = &met;
};

TGBOT::GET_FORWARD_FROM TGBOT::FORWARD_FROM::get(){
    return this->get_forward_from;;
}

// FORWARD_FROM

// ***************** FROM ******************* //
TGBOT::ID_FROM::ID_FROM(){};
TGBOT::ID_FROM::ID_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::ID_FROM::GetFromId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["from"].count("id")){
            data = js["result"][index]["message"]["from"]["id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_FROM::First(long &data){
    return GetFromId(data, 0, true);
}

bool TGBOT::ID_FROM::Last(long &data){
    return GetFromId(data, this->method->getSizeResult() -1, true);
}
bool TGBOT::ID_FROM::Index(long &data, long index){
    return GetFromId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::IS_BOT_FROM::IS_BOT_FROM(){};
TGBOT::IS_BOT_FROM::IS_BOT_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::IS_BOT_FROM::GetIsBotFrom(bool &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["from"].count("is_bot")){
            data = js["result"][index]["message"]["from"]["is_bot"].get<bool>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::IS_BOT_FROM::First(bool &data){
    return GetIsBotFrom(data, 0, true);
}

bool TGBOT::IS_BOT_FROM::Last(bool &data){
    return GetIsBotFrom(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::IS_BOT_FROM::Index(bool &data, long index){
    return GetIsBotFrom(data, index, (this->method->getSizeResult() > index));
}

TGBOT::FIRST_NAME_FROM::FIRST_NAME_FROM(){};
TGBOT::FIRST_NAME_FROM::FIRST_NAME_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FIRST_NAME_FROM::GetFirstNameFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["from"].count("first_name")){
            str = js["result"][index]["message"]["from"]["first_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FIRST_NAME_FROM::First(std::string &str){
    return GetFirstNameFrom(str, 0, true);
}

bool TGBOT::FIRST_NAME_FROM::Last(std::string &str){
    return GetFirstNameFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FIRST_NAME_FROM::Index(std::string &str, long index){
    return GetFirstNameFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::LAST_NAME_FROM::LAST_NAME_FROM(){};
TGBOT::LAST_NAME_FROM::LAST_NAME_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::LAST_NAME_FROM::GetLastNameFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["from"].count("last_name")){
            str = js["result"][index]["message"]["from"]["last_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::LAST_NAME_FROM::First(std::string &str){
    return GetLastNameFrom(str, 0, true);
}

bool TGBOT::LAST_NAME_FROM::Last(std::string &str){
    return GetLastNameFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::LAST_NAME_FROM::Index(std::string &str, long index){
    return GetLastNameFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::USERNAME_FROM::USERNAME_FROM(){};
TGBOT::USERNAME_FROM::USERNAME_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::USERNAME_FROM::GetUsernameFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["from"].count("username")){
            str = js["result"][index]["message"]["from"]["username"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::USERNAME_FROM::First(std::string &str){
    return GetUsernameFrom(str, 0, true);
}

bool TGBOT::USERNAME_FROM::Last(std::string &str){
    return GetUsernameFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::USERNAME_FROM::Index(std::string &str, long index){
    return GetUsernameFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::LANG_CODE_FROM::LANG_CODE_FROM(){};
TGBOT::LANG_CODE_FROM::LANG_CODE_FROM(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::LANG_CODE_FROM::GetLengCodeFrom(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["from"].count("language_code")){
            str = js["result"][index]["message"]["from"]["language_code"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::LANG_CODE_FROM::First(std::string &str){
    return GetLengCodeFrom(str, 0, true);
}

bool TGBOT::LANG_CODE_FROM::Last(std::string &str){
    return GetLengCodeFrom(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::LANG_CODE_FROM::Index(std::string &str, long index){
    return GetLengCodeFrom(str, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_FROM::GET_FROM(){};
TGBOT::GET_FROM::GET_FROM(VARIABLE &var, METHOD &met) :
id_from(var, met),
is_bot_from(var, met),
first_name_from(var, met),
last_name_from(var, met),
username_from(var, met),
leng_code_from(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::ID_FROM TGBOT::GET_FROM::id(){
    return this->id_from;
}

TGBOT::IS_BOT_FROM TGBOT::GET_FROM::is_bot(){
    return this->is_bot_from;
}

TGBOT::FIRST_NAME_FROM TGBOT::GET_FROM::first_name(){
    return this->first_name_from;
}

TGBOT::LAST_NAME_FROM TGBOT::GET_FROM::last_name(){
    return this->last_name_from;
}

TGBOT::USERNAME_FROM TGBOT::GET_FROM::username(){
    return this->username_from;
}

TGBOT::LANG_CODE_FROM TGBOT::GET_FROM::language_code(){
    return this->leng_code_from;
}

TGBOT::FROM::FROM(){}
TGBOT::FROM::FROM(VARIABLE &var, METHOD &met) : get_from(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_FROM TGBOT::FROM::get(){
    return this->get_from;
}
// FROM

// ***************** CHAT ******************* //
TGBOT::ID_CHAT::ID_CHAT(){};
TGBOT::ID_CHAT::ID_CHAT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::ID_CHAT::GetChatId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["chat"].count("id")){
            data = js["result"][index]["message"]["chat"]["id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_CHAT::First(long &data){
    return GetChatId(data, 0, true);
}

bool TGBOT::ID_CHAT::Last(long &data){
    return GetChatId(data, this->method->getSizeResult() -1, true);
}
bool TGBOT::ID_CHAT::Index(long &data, long index){
    return GetChatId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::FIRST_NAME_CHAT::FIRST_NAME_CHAT(){};
TGBOT::FIRST_NAME_CHAT::FIRST_NAME_CHAT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::FIRST_NAME_CHAT::GetFirstNameChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["chat"].count("first_name")){
            str = js["result"][index]["message"]["chat"]["first_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FIRST_NAME_CHAT::First(std::string &str){
    return GetFirstNameChat(str, 0, true);
}

bool TGBOT::FIRST_NAME_CHAT::Last(std::string &str){
    return GetFirstNameChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::FIRST_NAME_CHAT::Index(std::string &str, long index){
    return GetFirstNameChat(str, index, (this->method->getSizeResult() > index));
}

TGBOT::LAST_NAME_CHAT::LAST_NAME_CHAT(){};
TGBOT::LAST_NAME_CHAT::LAST_NAME_CHAT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::LAST_NAME_CHAT::GetLastNameChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["chat"].count("last_name")){
            str = js["result"][index]["message"]["chat"]["last_name"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::LAST_NAME_CHAT::First(std::string &str){
    return GetLastNameChat(str, 0, true);
}

bool TGBOT::LAST_NAME_CHAT::Last(std::string &str){
    return GetLastNameChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::LAST_NAME_CHAT::Index(std::string &str, long index){
    return GetLastNameChat(str, index, (this->method->getSizeResult() > index));
}


TGBOT::USERNAME_CHAT::USERNAME_CHAT(){};
TGBOT::USERNAME_CHAT::USERNAME_CHAT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::USERNAME_CHAT::GetUsernameChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["chat"].count("username")){
            str = js["result"][index]["message"]["chat"]["username"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::USERNAME_CHAT::First(std::string &str){
    return GetUsernameChat(str, 0, true);
}

bool TGBOT::USERNAME_CHAT::Last(std::string &str){
    return GetUsernameChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::USERNAME_CHAT::Index(std::string &str, long index){
    return GetUsernameChat(str, index, (this->method->getSizeResult() > index));
}


TGBOT::TYPE_CHAT::TYPE_CHAT(){};
TGBOT::TYPE_CHAT::TYPE_CHAT(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::TYPE_CHAT::GetTypeChat(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"]["chat"].count("type")){
            str = js["result"][index]["message"]["chat"]["type"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::TYPE_CHAT::First(std::string &str){
    return GetTypeChat(str, 0, true);
}

bool TGBOT::TYPE_CHAT::Last(std::string &str){
    return GetTypeChat(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::TYPE_CHAT::Index(std::string &str, long index){
    return GetTypeChat(str, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_CHAT::GET_CHAT(){}
TGBOT::GET_CHAT::GET_CHAT(VARIABLE &var, METHOD &met) :
id_chat(var, met),
first_name_chat(var, met),
last_name_chat(var, met),
username_chat(var, met),
type_chat(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::ID_CHAT TGBOT::GET_CHAT::id(){
    return this->id_chat;
}

TGBOT::FIRST_NAME_CHAT TGBOT::GET_CHAT::first_name(){
    return this->first_name_chat;
}

TGBOT::LAST_NAME_CHAT TGBOT::GET_CHAT::last_name(){
    return this->last_name_chat;
}

TGBOT::USERNAME_CHAT TGBOT::GET_CHAT::username(){
    return this->username_chat;
}

TGBOT::TYPE_CHAT TGBOT::GET_CHAT::type(){
    return this->type_chat;
}

TGBOT::CHAT::CHAT(){};
TGBOT::CHAT::CHAT(VARIABLE &var, METHOD &met) : get_chat(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_CHAT TGBOT::CHAT::get(){
    return this->get_chat;
}
// CHAT

// ***************** MESSAGE ******************* //
TGBOT::TEXT_MESSAGE::TEXT_MESSAGE(){};
TGBOT::TEXT_MESSAGE::TEXT_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::TEXT_MESSAGE::GetMessageText(std::string &str, long index, bool isStatus){
    this->method->strClear(str);
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"].count("text")){
            str = js["result"][index]["message"]["text"].get<std::string>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::TEXT_MESSAGE::First(std::string &str){
    return GetMessageText(str, 0, true);
}

bool TGBOT::TEXT_MESSAGE::Last(std::string &str){
    return GetMessageText(str, this->method->getSizeResult() -1, true);
}

bool TGBOT::TEXT_MESSAGE::Index(std::string &str, long index){
    return GetMessageText(str, index, (this->method->getSizeResult() > index));
}

bool TGBOT::FORWARD_DATE_MESSAGE::GetMessageForwardData(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"].count("forward_date")){
            data = js["result"][index]["message"]["forward_date"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::FORWARD_DATE_MESSAGE::First(long &data){
    return GetMessageForwardData(data, 0, true);
}

bool TGBOT::FORWARD_DATE_MESSAGE::Last(long &data){
    return GetMessageForwardData(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::FORWARD_DATE_MESSAGE::Index(long &data, long index){
    return GetMessageForwardData(data, index, (this->method->getSizeResult() > index));
}

TGBOT::ID_MESSAGE::ID_MESSAGE(){};
TGBOT::ID_MESSAGE::ID_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

TGBOT::DATE_MESSAGE::DATE_MESSAGE(){};
TGBOT::DATE_MESSAGE::DATE_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::DATE_MESSAGE::GetMessageData(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"].count("date")){
            data = js["result"][index]["message"]["date"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::DATE_MESSAGE::First(long &data){
    return GetMessageData(data, 0, true);
}

bool TGBOT::DATE_MESSAGE::Last(long &data){
    return GetMessageData(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::DATE_MESSAGE::Index(long &data, long index){
    return GetMessageData(data, index, (this->method->getSizeResult() > index));
}

TGBOT::FORWARD_DATE_MESSAGE::FORWARD_DATE_MESSAGE(){};
TGBOT::FORWARD_DATE_MESSAGE::FORWARD_DATE_MESSAGE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::ID_MESSAGE::GetMessageId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index]["message"].count("message_id")){
            data = js["result"][index]["message"]["message_id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_MESSAGE::First(long &data){
    return GetMessageId(data, 0, true);
}

bool TGBOT::ID_MESSAGE::Last(long &data){
    return GetMessageId(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::ID_MESSAGE::Index(long &data, long index){
    return GetMessageId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_MESSAGE::GET_MESSAGE(){};
TGBOT::GET_MESSAGE::GET_MESSAGE(VARIABLE &var, METHOD &met) : id_message(var, met), date_message(var, met), forward_date_message(var, met), text_message(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::ID_MESSAGE TGBOT::GET_MESSAGE::id(){
    return this->id_message;
}
TGBOT::DATE_MESSAGE TGBOT::GET_MESSAGE::date(){
    return this->date_message;
}
TGBOT::TEXT_MESSAGE TGBOT::GET_MESSAGE::text(){
    return this->text_message;
}

TGBOT::FORWARD_DATE_MESSAGE TGBOT::GET_MESSAGE::forward_date(){
    return this->forward_date_message;
}

TGBOT::MESSAGE::MESSAGE(){};
TGBOT::MESSAGE::MESSAGE(VARIABLE &var, METHOD &met) :
from(var, met),
chat(var, met),
reply_to_message(var, met),
forward_from(var, met),
voice(var, met),
document(var, met),
entities(var, met),
get_message(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_MESSAGE TGBOT::MESSAGE::get(){
    return this->get_message;
}

TGBOT::FROM TGBOT::MESSAGE::From(){
    return this->from;
};
TGBOT::CHAT TGBOT::MESSAGE::Chat(){
    return this->chat;
}

TGBOT::REPLY_TO_MESSAGE TGBOT::MESSAGE::Reply(){
    return this->reply_to_message;
}

TGBOT::FORWARD_FROM TGBOT::MESSAGE::Forward(){
    return this->forward_from;
}

TGBOT::VOICE TGBOT::MESSAGE::Voice(){
    return this->voice;
}

TGBOT::DOCUMENT TGBOT::MESSAGE::Document(){
    return this->document;
}

TGBOT::ENTITIES_MESSAGE TGBOT::MESSAGE::Entities(){
    return this->entities;
}
// MESSAGE

// ***************** RESULT ******************* //
TGBOT::ID_UPDATE::ID_UPDATE(){};
TGBOT::ID_UPDATE::ID_UPDATE(VARIABLE &var, METHOD &met){
    this->var = &var;
    this->method = &met;
}

bool TGBOT::ID_UPDATE::GetUpdateId(long &data, long index, bool isStatus){
    if ((this->method->getIsStatusBot() && this->method->getSizeResult() && (index +1)) && isStatus){
        JsonHpp js = JsonHpp::parse(this->var->updates);
        if (js["result"][index].count("update_id")){
            data = js["result"][index]["update_id"].get<long>();
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool TGBOT::ID_UPDATE::First(long &data){
    return GetUpdateId(data, 0, true);
}

bool TGBOT::ID_UPDATE::Last(long &data){
    return GetUpdateId(data, this->method->getSizeResult() -1, true);
}

bool TGBOT::ID_UPDATE::Index(long &data, long index){
    return GetUpdateId(data, index, (this->method->getSizeResult() > index));
}

TGBOT::GET_RESULT::GET_RESULT(){};
TGBOT::GET_RESULT::GET_RESULT(VARIABLE &var, METHOD &met) : id_update(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::ID_UPDATE TGBOT::GET_RESULT::id(){
    return this->id_update;
}

TGBOT::RESULT::RESULT(){};
TGBOT::RESULT::RESULT(VARIABLE &var, METHOD &met) : message(var, met), get_result(var, met){
    this->var = &var;
    this->method = &met;
}

TGBOT::GET_RESULT TGBOT::RESULT::get(){
    return this->get_result;
}

TGBOT::MESSAGE TGBOT::RESULT::Message(){
    return this->message;
}

// RESULT

// ***************** TGbot ******************* //
TGBOT::TGbot::TGbot(){};
TGBOT::TGbot::TGbot(const char *token) : result(this->var, this->method), event(this->var, this->method), method(this->var){
    this->var.token.append(token);
}

TGBOT::RESULT TGBOT::TGbot::Result(){
    return this->result;
}

TGBOT::EVENT TGBOT::TGbot::Event(){
    return this->event;
}

void TGBOT::TGbot::getStatusBot(){
    this->method.getStatusBot();
}

long TGBOT::TGbot::getSizeResult(){
    return this->method.getSizeResult();
}

void TGBOT::TGbot::Sleep_for(int sleep){
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
}
// TGbot
