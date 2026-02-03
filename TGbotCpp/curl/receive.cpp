//
//  receive.cpp
//  TGbot
//
//  Created by DiCode77.
//

#include "receive.hpp"

CURL_UPDATES::CURL_UPDATES(){
    this->timeOutHttp = 10;
}

void CURL_UPDATES::timeout(int time){
    this->timeOutHttp = time;
}

size_t CURL_UPDATES::getResponsetoString(void* contents, size_t size, size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size *nmemb;
}

CURLcode CURL_UPDATES::getCurlUpdates(std::string url, std::string &data){
    if (data.length())
        data.clear();

    CURL *curl = nullptr;
    CURLcode response = CURLE_FAILED_INIT;
    
    curl = curl_easy_init();
    
    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        response = curl_easy_perform(curl);
    }
    
    if (curl != nullptr)
        curl_easy_cleanup(curl);
    
    return response;
}

CURLcode CURL_UPDATES::sendHttpMessage(std::string url, std::string text, std::string &data){
    if (data.length())
        data.clear();
    
    CURL *curl = nullptr;
    CURLcode response = CURLE_FAILED_INIT;
    
    curl = curl_easy_init();
    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, text.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, text.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        response = curl_easy_perform(curl);
    }
    
    if (curl != nullptr)
        curl_easy_cleanup(curl);
    
    return response;
}

CURLcode CURL_UPDATES::getCurlEvent(std::string url, std::string &data){
    if (data.length())
        data.clear();

    CURL *curl = nullptr;
    CURLcode response = CURLE_FAILED_INIT;
    
    curl = curl_easy_init();
    
    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        response = curl_easy_perform(curl);
    }
    
    if (curl != nullptr)
        curl_easy_cleanup(curl);
    
    return response;
}

void CURL_UPDATES::addParameterCurl(CURL *curl, curl_mime *formpost, std::string url, std::string &res){
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeOutHttp);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
}

void CURL_UPDATES::addFileToForm(curl_mime *mime, const char *identifier, std::string info, bool isFile){
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_name(part, identifier);
    
    if (isFile){
        curl_mime_filedata(part, info.c_str());
    }
    else{
        curl_mime_data(part, info.c_str(), CURL_ZERO_TERMINATED);
    }
}

bool CURL_UPDATES::verifyType(std::string type){
    return (type == "photo" || type == "document" || type == "audio");
}

CURLcode CURL_UPDATES::sendFile(std::string url, std::string chatId, std::string path, std::string &update, std::string type, std::string caption){
    CURLcode res_curl = CURLE_FAILED_INIT;
    std::string result;
    CURL *curl = curl_easy_init();
    
    if (curl){
        curl_mime *mime = curl_mime_init(curl);
        
        if (!mime){
            curl_easy_cleanup(curl);
            return CURLE_OUT_OF_MEMORY;
        }
        
        if (verifyType(type)){
            addFileToForm(mime, "chat_id", chatId,  false);
            addFileToForm(mime, "caption", caption, false);
            addFileToForm(mime, type.c_str(), path, true);
        }
        else if (type == "video"){
            addFileToForm(mime, "chat_id", chatId,  false);
            addFileToForm(mime, "caption", caption, false);
            addFileToForm(mime, "width",   std::to_string(this->size_video.width),  false);
            addFileToForm(mime, "height",  std::to_string(this->size_video.height), false);
            addFileToForm(mime, type.c_str(), path, true);
        }
        
        addParameterCurl(curl, mime, url, result);
        res_curl = curl_easy_perform(curl);
        if (res_curl == CURLE_OK) {
            update = result;
        }
        
        curl_mime_free(mime);
    }
    
    if (curl != nullptr)
        curl_easy_cleanup(curl);
    
    return res_curl;
}

CURLcode CURL_UPDATES::sendReplyFile(std::string url, std::string chatId, std::string messId, std::string path, std::string &update, std::string type, std::string caption){
    CURLcode res_curl = CURLE_FAILED_INIT;
    std::string result;
    CURL *curl = curl_easy_init();
    
    if (curl){
        curl_mime *mime = curl_mime_init(curl);
        
        if (!mime){
            curl_easy_cleanup(curl);
            return CURLE_OUT_OF_MEMORY;
        }
        
        if (verifyType(type)){
            addFileToForm(mime, "chat_id", chatId, false);
            addFileToForm(mime, "caption", caption, false);
            addFileToForm(mime, "reply_to_message_id", messId, false);
            addFileToForm(mime, type.c_str(), path, true);
        }
        else if (type == "video"){
            addFileToForm(mime, "chat_id", chatId, false);
            addFileToForm(mime, "caption", caption, false);
            addFileToForm(mime, "width",   std::to_string(this->size_video.width), false);
            addFileToForm(mime, "height", std::to_string(this->size_video.height), false);
            addFileToForm(mime, "reply_to_message_id", messId, false);
            addFileToForm(mime, type.c_str(), path, true);
        }
        
        addParameterCurl(curl, mime, url, result);
        res_curl = curl_easy_perform(curl);
        if (res_curl == CURLE_OK) {
            update = result;
        }
        
        curl_mime_free(mime);
    }
    
    if (curl != nullptr)
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

void CURL_UPDATES::NewSleep(long sp){
    this->Sleep_for(sp);
}

void CURL_UPDATES::Sleep_for(long sleep){
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
}

