/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <iostream>
#define WAIT_TIME 10
void waiting()
{
    for (int i=0; i<WAIT_TIME; ++i)
    {
        sleep(1);
        std::cout << "waiting..." << std::endl;
    }
}

using namespace qpid::messaging;

int main(int argc, char** argv) {
    std::string broker = argc > 1 ? argv[1] : "localhost:5672";
    std::string address = argc > 2 ? argv[2] : "amq.topic";
    std::string connectionOptions = argc > 3 ? argv[3] : "";

    Connection connection(broker, connectionOptions);
    try {
        connection.open();
        Session session = connection.createSession();
        std::cout << "\nSession created and waiting..." << std::endl;
        waiting();

        Sender sender = session.createSender(address);
        std::cout << "\nSender created and waiting..." << std::endl;
        waiting();

        Receiver receiver = session.createReceiver(address);
        std::cout << "\nReceiver created and waiting..." << std::endl;
        waiting();

        std::cout << std::endl;
        sender.send(Message("Say yehhh or say nehhh!!!"));

        char ch;
        std::cin.get(ch);

        Message message = receiver.fetch(Duration::SECOND * 1);
        std::cout << message.getContent() << std::endl;
        session.acknowledge();

        connection.close();
        return 0;
    } catch(const std::exception& error) {
        std::cerr << error.what() << std::endl;
        connection.close();
        return 1;
    }
}

