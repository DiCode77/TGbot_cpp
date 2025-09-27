cd TGbot_cpp;

cd Resources;
curl -L https://github.com/nlohmann/json/releases/latest/download/json.hpp -o json.hpp;
cd ..;

mkdir build;
cd build;
cmake .. -G "Xcode";
