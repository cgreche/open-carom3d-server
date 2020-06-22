//
// Created by CGR on 13/06/2020.
//

#include "http_server.h"
#include <core/server/messaging_protocol.h>

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

            std::string& firstLine = requestLines[0];
            std::vector<std::string> request_ = split(firstLine," ");
            std::string& methodStr = request_[0];
            if(request_[2].compare("HTTP/1.1") != 0)
                return;

            std::string& uri = request_[1];
            if(!(methodStr[0] == 'G' && methodStr[1] == 'E' && methodStr[2] == 'T')) {
                return;
            }

            std::string content;
            if(uri.find("/blog/blog.asp") != std::string::npos) {
                size_t idPos = uri.find("memberid=");
                if(idPos != std::string::npos) {
                    size_t idPosEnd = uri.find_first_of("&", idPos);
                    std::string userId = uri.substr(idPos + std::strlen("memberid="), idPosEnd - idPos - std::strlen("memberid="));
                    content = generateProfileData(userId);
                }
            }
            else {
                content = "<html><body style=\"background-color:#000000;\"></body></html>";
            }

            std::string response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: text/html\r\n";
            response += "Content-Length: " + std::to_string(content.size());
            response += "\r\n";
            response += "Connection: Close\r\n";
            response += "\r\n";
            response += content;

            user.discardReadPendingData(user.pendingReadDataSize());
            user.ntClient().send(user.sessionId(), (u8*)response.c_str(), response.size());
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

}