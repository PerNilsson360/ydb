#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

#include <iostream>
#include <thread>
#include <set>

#include <xercesc/dom/DOM.hpp>

#include "Ydb.hh"
#include "DomUtils.hh"
#include "Config.hh"
#include "CapabilityExchange.hh"
#include "NetconfMessage.hh"
#include "NetconfException.hh"
#include "NetconfSessions.hh"

#include "YdbApi.hh"

using namespace xercesc;
using namespace YdbApi;

namespace
{
bool _running = false;
std::set<User*> _users;

#define TRUE   1  
#define FALSE  0  
#define PORT 8888  
    
void
run()
{
    int opt = TRUE;   
    int master_socket, addrlen, activity, valread;   
    int max_sd;   
    struct sockaddr_in address;   
         
    char buffer[1025];  //data buffer of 1K  
         
    DomUtils domUtils;
    Config config(domUtils);
    Ydb ydb(config, domUtils);
    NetconfSessions sessions;
             
    //create a master socket  
    if ((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
		   sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    //accept the incoming connection  
    addrlen = sizeof(address);   
    puts("Waiting for connections ...");   

    //set of socket descriptors  
    fd_set readfds;   
    while (true) {   
        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
             
        //add child sockets to set  
        for (auto&& session : sessions.getSessions()) {   
            //socket descriptor  
            int socket = session.getSocket();   
                 
            //if valid socket descriptor then add to read list  
            if (socket > 0) {
                FD_SET(socket, &readfds);
	    }
                 
            //highest file descriptor number, need it for the select function  
            if (socket > max_sd) {   
                max_sd = socket;
	    }
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno != EINTR)) {   
            printf("select error");   
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds)) {
	    int socket;
            if ((socket = accept(master_socket,  
				 (struct sockaddr *)&address,
				 (socklen_t*)&addrlen)) < 0) {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands  
            printf("New connection, socket fd is %d , ip is : %s , port : %d  \n" ,
		   socket,
		   inet_ntoa(address.sin_addr) ,
		   ntohs 
		   (address.sin_port));   

	    try {
		sessions.newSession(socket);
		CapabilityExchange capabilityExchange;
		const char* message = capabilityExchange.getMessage();
		size_t length = strlen(message);
		if (send(socket, message, length, 0) != length) {   
		    perror("send");   
		}
		puts("Capability exchange sent");   
	    } catch (const NetconfException& e) {
		std::cerr << "Got: " << e.what() << std::endl;
	    }
	}   
             
        //else its some IO operation on some other socket
	for (auto&& session : sessions.getSessions()) {   
            int socket = session.getSocket();   
            if (FD_ISSET(socket, &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                if ((valread = read(socket, buffer, 1024)) == 0) {   
                    //Somebody disconnected , get his details and print  
                    getpeername(socket , (struct sockaddr*)&address , (socklen_t*)&addrlen);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
			   inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close(socket);   
		    session.reset();
                } else {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    buffer[valread] = '\0';
		    std::string response;
		    try {
			NetconfMessage message(domUtils, buffer, valread);
			if (message.isHello()) {
			    std::cerr << "got hello message" << std::endl;
			} else {
			    response = message.apply(ydb);
			}
		    } catch (const NetconfException& e) {
			std::cerr << "Got exception: " << e.what() << std::endl;
			response = e.what();
		    } catch (const DOMException& toCatch) {
			char* message = XMLString::transcode(toCatch.msg);
			std::cerr << "Exception: \n" << message << "\n";
			XMLString::release(&message);
		    }
		    std::cerr << "sending response: "<< response << std::endl;
                    send(socket, response.c_str(), response.size(), 0);   
                }   
            }   
        }   
    }       
}
    
}

namespace YdbApi
{
    
void
start() 
{
    if (_running) {
	return;
    }
    _running = true;
    std::thread t(run);
    t.join();
    std::cerr << "Ydb is done." << std::endl;
}

void
subscribe(User* user)
{
    _users.insert(user);
}
    
}
