//
//  receive.cpp
//  TGbot
//
//  Created by DiCode77.
//

#include "receive.hpp"

CURL_UPDATES::CURL_UPDATES(){
    this->timeOutHttp = 2;
    this->sleep_for = 0;
    
}

void CURL_UPDATES::timeout(int time){
    this->timeOutHttp = time;
}

void CURL_UPDATES::sleep(int sleep){
    this->sleep_for = sleep;
}

size_t CURL_UPDATES::getResponsetoString(void* contents, size_t size, size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size *nmemb;
}

CURLcode CURL_UPDATES::getCurlUpdates(std::string url, std::string &data){
    if (data.length())
        data.clear();

    CURL *curl;
    CURLcode response;
    
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    Sleep_for(this->sleep_for);
    
    return response;
}

CURLcode CURL_UPDATES::sendHttpMessage(std::string url, std::string text, std::string &data){
    if (data.length())
        data.clear();
    
    CURL *curl;
    CURLcode response;
    
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, text.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, text.size());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 0L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return response;
}

CURLcode CURL_UPDATES::getCurlEvent(std::string url, std::string &data){
    if (data.length())
        data.clear();

    CURL *curl;
    CURLcode response;
    
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return response;
}

void CURL_UPDATES::addParameterCurl(CURL *curl, curl_mime *formpost, std::string url, std::string &res){
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
}

void CURL_UPDATES::addFileToForm(curl_mime *mime, const char *identifier, const char *info, bool isFile){
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_name(part, identifier);
    
    if (isFile){
        curl_mime_filedata(part, info);
    }
    else{
        curl_mime_data(part, info, CURL_ZERO_TERMINATED);
    }
}

bool CURL_UPDATES::verifyType(std::string type){
    return (type == "photo" || type == "document" || type == "audio");
}

CURLcode CURL_UPDATES::sendFile(std::string url, std::string chatId, std::string path, std::string &update, std::string type, std::string caption){
    CURLcode res_curl;
    std::string result;
    CURL *curl = curl_easy_init();
    
    curl_mime *mime = curl_mime_init(curl);
    
    if (verifyType(type)){
        addFileToForm(mime, "chat_id", chatId.c_str(), false);
        addFileToForm(mime, "caption", caption.c_str(), false);
        addFileToForm(mime, type.c_str(), path.c_str(), true);
    }
    else if (type == "video"){
        addFileToForm(mime, "chat_id", chatId.c_str(), false);
        addFileToForm(mime, "caption", caption.c_str(), false);
        addFileToForm(mime, "width",   std::to_string(this->size_video.width).c_str(), false);
        addFileToForm(mime, "height",  std::to_string(this->size_video.height).c_str(), false);
        addFileToForm(mime, type.c_str(), path.c_str(), true);
    }
    
    addParameterCurl(curl, mime, url, result);
    res_curl = curl_easy_perform(curl);
    if (res_curl == CURLE_OK) {
        update = result;
    }

    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    
    return res_curl;
}

CURLcode CURL_UPDATES::sendReplyFile(std::string url, std::string chatId, std::string messId, std::string path, std::string &update, std::string type, std::string caption){
    CURLcode res_curl;
    std::string result;
    CURL *curl = curl_easy_init();
    
    curl_mime *mime = curl_mime_init(curl);

    if (verifyType(type)){
        addFileToForm(mime, "chat_id", chatId.c_str(), false);
        addFileToForm(mime, "caption", caption.c_str(), false);
        addFileToForm(mime, "reply_to_message_id", messId.c_str(), false);
        addFileToForm(mime, type.c_str(), path.c_str(), true);
    }
    else if (type == "video"){
        addFileToForm(mime, "chat_id", chatId.c_str(), false);
        addFileToForm(mime, "caption", caption.c_str(), false);
        addFileToForm(mime, "width",   std::to_string(this->size_video.width).c_str(), false);
        addFileToForm(mime, "height", std::to_string(this->size_video.height).c_str(), false);
        addFileToForm(mime, "reply_to_message_id", messId.c_str(), false);
        addFileToForm(mime, type.c_str(), path.c_str(), true);
    }

    addParameterCurl(curl, mime, url, result);
    res_curl = curl_easy_perform(curl);
    if (res_curl == CURLE_OK) {
        update = result;
    }

    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    
    return res_curl;
}

void CURL_UPDATES::videoSize(long x, long y){
    this->size_video.width = x;
    this->size_video.height = y;
}

std::string CURL_UPDATES::GetEncryptHttpTxt(std::string txt){
    CURL *curl = curl_easy_init();
    std::string txt_enc;
    if (curl){
        char *enc = curl_easy_escape(curl, txt.c_str(), static_cast<int>(txt.length()));
        if (enc){
            txt_enc.append(enc);
            curl_free(enc);
        }
        curl_easy_cleanup(curl);
    }
    return txt_enc;
}

void CURL_UPDATES::Sleep_for(int sleep){
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
}

