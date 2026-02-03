//
//  receive.hpp
//  TGbot
//
//  Created by DiCode77.
//

#ifndef updates_hpp
#define updates_hpp

#include <string>
#include <curl/curl.h>
#include <thread>
#include <chrono>

typedef struct SVideo{
    long width = -1;
    long height = -1;
    long duration = -1;
} sizeVideo;

class CURL_UPDATES{
    int timeOutHttp = 10;
    SVideo size_video;
    
public:
    CURL_UPDATES();
    void timeout(int);
    CURLcode getCurlUpdates(std::string, std::string&);
    CURLcode sendHttpMessage(std::string, std::string, std::string&);
    CURLcode getCurlEvent(std::string, std::string&);
    CURLcode sendFile(std::string, std::string, std::string, std::string&, std::string, std::string);
    CURLcode sendReplyFile(std::string, std::string, std::string, std::string, std::string&, std::string, std::string);
    void videoSize(long, long);
    std::string GetEncryptHttpTxt(std::string);
    void NewSleep(long);

private:
    static size_t getResponsetoString(void*, size_t, size_t, void*);
    void Sleep_for(long);
    void addParameterCurl(CURL*, curl_mime*, std::string, std::string&);
    void addFileToForm(curl_mime*, const char*, std::string, bool);
    bool verifyType(std::string);
};

#endif /* updates_hpp */
