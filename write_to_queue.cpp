#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <algorithm>
#include <vector>
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
    //const char* url = "bosystem1:12090";

    const char* url = argc>1 ? argv[1] : "loco1:10000";
    const char* exch = argc>2 ? argv[2] : "RBOutbound.Ex";
    const int COUNT = argc>3 ? std::stoi(argv[3]) : 1;
    std::string connectionOptions = "";
    //std::cout << std::endl << url << std::endl << exch << std::endl << COUNT << std::endl;

    Connection connection(url, connectionOptions);
    try {
        connection.open();
        Session session = connection.createSession();
        std::string exch_str(exch);
        Sender sender = session.createSender(exch_str);

        std::stringstream newSs;
        std::ifstream fin;
        //fin.open("queue_message.txt");
        fin.open("fragmented_messages.txt");

        int count = 0;
        while (count < COUNT) {
            std::cout << "\n =============== \n";
            count++;
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
            newSs.str(std::string());
            newSs.clear();
            newSs << newPriorityStr;
            int newPriority; // std::stoi(newPriorityStr, &sz);
            newSs >> newPriority;
            //std::cout << "prio str = " << newPriorityStr << std::endl;
            //std::cout << "prio int = " << newPriority << std::endl;

            std::string newTtlMilliStr;
            std::getline(fin, newTtlMilliStr);
            newSs.str(std::string());
            newSs.clear();
            int newTtlMilli; // std::stoi(newTtlMilliStr.c_str());
            newSs << newTtlMilliStr;
            newSs >> newTtlMilli;
            //std::cout << "ttl str = " << newTtlMilliStr << std::endl;
            //std::cout << "ttl int = " << newTtlMilli << std::endl;
            Duration newTtl(newTtlMilli);

            std::string newPropsStr;
            std::getline(fin, newPropsStr);
            newPropsStr = newPropsStr.substr(1, newPropsStr.length()-2);
            std::vector<std::string> propsList;
            boost::split(propsList, newPropsStr, boost::is_any_of(", "), boost::token_compress_on);

            std::string newContentType;
            std::getline(fin, newContentType);

            std::string newContent;
            std::getline(fin, newContent);
/*
            std::cout << std::endl           << "Contents read from file ----- " << std::endl;
            std::cout << "newSubject = "     << newSubject << std::endl
                      << "newUserId = "      << newUserId << std::endl
                      << "newReplyTo = "     << newReplyTo << std::endl
                      << "newCorrId = "      << newCorrId << std::endl
                      << "newPriority = "    << newPriority << std::endl
                      << "newTtl = "         << newTtl.getMilliseconds() << std::endl
            //          << "newProps = "       << propCount << std::endl
                      << "newContentType = " << newContentType << std::endl
                      << "newContent = "     << newContent << std::endl;

            std::ofstream fout;
            fout.open("new_mr_retry_msgs.txt");
            stringstream ss;
            ss << newSubject  << std::endl
               << newUserId << std::endl
               << newAddress.str() << std::endl
               << newCorrId << std::endl
               << newPriority << std::endl
               << newTtl.getMilliseconds() << std::endl
               << newPropsChar << std::endl
               << newContentType << std::endl
               << newContent << std::endl;
            fout << ss.rdbuf();
            fout.close(); */

            Message newMsg;
            newMsg.setSubject(newSubject);
            newMsg.setUserId(newUserId);
            newMsg.setReplyTo(newAddress);
            newMsg.setCorrelationId(newCorrId);
            newMsg.setPriority(newPriority);
            newMsg.setTtl(newTtl);
            newMsg.setContentType(newContentType);
            newMsg.setContent(newContent);
            //newMsg.setProperty(newProps);
            std::string name, value;
            int index, size;
            for(std::vector<std::string>::iterator it = propsList.begin(); it != propsList.end(); ++it) {
                index = it->find_first_of(":");
                size = it->size();
                name = it->substr(0, index);
                value = it->substr(index+1, size);
                if (name == "extHeaders"){
                    int exIndex = value.find_first_of(":");
                    int exSize = value.size();
                    std::string exName = value.substr(0, exIndex);
                    std::string exValue = value.substr(exIndex+1, exSize);
                    std::map<std::string, qpid::types::Variant> mapVar;
                    mapVar[exName] = qpid::types::Variant(exValue);
                    newMsg.getProperties()[name] = qpid::types::Variant(mapVar);
                }
                else{
                    newMsg.getProperties()[name] = qpid::types::Variant(value);
                }
                //std::cout << std::endl << "name = " << it->substr(0,index) << " value = " << it->substr(index+1, size);
            }
            std::cout << std::endl;
            sender.send(newMsg);
        }

        fin.close();
        connection.close();
        return 0;
    }
        catch(const std::exception& error) {
        std::cout << error.what() << std::endl;
        connection.close();
    }

    return 1;
}

