//  ---------------------------------------------------------------------------------
//  Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
// 
//  The MIT License (MIT)
// 
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
// 
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//  ---------------------------------------------------------------------------------

//#define DEBUG_LOG


namespace Microsoft.OpenIoT
{
    using System;
    using System.IO;
    using System.Collections.Generic;
    using System.Text;
    using System.Threading;
    using Microsoft.OpenIoT.Common;
    using Amqp;
    using Amqp.Framing;

    //--//
    
    public class AMQPReceiver<T> : IMessageReceiver<T>
    {

        //--//
        string amqpsAddress, entityName;
        static string PAYLOAD_KEY = "payload";
        Connection amqpConnection;
        Session amqpSession;
        ReceiverLink amqpReceiver;
        public AMQPReceiver(string amqpsAddress, string entityName)
        {
            this.amqpsAddress = amqpsAddress;
            this.entityName = entityName;
            
        }

        public void Open(string clientId)
        {
            amqpConnection = new Connection(new Address(amqpsAddress));
            amqpSession = new Session(amqpConnection);

            amqpReceiver = new ReceiverLink(amqpSession, clientId, entityName);
           
        }

        public void Start()
        {
            
            while (true)
            {
                Message message = amqpReceiver.Receive();
                if (message == null)
                {
                    continue;
                }
                var value = message.ApplicationProperties[PAYLOAD_KEY];
                OnMessage(value);
                amqpReceiver.Accept(message);
            }
            
            //amqpReceiver.Start(5, OnMessageCallback);
        }

        public void OnMessage<T>(T value)
        {
            Console.WriteLine(value);
            // do your logic here

        }

        public void Close()
        {
            amqpReceiver.Close();
            amqpSession.Close();
            amqpConnection.Close();

        }

        /**
        void OnMessageCallback(ReceiverLink receiver, Message message)
        {
            var value = message.ApplicationProperties[PAYLOAD_KEY];
            Console.WriteLine(value);

            // Do something with the value
            
            receiver.Accept(message);
            receiver.SetCredit(5);
        }
        **/
        
    }
}
    