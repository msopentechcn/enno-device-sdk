using Amqp;
using Amqp.Framing;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Microsoft.OpenIoT
{
    class Test
    {

        /////////////////  Below information should be customized  /////////////////

        /**
        
        private static string uniqueClientId = "[YourUniqueClientId]";

        private static string receiveAmqpsAddress = "amqps://[sasName]:[sasKey]@[namespace].servicebus.chinacloudapi.cn";
        private static string receiveEntityPath = "/[topicName]/Subscriptions/[subscriptionName]";

        private static string sendAmqpsAddress = "amqps://[sasName]:[sasKey]@[namespace].servicebus.chinacloudapi.cn";
        private static string sendEntityPath = "/[entityPath]";

        // Unique device id, eg. Mac address or UUID
        private static string deviceId = "[YourDeviceId]]";
        // Unique device specification ID, assigned by server admin
        private static string specificationId = "[YourSpecificationId]]";

        **/

        private static string uniqueClientId = "/system/Subscriptions/s000001";

        private static string receiveAmqpsAddress = "amqps://iot:vUG53u3%2BDamdR0YLHqU5kb0h%2BpFZa4NFBqusqpf%2F4vc%3D@iot-demo-ns.servicebus.chinacloudapi.cn";
        private static string receiveEntityPath = "/system/Subscriptions/test-2";

        private static string sendAmqpsAddress = "amqps://iot:ov3Sycoo%2F2ZfU3mNL3gZVPBpishymqGb01SK5hD%2BpWI%3D@iot-ns.servicebus.chinacloudapi.cn";
        private static string sendEntityPath = "/dev";

        // Unique device id, eg. Mac address or UUID
        private static string deviceId = "demo-healthcare-id-1";
        // Unique device specification ID, assigned by server admin
        private static string specificationId = "eca02bb6-a794-4d9a-a669-5a02c43ff21d";


        ////////////////////// End of customized information //////////////////////

        public static void testReceiver()
        {
            string amqpsAddress = receiveAmqpsAddress;
            string entityName = receiveEntityPath;
            AMQPReceiver<string> r = new AMQPReceiver<string>(amqpsAddress, entityName);
            r.Open(uniqueClientId);
            r.Start();
        }

        public static void testRegisterAPI(string deviceId, string specificationId)
        {
            string amqpsAddress = sendAmqpsAddress;
            string entityPath = sendEntityPath;

            Gateway gateway = new Gateway(amqpsAddress, entityPath);

            gateway.Register(deviceId, specificationId);
            //gateway.Alert(deviceId, alertLevel, alertMessage);
            //gateway.Measurement(deviceId, "cpu.utils", 23.5);
            //gateway.Location(deviceId, 176.223223f, 54.234323f, 1.0012323f);

            gateway.Close();

        }

        public static void testAlertAPI(string deviceId, string alertLevel, string alertMessage)
        {
            string amqpsAddress = sendAmqpsAddress;
            string entityPath = sendEntityPath;

            Gateway gateway = new Gateway(amqpsAddress, entityPath);

            gateway.Alert(deviceId, alertLevel, alertMessage);
            //gateway.Measurement(deviceId, "cpu.utils", 23.5);
            //gateway.Location(deviceId, 176.223223f, 54.234323f, 1.0012323f);

            gateway.Close();

        }

        public static void testMeasurementAPI(string deviceId, string measurementId, double value)
        {
            string amqpsAddress = sendAmqpsAddress;
            string entityPath = sendEntityPath;

            Gateway gateway = new Gateway(amqpsAddress, entityPath);

            gateway.Measurement(deviceId, measurementId, value);

            gateway.Close();

        }

        public static void testLocationAPI(string deviceId, double longtitude, double latitude, double elevation)
        {
            string amqpsAddress = sendAmqpsAddress;
            string entityPath = sendEntityPath;

            Gateway gateway = new Gateway(amqpsAddress, entityPath);

            gateway.Location(deviceId, longtitude, latitude, elevation);

            gateway.Close();

        }

        public static void testMessageSend(string textMessage)
        {
            string amqpsAddress = sendAmqpsAddress;
            string entityPath = sendEntityPath;

            var amqpConnection = new Connection(new Address(amqpsAddress));

            var amqpSession = new Session(amqpConnection);

            SenderLink amqpSender = new SenderLink(amqpSession,
                   uniqueClientId,       // unique name for all links from this client 
                   entityPath);          // Service Bus entity path 

            var message = new Message();
            message.ApplicationProperties = new ApplicationProperties();
            message.ApplicationProperties[Constants.PayloadKey] = textMessage;

            amqpSender.Send(message);

        }

        public static void Main(String[] args)
        {
            //testSenderAPI("info","hello");
            testReceiver();   
        }
    }
}
