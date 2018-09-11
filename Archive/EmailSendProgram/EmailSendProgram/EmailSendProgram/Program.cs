/*EmailSendProgram
 * 
 * Greg McIntyrey
 * 
 * thank you to Pat Vdp.
 * 
 * this program reads from the serial port and sends and email via smtp
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Mail;
using System.IO;
using System.IO.Ports;


namespace EmailSendProgram
{
    class Program
    {
        static void Main(string[] args)
        {
            //intialise port
            SerialPort myport = new SerialPort();
            myport.BaudRate = 9600;
            myport.PortName = "COM4";
            myport.Open();

            //email
            MailMessage mail = new MailMessage("hrandemail@gmail.com", "hrandemail@gmail.com", "Alert", "Alert from HR Monitor");
            SmtpClient client = new SmtpClient("smtp.gmail.com");
            client.Port = 587;  //Gmail SMTP port (TLS)
            client.Credentials = new System.Net.NetworkCredential("hrandemail@gmail.com", "724e524b54");
            client.EnableSsl = true;

            while (true)
            {
                string data_rx = myport.ReadLine();     //read from serial port

                if (Convert.ToChar(data_rx[0]) == '1')
                {
                    client.Send(mail);
                    myport.WriteLine("1");
                }
            }

        }
    }
}
