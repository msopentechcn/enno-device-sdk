using Amqp;
using Amqp.Framing;
using Microsoft.OpenIoT.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Microsoft.OpenIoT
{

    class Gateway
    {

        AMQPSender<string> _sender;
        static string PAYLOAD_KEY = "payload";
        //AMQPReceiver<string> _receiver;

        public Gateway(string amqpsAddress, string entityPath)
        {
            _sender = new AMQPSender<string>(amqpsAddress, entityPath);
        }


        public void Register(string hardwareId, string specificationToken)
        {
            byte[] payload = Encoder.Register(hardwareId, specificationToken);
            _sender.SendSerialized(payload);
        }

        public void Measurement(string hardwareId, string name, double value)
        {
            byte[] payload = Encoder.Measurement(hardwareId, name, value);
            _sender.SendSerialized(payload);
        }

        public void Alert(string hardwareId, string alertType, string message)
        {
            byte[] payload = Encoder.Alert(hardwareId, alertType, message);
            _sender.SendSerialized(payload);

        }

        public void Location(string hardwareId, double latitude, double longtitude, double elevation)
        {
            byte[] payload = Encoder.Location(hardwareId, latitude, longtitude, elevation);
            _sender.SendSerialized(payload);

        }

        public void Close()
        {
            this._sender.Close();
        }

    }
}
