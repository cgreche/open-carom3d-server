//
// Created by CGR on 13/06/2020.
//

#include "http_server.h"
#include <core/server/messaging_protocol.h>
#include <thread>
#include <fstream>

namespace core {

    class HTTPProtocol : public MessagingProtocol {
    public:
        HTTPProtocol() {
        }

        ClientSession* createSession(nettools::ntConnection& ntClient, Server& server) override {
            return new ClientSession(ntClient, server);
        }
        
        std::vector<std::string> split(std::string source, std::string delimiter) {
            std::vector<std::string> ret;

            size_t startPos = 0, endPos;
            while((endPos = source.find(delimiter, startPos)) != std::string::npos) {
                ret.push_back(source.substr(startPos, endPos-startPos));
                startPos = endPos + delimiter.length();
            }
            ret.push_back(source.substr(startPos));
            return ret;
        }

        std::vector<uint8_t> readFile(const char* path) {
            std::vector<uint8_t> ret;
            std::ifstream file;
            file.open(path, std::ios::binary | std::ios::ate);
            if(file.is_open()) {
                unsigned int size = file.tellg();
                uint8_t* memblock = new uint8_t[size];
                file.seekg(0, std::ios::beg);
                file.read((char*)memblock, size);
                file.close();
                ret.insert(ret.begin(), memblock, memblock + size);
                delete[] memblock;
                file.close();
            }

            return ret;
        }

        std::string generateProfileData(std::string userId) {
            std::string content =
                "<html> \
                <style> \
                    body { \
                        background-image: url('https://cdn.pixabay.com/photo/2016/09/01/08/24/smiley-1635449_960_720.png'); \
                        background-position: center center; \
                        background-repeat: no-repeat; \
                        background-size: auto 100%; \
                        height: 100%; \
                    } \
                    .user-id { display: table; margin: auto; position: relative; top: 10%; font-size: 2em; font-weight: bold; text-align: center; color:#0080FF; } \
                </style> \
                <body> \
                <span class=\"user-id\">" + userId + "</span> \
                </body</html>";

            return content;
        }

        void onMessageReceived(ClientSession & user) override {
            u8* data = user.pendingReadData();
            int size = user.pendingReadDataSize();
            std::string requestData((const char*)data, size);
            printf(requestData.c_str());
            printf("\n");
            
            std::vector<std::string> requestLines = split(requestData, "\r\n");
            if(requestLines.size() < 2)
                return;

            std::string& firstLine = requestLines[0];
            std::vector<std::string> request_ = split(firstLine," ");
            if(request_.size() < 3)
                return;

            std::string& methodStr = request_[0];
            std::string& uri = request_[1];
            std::string& httpVersion = request_[2];

            if(requestLines[requestLines.size() - 1].compare("") != 0)
                return;

            std::string header;
            std::vector<unsigned char> content;
            if(uri.find("/blog/blog.asp") != std::string::npos) {
                size_t idPos = uri.find("memberid=");
                if(idPos != std::string::npos) {
                    size_t idPosEnd = uri.find_first_of("&", idPos);
                    std::string userId = uri.substr(idPos + std::strlen("memberid="), idPosEnd - idPos - std::strlen("memberid="));
                    std::string profileData = generateProfileData(userId);
                    content.insert(content.begin(), profileData.begin(), profileData.end());
                }
                header += "Content-Type: text/html\r\n";
            }
            else if(uri.find("/blog/getpp.asp") != std::string::npos) {
                content = readFile("resources/open.png");
                header += "Content-Type: image/png\r\n";
            }
            else if(uri.find("/_banner/banner.asp") != std::string::npos) {
                header += "Content-Type: text/html\r\n";
                content = readFile("resources/banner.html");
            }
            else if(uri.find("/spotlight/slide.asp") != std::string::npos) {
                header += "Content-Type: text/html\r\n";
                content = readFile("resources/spotlight.html");
            }
            else {
                header += "Content-Type: text/html\r\n";
                std::string html = "<html><body style=\"background-color:#000000;\"></body></html>";
                content.insert(content.begin(), html.begin(), html.end());
            }

            std::string responseS = "HTTP/1.1 200 OK\r\n";
            responseS += header;
            responseS += "Cache-Control: no-cache\r\n";
            responseS += "Content-Length: " + std::to_string(content.size()) + "\r\n";
            responseS += "Connection: Close\r\n";
            responseS += "\r\n";

            std::vector<unsigned char> response(responseS.c_str(), responseS.c_str()+responseS.size());
            if(content.size() > 0)
                response.insert(response.end(), content.begin(), content.end());

            user.discardReadPendingData(user.pendingReadDataSize());
            user.ntClient().send(user.sessionId(), response.data(), response.size());
        }

        void onMessageSent(ClientSession& user) override {
        }

        void closeSession(ClientSession& user) override {
            delete &user;
        }

    };

    static HTTPProtocol g_httpProtocol;

    HTTPServer::HTTPServer(const ServerConfig& config)
        : Server(config) {
    }

    MessagingProtocol* HTTPServer::messagingProtocol() {
        return &g_httpProtocol;
    }

    void HTTPServer::run() {
        for(;;) {
            m_ntServer.poll();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

}