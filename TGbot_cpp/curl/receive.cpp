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

void CURL_UPDATES::Sleep_for(int sleep){
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
}

