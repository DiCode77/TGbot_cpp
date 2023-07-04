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

class CURL_UPDATES{
    int timeOutHttp;
    int sleep_for;
    
public:
    CURL_UPDATES();
    void timeout(int);
    void sleep(int);
    CURLcode getCurlUpdates(std::string, std::string&);
    CURLcode sendHttpMessage(std::string, std::string, std::string&);
    CURLcode getCurlEvent(std::string, std::string&);
    CURLcode sendFile(std::string, std::string, std::string, std::string&, std::string, std::string);

private:
    static size_t getResponsetoString(void*, size_t, size_t, void*);
    void Sleep_for(int);
};

#endif /* updates_hpp */
