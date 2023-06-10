//
//  TGbot.hpp
//  TGbot
//
//  Created by DiCode77.
//

#ifndef TGbot_hpp
#define TGbot_hpp
#include <iostream>
#include <curl/curl.h>
#include <json.hpp>
#include <thread>
#include <chrono>
#include "receive.hpp"

#define PROTOCOL "https://"
#define DOMIN_NAME "api.telegram.org/"
#define PATH "bot"
#define GET_UPDATES "/getUpdates"
#define SEND_MESSAGE "/sendMessage?"
#define CHAT_ID "chat_id="
#define TEXTS "&text="
#define REPLY_MESSAGE_ID "&reply_to_message_id="
#define OFFSET "?offset="

namespace TGBOT{

using JsonHpp = nlohmann::json;

typedef struct VARIABLE{
    std::string updates;
    std::string token;
    bool status[2]{};
    size_t index = 0;
} VARIABLE;


// ***************** METHOD ******************* //
class METHOD{
    VARIABLE *var;
    long size_result;
public:
    METHOD();
    METHOD(VARIABLE&);
    bool getStatusBot();
    bool getIsStatusBot();
    long getSizeResult();
    void strClear(std::string&);
    bool getUpdateStatus(std::string&);
};

// ***************** EVENT ******************* //
class EVENT_UPDATE{
    VARIABLE *var;
    METHOD *method;
    CURL_UPDATES *curl_update;
    
public:
    EVENT_UPDATE();
    EVENT_UPDATE(VARIABLE&, METHOD&, CURL_UPDATES&);
    void Update();
    bool UpdateAndisStatus();
    
private:
    void isUpdate();
};

class SEND_EVENT{
    VARIABLE *var;
    METHOD *method;
    CURL_UPDATES *curl_update;
    
public:
    SEND_EVENT();
    SEND_EVENT(VARIABLE&, METHOD&, CURL_UPDATES&);
    
    bool sendMessage(long chat_id, std::string text);
    bool sendReplyMessage(long chat_id, long message_id, std::string text);
};

class CLEAR_UPDATE_EVENT{
    VARIABLE *var;
    METHOD *method;
    CURL_UPDATES *curl_update;
    
public:
    CLEAR_UPDATE_EVENT();
    CLEAR_UPDATE_EVENT(VARIABLE&, METHOD&, CURL_UPDATES&);
    bool First(long);
    bool All(long);
};

class CLEAR_EVENT{
    VARIABLE *var;
    METHOD *method;
    CURL_UPDATES *curl_update;
    CLEAR_UPDATE_EVENT clear_update_event;
    
public:
    CLEAR_EVENT();
    CLEAR_EVENT(VARIABLE&, METHOD&, CURL_UPDATES&);
    CLEAR_UPDATE_EVENT updates();
};

class EVENT{
    VARIABLE *var;
    METHOD *method;
    EVENT_UPDATE event_update;
    CURL_UPDATES curl_update;
    SEND_EVENT send_event;
    CLEAR_EVENT clear_event;
    
public:
    EVENT();
    EVENT(VARIABLE&, METHOD&);
    void timeOut(int);
    void sleep(int);
    EVENT_UPDATE getUpdate();
    SEND_EVENT send();
    CLEAR_EVENT clear();
};
// EVENT

// ***************** ENTITIES MESSAGE ******************* //

class OFFSET_ENTITIES_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    long index;
    
public:
    OFFSET_ENTITIES_MESSAGE();
    OFFSET_ENTITIES_MESSAGE(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetOffset(long&, long, long, bool);
    void getIndex(long);
    
    friend class GET_ENTITIES_MESSAGE;
};

class LENGTH_ENTITIES_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    long index;
    
public:
    LENGTH_ENTITIES_MESSAGE();
    LENGTH_ENTITIES_MESSAGE(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetLength(long&, long, long, bool);
    void getIndex(long);
    
    friend class GET_ENTITIES_MESSAGE;
};

class TYPE_ENTITIES_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    long index;
    
public:
    TYPE_ENTITIES_MESSAGE();
    TYPE_ENTITIES_MESSAGE(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetType(std::string&, long, long, bool);
    void getIndex(long);
    
    friend class GET_ENTITIES_MESSAGE;
};

class GET_ENTITIES_MESSAGE{
    VARIABLE *var;
    METHOD *met;
    OFFSET_ENTITIES_MESSAGE offset_entities;
    LENGTH_ENTITIES_MESSAGE length_entities;
    TYPE_ENTITIES_MESSAGE type_entities;
public:
    GET_ENTITIES_MESSAGE();
    GET_ENTITIES_MESSAGE(VARIABLE &var, METHOD &met);
    
    OFFSET_ENTITIES_MESSAGE offset(long index);
    LENGTH_ENTITIES_MESSAGE length(long index);
    TYPE_ENTITIES_MESSAGE type(long index);
};

class ENTITIES_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    GET_ENTITIES_MESSAGE get_entities_mess;

public:
    ENTITIES_MESSAGE();
    ENTITIES_MESSAGE(VARIABLE&, METHOD&);
    GET_ENTITIES_MESSAGE get();
    bool is();
};

//

// ***************** FROM REPLY MESSAGE ******************* //
class ID_FROM_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    ID_FROM_REPLY();
    ID_FROM_REPLY(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetFromId(long&, long, bool);
};

class IS_BOT_FROM_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    IS_BOT_FROM_REPLY();
    IS_BOT_FROM_REPLY(VARIABLE&, METHOD&);
    bool isBool(bool&);
    
private:
    bool GetIsBotFrom(bool&, long, bool);
};

class FIRST_NAME_FROM_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    FIRST_NAME_FROM_REPLY();
    FIRST_NAME_FROM_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetFirstNameFrom(std::string&, long, bool);
};

class LAST_NAME_FROM_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    LAST_NAME_FROM_REPLY();
    LAST_NAME_FROM_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetLastNameFrom(std::string&, long, bool);
};

class USERNAME_FROM_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    USERNAME_FROM_REPLY();
    USERNAME_FROM_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetUserNameFrom(std::string&, long, bool);
};

class LENG_CODE_FROM_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    LENG_CODE_FROM_REPLY();
    LENG_CODE_FROM_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetLengCodeFrom(std::string&, long, bool);
};


class GET_FROM_REPLY_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    ID_FROM_REPLY id_from_reply;
    IS_BOT_FROM_REPLY is_bot_from_reply;
    FIRST_NAME_FROM_REPLY first_name_from_reply;
    LAST_NAME_FROM_REPLY last_name_from_reply;
    USERNAME_FROM_REPLY username_from_reply;
    LENG_CODE_FROM_REPLY leng_code_from_reply;
public:
    GET_FROM_REPLY_MESSAGE();
    GET_FROM_REPLY_MESSAGE(VARIABLE&, METHOD&);
    ID_FROM_REPLY id();
    IS_BOT_FROM_REPLY is_bot();
    FIRST_NAME_FROM_REPLY first_name();
    LAST_NAME_FROM_REPLY last_name();
    USERNAME_FROM_REPLY username();
    LENG_CODE_FROM_REPLY language_code();
};

class FROM_REPLY_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    GET_FROM_REPLY_MESSAGE get_from_reply_message;
public:
    FROM_REPLY_MESSAGE();
    FROM_REPLY_MESSAGE(VARIABLE&, METHOD&);
    GET_FROM_REPLY_MESSAGE get();
    bool is();
};
// FROM REPLY MESSAGE

// ***************** CHAT REPLY MESSAGE ******************* //
class ID_CHAT_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    ID_CHAT_REPLY();
    ID_CHAT_REPLY(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetChatId(long&, long, bool);
};

class FIRST_NAME_CHAT_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    FIRST_NAME_CHAT_REPLY();
    FIRST_NAME_CHAT_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetFirstNameChat(std::string&, long, bool);
};

class LAST_NAME_CHAT_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    LAST_NAME_CHAT_REPLY();
    LAST_NAME_CHAT_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetLastNameChat(std::string&, long, bool);
};

class USERNAME_CHAT_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    USERNAME_CHAT_REPLY();
    USERNAME_CHAT_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetUsernameChat(std::string&, long, bool);
};

class TYPE_CHAT_REPLY{
    VARIABLE *var;
    METHOD *method;
public:
    TYPE_CHAT_REPLY();
    TYPE_CHAT_REPLY(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetTypeChat(std::string&, long, bool);
};

class GET_CHAT_REPLY_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    ID_CHAT_REPLY id_chat_reply;
    FIRST_NAME_CHAT_REPLY first_name_chat_reply;
    LAST_NAME_CHAT_REPLY last_name_chat_reply;
    USERNAME_CHAT_REPLY username_chat_reply;
    TYPE_CHAT_REPLY type_chat_reply;
public:
    GET_CHAT_REPLY_MESSAGE();
    GET_CHAT_REPLY_MESSAGE(VARIABLE&, METHOD&);
    
    ID_CHAT_REPLY id();
    FIRST_NAME_CHAT_REPLY first_name();
    LAST_NAME_CHAT_REPLY last_name();
    USERNAME_CHAT_REPLY username();
    TYPE_CHAT_REPLY type();
};

class CHAT_REPLY_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    GET_CHAT_REPLY_MESSAGE get_chat_reply_message;
public:
    CHAT_REPLY_MESSAGE();
    CHAT_REPLY_MESSAGE(VARIABLE&, METHOD&);
    GET_CHAT_REPLY_MESSAGE get();
    bool is();

};
// CHAT REPLY MESSAGE

// ***************** REPLY MESSAGE ******************* //
class ID_REPLY_MESSAGE{
    VARIABLE *var;
    METHOD *method;
public:
    ID_REPLY_MESSAGE();
    ID_REPLY_MESSAGE(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetMessageId(long&, long, bool);
};

class DATE_REPLY_MESSAGE{
    VARIABLE *var;
    METHOD *method;
public:
    DATE_REPLY_MESSAGE();
    DATE_REPLY_MESSAGE(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetMessageData(long&, long, bool);
};

class TEXT_REPLY_MESSAGE{
    VARIABLE *var;
    METHOD *method;
public:
    TEXT_REPLY_MESSAGE();
    TEXT_REPLY_MESSAGE(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetMessageText(std::string&, long, bool);
};

class GET_REPLY_TO_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    ID_REPLY_MESSAGE id_message;
    DATE_REPLY_MESSAGE date_message;
    TEXT_REPLY_MESSAGE text_message;
    
public:
    GET_REPLY_TO_MESSAGE();
    GET_REPLY_TO_MESSAGE(VARIABLE&, METHOD&);
    ID_REPLY_MESSAGE id();
    DATE_REPLY_MESSAGE date();
    TEXT_REPLY_MESSAGE text();
};

class REPLY_TO_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    GET_REPLY_TO_MESSAGE get_reply_to_message;
    FROM_REPLY_MESSAGE from_reply_message;
    CHAT_REPLY_MESSAGE chat_reply_message;
    
public:
    REPLY_TO_MESSAGE();
    REPLY_TO_MESSAGE(VARIABLE&, METHOD&);
    GET_REPLY_TO_MESSAGE get();
    FROM_REPLY_MESSAGE from();
    CHAT_REPLY_MESSAGE chat();
    bool is();
};
// REPLY MESSAGE

// ***************** VOICE ******************* //

class DURATION_VOICE{
    VARIABLE *var;
    METHOD *method;
    
public:
    DURATION_VOICE();
    DURATION_VOICE(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetDuration(long&, long, bool);
};

class MIME_TYPE_VOICE{
    VARIABLE *var;
    METHOD *method;
    
public:
    MIME_TYPE_VOICE();
    MIME_TYPE_VOICE(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetMimeType(std::string&, long, bool);
};

class FILE_ID_VOICE{
    VARIABLE *var;
    METHOD *method;
    
public:
    FILE_ID_VOICE();
    FILE_ID_VOICE(VARIABLE &var, METHOD &met);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetFileId(std::string&, long, bool);
};

class FILE_UNIQUE_ID_VOICE{
    VARIABLE *var;
    METHOD *method;
    
public:
    FILE_UNIQUE_ID_VOICE();
    FILE_UNIQUE_ID_VOICE(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetFileUniqueId(std::string&, long, bool);
};

class FILE_SIZE_VOICE{
    VARIABLE *var;
    METHOD *method;
    
public:
    FILE_SIZE_VOICE();
    FILE_SIZE_VOICE(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetFileSize(long&, long, bool);
};

class GET_VOICE{
    VARIABLE *var;
    METHOD *method;
    DURATION_VOICE duration_voice;
    MIME_TYPE_VOICE mime_type_voice;
    FILE_ID_VOICE file_id_voice;
    FILE_UNIQUE_ID_VOICE file_unique_id_voice;
    FILE_SIZE_VOICE file_size_voice;
    
public:
    GET_VOICE();
    GET_VOICE(VARIABLE&, METHOD&);
    DURATION_VOICE duration();
    MIME_TYPE_VOICE mime_type();
    FILE_ID_VOICE file_id();
    FILE_UNIQUE_ID_VOICE file_unique_id();
    FILE_SIZE_VOICE file_size();
};

class VOICE{
    VARIABLE *var;
    METHOD *method;
    GET_VOICE get_voice;
    
public:
    VOICE();
    VOICE(VARIABLE&, METHOD&);
    GET_VOICE get();
    bool is();
};

// VOICE

// ***************** thumbnail document ******************* //

class FILE_ID_THUMBNAIL_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_ID_THUMBNAIL_DOCUMENT();
    FILE_ID_THUMBNAIL_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetFileId(std::string&, long, bool);
};

class FILE_UIQUE_ID_THUMBNAIL_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_UIQUE_ID_THUMBNAIL_DOCUMENT();
    FILE_UIQUE_ID_THUMBNAIL_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetUniqueId(std::string&, long, bool);
};

class FILE_SIZE_THUMBNAIL_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_SIZE_THUMBNAIL_DOCUMENT();
    FILE_SIZE_THUMBNAIL_DOCUMENT(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetFileSize(long&, long, bool);
};

class WIDTH_THUMBNAIL_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    WIDTH_THUMBNAIL_DOCUMENT();
    WIDTH_THUMBNAIL_DOCUMENT(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetWidth(long&, long, bool);
};

class HEIGHT_THUMBNAIL_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    HEIGHT_THUMBNAIL_DOCUMENT();
    HEIGHT_THUMBNAIL_DOCUMENT(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetHeight(long&, long, bool);
};

class GET_THUMBNAIL_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
    FILE_ID_THUMBNAIL_DOCUMENT file_id_thumbnail_doc;
    FILE_UIQUE_ID_THUMBNAIL_DOCUMENT file_uique_id_thu_doc;
    FILE_SIZE_THUMBNAIL_DOCUMENT file_size_thu_doc;
    WIDTH_THUMBNAIL_DOCUMENT width_thu_doc;
    HEIGHT_THUMBNAIL_DOCUMENT height_thu_doc;
public:
    GET_THUMBNAIL_DOCUMENT();
    GET_THUMBNAIL_DOCUMENT(VARIABLE&, METHOD&);
    
    FILE_ID_THUMBNAIL_DOCUMENT file_id();
    FILE_UIQUE_ID_THUMBNAIL_DOCUMENT file_unique_id();
    FILE_SIZE_THUMBNAIL_DOCUMENT file_size();
    WIDTH_THUMBNAIL_DOCUMENT width();
    HEIGHT_THUMBNAIL_DOCUMENT height();
};

class THUMBNAIL_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
    GET_THUMBNAIL_DOCUMENT get_thumbnail_doc;
public:
    THUMBNAIL_DOCUMENT();
    THUMBNAIL_DOCUMENT(VARIABLE&, METHOD&);
    GET_THUMBNAIL_DOCUMENT get();
};
// thumbnail document

// ***************** thumb document ******************* //
class FILE_ID_THUMB_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_ID_THUMB_DOCUMENT();
    FILE_ID_THUMB_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetFileId(std::string&, long, bool);
};

class FILE_UNIQUE_ID_THUMB_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_UNIQUE_ID_THUMB_DOCUMENT();
    FILE_UNIQUE_ID_THUMB_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetUniqueId(std::string&, long, bool);
};

class FILE_SIZE_THUMB_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_SIZE_THUMB_DOCUMENT();
    FILE_SIZE_THUMB_DOCUMENT(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetFileSize(long&, long, bool);
};

class WIDTH_THUMB_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    WIDTH_THUMB_DOCUMENT();
    WIDTH_THUMB_DOCUMENT(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetWidth(long&, long, bool);
};

class HEIGHT_THUMB_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    HEIGHT_THUMB_DOCUMENT();
    HEIGHT_THUMB_DOCUMENT(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetHeight(long&, long, bool);
};

class GET_THUMB_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
    
    FILE_ID_THUMB_DOCUMENT file_id_thumb_doc;
    FILE_UNIQUE_ID_THUMB_DOCUMENT file_unique_id_thumb_doc;
    FILE_SIZE_THUMB_DOCUMENT file_size_thumb_doc;
    WIDTH_THUMB_DOCUMENT width_thumb_doc;
    HEIGHT_THUMB_DOCUMENT height_thumb_doc;
public:
    GET_THUMB_DOCUMENT();
    GET_THUMB_DOCUMENT(VARIABLE&, METHOD&);
    
    FILE_ID_THUMB_DOCUMENT file_id();
    FILE_UNIQUE_ID_THUMB_DOCUMENT file_unique_id();
    FILE_SIZE_THUMB_DOCUMENT file_size();
    WIDTH_THUMB_DOCUMENT width();
    HEIGHT_THUMB_DOCUMENT height();
};

class THUMB_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
    GET_THUMB_DOCUMENT get_thumb_doc;
public:
    THUMB_DOCUMENT();
    THUMB_DOCUMENT(VARIABLE&, METHOD&);
    GET_THUMB_DOCUMENT get();
};
// humb document

// ***************** document ******************* //


class FILE_NAME_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_NAME_DOCUMENT();
    FILE_NAME_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetFileName(std::string&, long, bool);
};

class MIME_TYPE_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    MIME_TYPE_DOCUMENT();
    MIME_TYPE_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetMimeType(std::string&, long, bool);
};

class FILE_ID_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_ID_DOCUMENT();
    FILE_ID_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetFileId(std::string&, long, bool);
};

class FILE_UNIQUE_ID_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_UNIQUE_ID_DOCUMENT();
    FILE_UNIQUE_ID_DOCUMENT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetFileUniqueId(std::string&, long, bool);
};

class FILE_SIZE_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
public:
    FILE_SIZE_DOCUMENT();
    FILE_SIZE_DOCUMENT(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetFileSize(long&, long, bool);
};

class GET_DOCUMENT{
    VARIABLE *var;
    METHOD *method;
    FILE_NAME_DOCUMENT file_name_doc;
    MIME_TYPE_DOCUMENT mime_type_doc;
    FILE_ID_DOCUMENT file_id_doc;
    FILE_UNIQUE_ID_DOCUMENT file_unique_id_doc;
    FILE_SIZE_DOCUMENT file_size_doc;
public:
    GET_DOCUMENT();
    GET_DOCUMENT(VARIABLE&, METHOD&);
    FILE_NAME_DOCUMENT file_name();
    MIME_TYPE_DOCUMENT mime_type();
    FILE_ID_DOCUMENT file_id();
    FILE_UNIQUE_ID_DOCUMENT file_unique_id();
    FILE_SIZE_DOCUMENT file_size();
};

class DOCUMENT{
    VARIABLE *var;
    METHOD *method;
    GET_DOCUMENT get_document;
    THUMBNAIL_DOCUMENT thumbnail;
    THUMB_DOCUMENT thumb;
public:
    DOCUMENT();
    DOCUMENT(VARIABLE&, METHOD&);
    GET_DOCUMENT get();
    THUMBNAIL_DOCUMENT Thumbnail();
    THUMB_DOCUMENT Thumb();
    bool is();
};
// document

// ***************** FORWARD_FROM ******************* //

class ID_FORWARD_FROM{
    VARIABLE *var;
    METHOD *method;
public:
    ID_FORWARD_FROM();
    ID_FORWARD_FROM(VARIABLE&, METHOD&);
    bool isNum(long&);
    
private:
    bool GetForwardId(long&, long, bool);
};

class IS_BOT_FORWARD_FROM{
    VARIABLE *var;
    METHOD *method;
public:
    IS_BOT_FORWARD_FROM();
    IS_BOT_FORWARD_FROM(VARIABLE&, METHOD&);
    bool First(bool&);
    bool Last(bool&);
    bool Index(bool&, long);
    bool isBool(bool&);
    
private:
    bool GetIsBotForward(bool&, long, bool);
};

class FIRST_NAME_FORWARD_FROM{
    VARIABLE *var;
    METHOD *method;
public:
    FIRST_NAME_FORWARD_FROM();
    FIRST_NAME_FORWARD_FROM(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetFirstNameForward(std::string&, long, bool);
};

class LAST_NAME_FORWARD_FROM{
    VARIABLE *var;
    METHOD *method;
public:
    LAST_NAME_FORWARD_FROM();
    LAST_NAME_FORWARD_FROM(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetLastNameForward(std::string&, long, bool);
};

class USERNAME_FORWARD_FROM{
    VARIABLE *var;
    METHOD *method;
public:
    USERNAME_FORWARD_FROM();
    USERNAME_FORWARD_FROM(VARIABLE&, METHOD&);
    bool isString(std::string&);
    
private:
    bool GetUserNameForward(std::string&, long, bool);
};

class GET_FORWARD_FROM{
    VARIABLE *var;
    METHOD *method;
    ID_FORWARD_FROM id_rorward_from;
    IS_BOT_FORWARD_FROM is_bot_forward_from;
    FIRST_NAME_FORWARD_FROM first_name_forward_from;
    LAST_NAME_FORWARD_FROM last_name_forward_wrom;
    USERNAME_FORWARD_FROM username_forward_from;
    
public:
    GET_FORWARD_FROM();
    GET_FORWARD_FROM(VARIABLE&, METHOD&);
    
    ID_FORWARD_FROM id();
    IS_BOT_FORWARD_FROM is_bot();
    FIRST_NAME_FORWARD_FROM first_name();
    LAST_NAME_FORWARD_FROM last_name();
    USERNAME_FORWARD_FROM username();
};

class FORWARD_FROM{
    VARIABLE *var;
    METHOD *method;
    GET_FORWARD_FROM get_forward_from;
    
public:
    FORWARD_FROM();
    FORWARD_FROM(VARIABLE&, METHOD&);
    GET_FORWARD_FROM get();
    bool is();
};
// FORWARD_FROM

// ***************** FROM ******************* //
class ID_FROM{
    VARIABLE *var;
    METHOD *method;
    
public:
    ID_FROM();
    ID_FROM(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetFromId(long&, long, bool);
};

class IS_BOT_FROM{
    VARIABLE *var;
    METHOD *method;
    
public:
    IS_BOT_FROM();
    IS_BOT_FROM(VARIABLE&, METHOD&);
    bool isBool(bool&);
    bool is();
    
private:
    bool GetIsBotFrom(bool&, long, bool);
};

class FIRST_NAME_FROM{
    VARIABLE *var;
    METHOD *method;
    
public:
    FIRST_NAME_FROM();
    FIRST_NAME_FROM(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetFirstNameFrom(std::string&, long, bool);
};

class LAST_NAME_FROM{
    VARIABLE *var;
    METHOD *method;
    
public:
    LAST_NAME_FROM();
    LAST_NAME_FROM(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetLastNameFrom(std::string&, long, bool);
};

class USERNAME_FROM{
    VARIABLE *var;
    METHOD *method;
    
public:
    USERNAME_FROM();
    USERNAME_FROM(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetUsernameFrom(std::string&, long, bool);
};

class LANG_CODE_FROM{
    VARIABLE *var;
    METHOD *method;
    
public:
    LANG_CODE_FROM();
    LANG_CODE_FROM(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetLengCodeFrom(std::string&, long, bool);
};

class GET_FROM{
    VARIABLE *var;
    METHOD *method;
    ID_FROM id_from;
    IS_BOT_FROM is_bot_from;
    FIRST_NAME_FROM first_name_from;
    LAST_NAME_FROM last_name_from;
    USERNAME_FROM username_from;
    LANG_CODE_FROM leng_code_from;
    
public:
    GET_FROM();
    GET_FROM(VARIABLE&, METHOD&);
    ID_FROM id();
    IS_BOT_FROM is_bot();
    FIRST_NAME_FROM first_name();
    LAST_NAME_FROM last_name();
    USERNAME_FROM username();
    LANG_CODE_FROM language_code();

};

class FROM{
    VARIABLE *var;
    METHOD *method;
    GET_FROM get_from;
public:
    FROM();
    FROM(VARIABLE&, METHOD&);
    GET_FROM get();
    bool is();
    
};
// FROM

// ***************** CHAT ******************* //
class ID_CHAT{
    VARIABLE *var;
    METHOD *method;
    
public:
    ID_CHAT();
    ID_CHAT(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetChatId(long&, long, bool);
};

class FIRST_NAME_CHAT{
    VARIABLE *var;
    METHOD *method;
    
public:
    FIRST_NAME_CHAT();
    FIRST_NAME_CHAT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetFirstNameChat(std::string&, long, bool);
};

class LAST_NAME_CHAT{
    VARIABLE *var;
    METHOD *method;
    
public:
    LAST_NAME_CHAT();
    LAST_NAME_CHAT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetLastNameChat(std::string&, long, bool);
};

class USERNAME_CHAT{
    VARIABLE *var;
    METHOD *method;
    
public:
    USERNAME_CHAT();
    USERNAME_CHAT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetUsernameChat(std::string&, long, bool);
};

class TYPE_CHAT{
    VARIABLE *var;
    METHOD *method;
    
public:
    TYPE_CHAT();
    TYPE_CHAT(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetTypeChat(std::string&, long, bool);
};

class GET_CHAT{
    VARIABLE *var;
    METHOD *method;
    ID_CHAT id_chat;
    FIRST_NAME_CHAT first_name_chat;
    LAST_NAME_CHAT last_name_chat;
    USERNAME_CHAT username_chat;
    TYPE_CHAT type_chat;
public:
    GET_CHAT();
    GET_CHAT(VARIABLE &var, METHOD &met);
    
    ID_CHAT id();
    FIRST_NAME_CHAT first_name();
    LAST_NAME_CHAT last_name();
    USERNAME_CHAT username();
    TYPE_CHAT type();
    
};

class CHAT{
    VARIABLE *var;
    METHOD *method;
    GET_CHAT get_chat;
public:
    CHAT();
    CHAT(VARIABLE&, METHOD&);
    GET_CHAT get();
    bool is();
};
// CHAT

// ***************** MESSAGE ******************* //
class TEXT_MESSAGE{
    VARIABLE *var;
    METHOD *method;
public:
    TEXT_MESSAGE();
    TEXT_MESSAGE(VARIABLE&, METHOD&);
    bool isString(std::string&);
    bool is();
    
private:
    bool GetMessageText(std::string&, long, bool);
};

class FORWARD_DATE_MESSAGE{
    VARIABLE *var;
    METHOD *method;
public:
    FORWARD_DATE_MESSAGE();
    FORWARD_DATE_MESSAGE(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetMessageForwardData(long&, long, bool);
};

class DATE_MESSAGE{
    VARIABLE *var;
    METHOD *method;
public:
    DATE_MESSAGE();
    DATE_MESSAGE(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetMessageData(long&, long, bool);
};

class ID_MESSAGE{
    VARIABLE *var;
    METHOD *method;
public:
    ID_MESSAGE();
    ID_MESSAGE(VARIABLE&, METHOD&);
    bool isNum(long&);
    bool is();
    
private:
    bool GetMessageId(long&, long, bool);
};

class GET_MESSAGE{
    VARIABLE *var;
    METHOD *method;
    ID_MESSAGE id_message;
    DATE_MESSAGE date_message;
    FORWARD_DATE_MESSAGE forward_date_message;
    TEXT_MESSAGE text_message;
    
public:
    GET_MESSAGE();
    GET_MESSAGE(VARIABLE&, METHOD&);
    ID_MESSAGE id();
    DATE_MESSAGE date();
    TEXT_MESSAGE text();
    FORWARD_DATE_MESSAGE forward_date();
};

class MESSAGE{
    FROM from;
    CHAT chat;
    REPLY_TO_MESSAGE reply_to_message;
    FORWARD_FROM forward_from;
    VOICE voice;
    DOCUMENT document;
    ENTITIES_MESSAGE entities;
    VARIABLE *var;
    METHOD *method;
    GET_MESSAGE get_message;
    
public:
    MESSAGE();
    MESSAGE(VARIABLE&, METHOD&);
    GET_MESSAGE get();
    FROM From();
    CHAT Chat();
    REPLY_TO_MESSAGE Reply();
    FORWARD_FROM Forward();
    VOICE Voice();
    DOCUMENT Document();
    ENTITIES_MESSAGE Entities();
};
// MESSAGE

// ***************** RESULT ******************* //
class ID_UPDATE{
    VARIABLE *var;
    METHOD *method;
public:
    ID_UPDATE();
    ID_UPDATE(VARIABLE&, METHOD&);
    
    bool isNum(long&);
private:
    bool GetUpdateId(long&, long, bool);
};

class GET_RESULT{
    VARIABLE *var;
    METHOD *method;
    ID_UPDATE id_update;
    
public:
    GET_RESULT();
    GET_RESULT(VARIABLE&, METHOD&);
    ID_UPDATE id();
};

class RESULT{
    MESSAGE message;
    VARIABLE *var;
    METHOD *method;
    GET_RESULT get_result;
public:
    RESULT();
    RESULT(VARIABLE&, METHOD&);
    GET_RESULT get();
    MESSAGE Message();
};
// RESULT

// ***************** TGbot ******************* //

class TGbot{
    RESULT result;
    VARIABLE var;
    EVENT event;
    METHOD method;
    
public:
    TGbot();
    TGbot(const char*);
    RESULT Result(long index);
    EVENT Event();
    void getStatusBot();
    long getSizeResult();
    void Sleep_for(int);
};
// TGbot

}


#endif /* TGbot_hpp */
