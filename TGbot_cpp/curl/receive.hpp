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
    int timeOutHttp;
    int sleep_for;
    SVideo size_video;
    
public:
    CURL_UPDATES();
    void timeout(int);
    void sleep(int);
    CURLcode getCurlUpdates(std::string, std::string&);
    CURLcode sendHttpMessage(std::string, std::string, std::string&);
    CURLcode getCurlEvent(std::string, std::string&);
    CURLcode sendFile(std::string, std::string, std::string, std::string&, std::string, std::string);
    CURLcode sendReplyFile(std::string, std::string, std::string, std::string, std::string&, std::string, std::string);
    void videoSize(long, long);

private:
    static size_t getResponsetoString(void*, size_t, size_t, void*);
    void Sleep_for(int);
    void addParameterCurl(CURL*, curl_httppost*, std::string, std::string&);
    void addFileToForm(curl_httppost**, curl_httppost**, const char*, int, const char*);
    bool verifyType(std::string);
};

#endif /* updates_hpp */
