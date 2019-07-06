#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Message.h>
#include <iostream>
using namespace qpid::messaging;

int main(int argc, char** argv)
{
std::string broker = argc>1 ? argv[1] : "amqp:tcp:127.0.0.1:5672";
//std::string address = "random.topic; {create:always}";
std::string address = argc>2 ? argv[2] : "service_queue; {create:always}";
Connection conn(broker);

try {
    conn.open();
    Session sess = conn.createSession();
//    Sender sender = sess.createSender(address);
    Receiver receiver = sess.createReceiver(address);
//    sender.send(Message("Boooooooo....."));
//    std::cout << "\nSent Boooo...." << std::endl;
    std::cout << "\nReading from queue..." << std::endl;

    Message message = receiver.fetch();
    std::cout << "Received = " << message.getContent() << std::endl;

    conn.close();
    return 0;
}
catch (const std::exception& e)
{
    std::cerr << "Caught exception = " << e.what() << std::endl;
    conn.close();
    return 1;
}

return 0;
}


