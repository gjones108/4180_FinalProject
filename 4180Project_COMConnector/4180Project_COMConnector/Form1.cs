using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace _4180Project_COMConnector
{

    public partial class Form1 : Form
    {
        private static Mutex mut = new Mutex();
        private double lightData = -1;
        private double moistureData = -1;
        private double tempData = -1;

        public Form1()
        {
            InitializeComponent();
            //mbedCOM.ReadTimeout = 500;
            //mbedCOM.WriteTimeout = 500;
            mbedCOM.Open();
        }

        private void mbedCOM_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            //Console.WriteLine("trying");
            String inData;

            try
            {
                inData = mbedCOM.ReadLine();
                Console.WriteLine(inData);

                // successful data, try to display
                if (label1.InvokeRequired)
                {
                    Invoke(new EventHandler(DisplayData), inData, EventArgs.Empty);
                }
                else
                {
                    DisplayData(inData, EventArgs.Empty);
                }
            }
            catch (System.IO.IOException error)
            {
                //return;
            }
            catch (System.InvalidOperationException error)
            {
                //return;
            }
        }

        private void DisplayData(object sender, EventArgs e)
        {
            string data = (string)sender;
            //label1.Text = data;
            string first = "empty";
            try
            {
                first = data.Substring(0, 1);
            }
            catch { }
            string post_equals = "";

            int indexOfEquals = data.IndexOf("=");
            if (indexOfEquals != -1)
            {
                post_equals = data.Substring(indexOfEquals + 1);
            }

            if (first == "L")
            {
                // light
                lightlabel.Text = post_equals;


            }
            else if (first == "R")
            {
                // humidity
                humidlabel.Text = post_equals;
            }
            else if (first == "T")
            {
                // temperature
                templabel.Text = post_equals;
            }
            else if (first == "M")
            {
                // moisture
                moistlabel.Text = post_equals;
            }
            else { 
                // error

            }

            //mut.WaitOne();
            //String newData = inData;
            //mut.ReleaseMutex();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click_1(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (mbedCOM.IsOpen) {
                mbedCOM.Write(thresholdtxt.Text);
            }
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {

        }
    }
}
