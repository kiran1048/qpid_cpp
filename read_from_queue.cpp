#include <cstdio>
#include <string>
#include <fstream>
#include <qpid/messaging/Address.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>

using namespace qpid::messaging;

using std::stringstream;
using std::string;

int main(int argc, char** argv) {
    //const char* url = "loco1:10000";
    std::string connectionOptions = "";
    const char* url =  argc>1 ? argv[1] : "loco1:10000";
    const char* queue =  argc>2 ? argv[2] : "MR.Retry";
    const int COUNT =  argc>3 ? std::stoi(argv[3]) : 1;
    std::string queue_str(queue);
    //queue_str = queue_str + ";{mode:browse}";

    Connection connection(url, connectionOptions);
    connection.open();
    Session session = connection.createSession();
    Receiver receiver = session.createReceiver(queue_str);

    std::ofstream fout;
    //fout.open("queue_message.txt");
    fout.open("fragmented_messages.txt");

    int count = 0 ;
    while (count < COUNT) {
       Message request = receiver.fetch();
       /*
       std::cout << "Processed request: " << std::endl
                         << "getSubject         = " << request.getSubject() << std::endl
                         << "getUserId          = " << request.getUserId() << std::endl
                         << "getReplyTo         = " << request.getReplyTo() << std::endl
                         << "getCorrelationId   = " << request.getCorrelationId() << std::endl
                         << "getPriority        = " << request.getPriority() << std::endl
                         << "getTtlmillSec      = " << request.getTtl().getMilliseconds() << std::endl
                         << "getProperties      = " << request.getProperties() << std::endl
                         << "get=getContentType = " << request.getContentType() << std::endl
                         << "getContent         = " << request.getContent() << std::endl
                         << "getContentPtr      = " << conPtr << std::endl
                         << "getContentSize     = " << request.getContentSize() << std::endl
                         << std::endl; 
       std::cout << "\n =============== \n " << std::endl;
       std::cout << "\n\n ";
       const char* conPtr = request.getContentPtr();
       int contentSize = request.getContentSize();
       for (int i=0; i<contentSize; ++i)
           std::cout << conPtr[i] << std::endl; */

       std::string subject = request.getSubject();
       std::string userId = request.getUserId();
       const Address& address = request.getReplyTo();
       std::string corrId = request.getCorrelationId();
       int priority = request.getPriority();
       int ttlMilli = request.getTtl().getMilliseconds();
       qpid::types::Variant::Map props = request.getProperties();
       std::string contentType = request.getContentType();
       std::string content = request.getContent();

       std::cout << "\n count = " << count;
       std::cout << "\n ===== Writing to file ========= \n " << std::endl;
       stringstream ss;
       ss.str(std::string());

       ss << subject  << std::endl
          << userId << std::endl
          << address.str() << std::endl
          << corrId << std::endl
          << priority << std::endl
          << ttlMilli << std::endl
          << props << std::endl
          << contentType << std::endl
          << content << std::endl;

       fout << ss.rdbuf();
       count++;
    }

        /*
        std::stringstream newSs;
        newSs.str(std::string());

        std::ifstream fin;
        fin.open("mr_retry_msgs.txt");

        std::string newSubject;
        std::getline(fin, newSubject);

        std::string newUserId;
        std::getline(fin, newUserId);

        std::string newReplyTo;
        std::getline(fin, newReplyTo);
        const Address& newAddress(newReplyTo);

        std::string newCorrId;
        std::getline(fin, newCorrId);

        std::string newPriorityStr;
        std::getline(fin, newPriorityStr);
        newSs << newPriorityStr;
        int newPriority; // = std::stoi(newPriorityStr, &sz);
        newSs >> newPriority;

        string newTtlMilliStr;
        std::getline(fin, newTtlMilliStr);
        newSs << newTtlMilliStr;
        int newTtlMilli; // = std::stoi(newTtlMilliStr, &sz);
        newSs >> newTtlMilli;
        Duration newTtl(newTtlMilli);

        std::string newPropsStr;
        std::getline(fin, newPropsStr);
        const char* newPropsChar = newPropsStr.c_str();
        //qpid::types::Variant::Map newProps = qpid::types::Variant(newPropsChar).asMap();

        std::string newContentType;
        std::getline(fin, newContentType);

        std::string newContent;
        std::getline(fin, newContent);

        std::cout << std::endl <<" Contents read from file ----- " << std::endl;
        std::cout << "newSubject = "  << newSubject << std::endl
                  << "newUserId = "   << newUserId << std::endl
                  << "newReplyTo = "  << newReplyTo << std::endl
                  << "newCorrId = "   << newCorrId << std::endl
                  << "newPriority = " <<newPriority << std::endl
                  << "newTtl = "      << newTtl.getMilliseconds() << std::endl
                  //<< newProps << std::endl
                  << "newContentType = " << newContentType << std::endl
                  << "newContent =  " << newContent << std::endl; */

        fout.close();
        connection.close(); 
        return 0;

}

