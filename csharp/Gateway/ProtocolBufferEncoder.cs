using Google.ProtocolBuffers;
using Microsoft.OpenIoT;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Microsoft.OpenIoT
{
 
    class Encoder
    {
        public static byte[] Register(string hardwareId, string specificationToken)
        {

            MemoryStream stream = new MemoryStream();

            new OpenIoT.Types.Header.Builder().SetCommand(OpenIoT.Types.Command.REGISTER).Build().WriteDelimitedTo(stream);


            new OpenIoT.Types.RegisterDevice.Builder
            {
                HardwareId = hardwareId,
                SpecificationToken = specificationToken
                //SiteToken = "46b6dd80-9cec-484e-9d6a-f6cb9f0a9801"
            }.Build().WriteDelimitedTo(stream);

            return stream.ToArray();

        }

        public static byte[] Measurement(string hardwareId, string name, double value)
        {
            MemoryStream stream = new MemoryStream();

            new OpenIoT.Types.Header.Builder
            {
                Command = OpenIoT.Types.Command.DEVICEMEASUREMENT
            }.Build().WriteDelimitedTo(stream);

            OpenIoT.Types.Measurement.Builder m = new OpenIoT.Types.Measurement.Builder
            {
                MeasurementId = name,
                MeasurementValue = value
            
            };
            new OpenIoT.Types.DeviceMeasurements.Builder().SetHardwareId(hardwareId).AddMeasurement(m).Build().WriteDelimitedTo(stream);
            
            return stream.ToArray();
        }

        public static byte[] Alert(string hardwareId, string alertType, string message)
        {
            MemoryStream stream = new MemoryStream();

            new OpenIoT.Types.Header.Builder
            {
                Command = OpenIoT.Types.Command.DEVICEALERT
            }.Build().WriteDelimitedTo(stream);

            new OpenIoT.Types.DeviceAlert.Builder
            {
                HardwareId = hardwareId,
                AlertMessage = message,
                AlertType = alertType
            }.Build().WriteDelimitedTo(stream);

            return stream.ToArray();

        }

        public static byte[] Location(string hardwareId, double latitude, double longtitude, double elevation)
        {
            MemoryStream stream = new MemoryStream();

            new OpenIoT.Types.Header.Builder
            {
                Command = OpenIoT.Types.Command.DEVICELOCATION
            }.Build().WriteDelimitedTo(stream);

            new OpenIoT.Types.DeviceLocation.Builder
            {
                HardwareId = hardwareId,
                Latitude = latitude,
                Longitude = longtitude,
                Elevation = elevation
            }.Build().WriteDelimitedTo(stream);

            return stream.ToArray();

        }

    }
}
