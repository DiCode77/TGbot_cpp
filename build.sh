#!/bin/bash

TBOT_REPO_URL="https://github.com/DiCode77/TGbot_cpp.git"
JSON_REPO_URL="https://github.com/nlohmann/json/releases/latest/download/json.hpp"

DIR_NAME_REPO="TGbot_cpp"
DIR_NAME_PRJ="TGbotCpp"
DIR_NAME_RES="Resources"
DIR_NAME_RES_NLHNN="Resources/nlohmann"
DIR_NAME_BUILD="Build"

if [[ ! -d "$DIR_NAME_PRJ" ]]; then
   if [[ -d "$DIR_NAME_REPO" ]]; then
       rm -rf -- "$DIR_NAME_REPO"
       mkdir "$DIR_NAME_REPO"
       cd "$DIR_NAME_REPO"
    else
        mkdir "$DIR_NAME_REPO"
        cd "$DIR_NAME_REPO"
    fi
    
    git clone "$TBOT_REPO_URL"
    cd "$DIR_NAME_REPO"
    
    if [ ! -d "$DIR_NAME_RES" ]; then
        mkdir "$DIR_NAME_RES"
        mkdir "$DIR_NAME_RES_NLHNN"
        
        cd "$DIR_NAME_RES_NLHNN"
        curl -fsSL "$JSON_REPO_URL" -o json.hpp
        cd ../..
    fi
fi

if [[ -d "$DIR_NAME_BUILD" ]]; then
    rm -rf "$DIR_NAME_BUILD"
fi

mkdir -p "$DIR_NAME_BUILD"
cd "$DIR_NAME_BUILD"
cmake .. -G "Xcode"
