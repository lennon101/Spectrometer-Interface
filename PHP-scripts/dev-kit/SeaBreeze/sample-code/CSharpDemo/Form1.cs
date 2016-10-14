using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;

namespace CSharpDemo
{
    public partial class Form1 : Form
    {
        ////////////////////////////////////////////////////////////////////////
        // Spectrometer
        ////////////////////////////////////////////////////////////////////////

        Spectrometer spectrometer = null;
        int pixels = 0;
        double[] wavelengths;
        double[] rawSpectrum;
        double[] procSpectrum;
        double[] dark;
        double[] irradCal;
        int[] edcIndices;
        double[] nlcCoeffs;
        bool scanning = false;
        int integrationTimeMillisec = 100; 
        double collectionArea = 1.0;    // cm^2
        string serialNumber;
        string spectrometerType;
        uint scansToAverage = 1;

        private Logger logger = new Logger();

        bool closePending = false;

        ////////////////////////////////////////////////////////////////////////
        // Initialization
        ////////////////////////////////////////////////////////////////////////

        public Form1()
        {
            InitializeComponent();

            // initialize logger
            logger.setTextBox(textBoxEventLog);

            // assume initialization will fail
            btnScan.Enabled = false;
            checkBoxEnableIrradiance.Enabled = false;

            initializeSpectrometer();
            updateSpectrometerFromGUI();
            updateGUIFromSpectrometer();
        }

        private void initializeSpectrometer()
        {
            if (spectrometer != null && !backgroundWorkerAcquisition.IsBusy)
            {
                spectrometer.close();
                spectrometer = null;
            }

            if ((spectrometer == null || !spectrometer.isOk()) && !backgroundWorkerAcquisition.IsBusy)
            {
                logger.display("Initializing spectrometer...");
                spectrometer = new SeaBreezeSpectrometer();

                spectrometer.setLogger(logger);
                spectrometer.open();
                if (spectrometer.isOk())
                {
                    serialNumber = spectrometer.getSerialNumber();
                    spectrometerType = spectrometer.getSpectrometerType();
                    pixels = spectrometer.getPixelCount();
                    wavelengths = spectrometer.getWavelengths();
                    rawSpectrum = new double[pixels];

                    logger.display("Successfully claimed {0} with {1} pixels", spectrometerType, pixels);

                    collectionArea = spectrometer.getCollectionArea();
                    irradCal = spectrometer.getIrradianceCalibration();
                    if (collectionArea <= 0 || irradCal == null)
                    {
                        checkBoxEnableIrradiance.Checked = false;
                        checkBoxEnableIrradiance.Enabled = false;
                    }
                    else
                    {
                        checkBoxEnableIrradiance.Enabled = true;
                        logger.display("Irradiance Collection Area = {0} cm^2", collectionArea);
                    }

                    logger.display("Electrically dark pixel indices:");
                    edcIndices = spectrometer.getEDCIndices();
                    for (int i = 0; i < edcIndices.Length; i++)
                        logger.display("  [{0}] = {1}", i, edcIndices[i]);

                    logger.display("Non-Linearity Correction Coefficients:");
                    nlcCoeffs = spectrometer.getNLCCoeffs();
                    if (nlcCoeffs != null)
                    {
                        for (int i = 0; i < nlcCoeffs.Length; i++)
                            logger.display("  [{0}] = {1}", i, nlcCoeffs[i]);
                    }

                    // buttonSave.Enabled = true;
                    btnScan.Enabled = true;
                    Refresh();
                }
                else
                {
                    logger.display("Failed to initialize spectrometer");
                    MessageBox.Show(
                        "No usable spectrometer available",
                        "SeaBreeze",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error,
                        MessageBoxDefaultButton.Button1,
                        MessageBoxOptions.DefaultDesktopOnly,
                        false);
                }
            }
            else
            {
                logger.log("Spectrometer already initialized");
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // Methods
        ////////////////////////////////////////////////////////////////////////

        private void updateGUIFromSpectrometer()
        {
            labelSerialNumber.Text = serialNumber;
            labelType.Text = spectrometerType;
            labelPixels.Text = String.Format("{0}", pixels);
        }

        private void updateSpectrometerFromGUI()
        {
            // integration time
            integrationTimeMillisec = (int) numericUpDownIntegrationTimeMillisec.Value;
            spectrometer.setIntegrationTimeMilliseconds(integrationTimeMillisec);
        }

        // Perform absolute irradiance calibration for spectrometer.
        //
        // S:  Sample spectrum (counts per nanometer)
        // D:  Dark spectrum (counts per nanometer, with the same integration time, 
        //     corrections, and smoothing as sample)
        // C:  Calibration (represented in micro-Joules per count)
        // T:  Integration time (represented here in seconds)
        // A:  Collection area (represented in square centimeters) unless the light 
        //     source is entirely inside an integrating sphere
        // dL: The wavelength spread (how many nanometers a given pixel represents)
        //
        // Absolute irradiance (I) is computed as follows.  Below, the subscript P 
        // will indicate a particular pixel for I, dL, S, D, and C.  Thus, SP refers 
        // to pixel P of the sample spectrum.
        //
        //      IP = (SP - DP) * CP / (T * A * dLP)
        //
        // Note that if the lamp is entirely enclosed within an integrating sphere, 
        // then the A term is omitted.
        //
        // dL is typically computed as follows, where L(P) refers to the wavelength 
        // represented by the center of pixel index P.
        //
        //      dL = [L(P + 1) - L(P - 1)] / 2
        //
        private double[] computeAbsoluteIrradiance()
        {
            if (collectionArea <= 0 || procSpectrum == null || wavelengths == null)
                return null;

            double[] irradiance = new double[pixels];
            double[] dLp = new double[pixels];

            // Calculate dLp
            dLp[0] = wavelengths[1] - wavelengths[0];
            for (int i = 1; i < pixels - 1; i++)
                dLp[i] = (wavelengths[i + 1] - wavelengths[i - 1]) / 2.0;
            dLp[pixels - 1] = wavelengths[pixels - 1] - wavelengths[pixels - 2];

            // compute irradiance
            for (int i = 0; i < pixels; i++)
                irradiance[i] = (procSpectrum[i] * irradCal[i]) 
                              / (integrationTimeMillisec * 1000.0 * collectionArea * dLp[i]);

            return irradiance;
        }

        ////////////////////////////////////////////////////////////////////////
        // Graphing
        ////////////////////////////////////////////////////////////////////////

        private void processSpectrum()
        {
            // start with raw spectrum we just acquired
            procSpectrum = new double[pixels];
            Array.Copy(rawSpectrum, procSpectrum, pixels);

            // perform ODC if requested
            if (checkBoxSubtractDark.Checked && dark != null)
            {
                for (int i = 0; i < pixels; i++)
                    procSpectrum[i] -= dark[i];
            }

            // perform EDC if requested
            if (checkBoxEnableEDC.Checked)
            {
                double value = 0.0;
                int usableDarkCount = 0;
                for (int i = 0; i < edcIndices.Length; i++)
                {
                    int edcPixelIndex = edcIndices[i];
                    if (edcPixelIndex < pixels)
                    {
                        value += procSpectrum[edcPixelIndex];
                        usableDarkCount++;
                    }
                }
                if (usableDarkCount > 0)
                {
                    value /= usableDarkCount;
                    for (int i = 0; i < pixels; i++)
                        procSpectrum[i] -= value;
                }
            }

            // perform NLC if requested
            if (checkBoxEnableNLC.Checked && nlcCoeffs != null)
            {   
                for (int i = 0; i < pixels; i++) 
                {   
                    double pixel = procSpectrum[i];
                    double factor = nlcCoeffs[0];
                    double pixelToTheN = pixel;

                    // factor is already initialized with x^0...start at x^1
                    for (int j = 1; j < nlcCoeffs.Length; j++) 
                    {   
                        factor += pixelToTheN * nlcCoeffs[j];
                        pixelToTheN *= pixel;
                    }   

                    if (factor != 0.0) 
                        procSpectrum[i] = pixel / factor;
                }   
            }   

            // perform boxcar if requested
            if (numericUpDownBoxcarHalfWidth.Value > 0)
            {
                int boxcar = (int) numericUpDownBoxcarHalfWidth.Value;
                double[] smoothed = new double[pixels];

                int boxcarLimit = pixels - boxcar - 1;
                int boxcarRange = 2 * boxcar + 1;

                double sum;
                for (int i = boxcar; i <= boxcarLimit; i++)
                {
                    sum = procSpectrum[i];
                    for (int j = 1; j <= boxcar; j++)
                        sum += procSpectrum[i - j] + procSpectrum[i + j];
                    smoothed[i] = sum / boxcarRange;
                }

                for (int i = boxcar; i <= boxcarLimit; i++)
                    procSpectrum[i] = smoothed[i];
            }

            updateGraph();
        }

        private void updateGraph()
        {
            chartSpectrum.ChartAreas["ChartArea1"].AxisX.LabelStyle.Format = "N";

            Series seriesSpectrum = chartSpectrum.Series.FindByName("seriesSpectrum");
            Series seriesIrradiance = chartSpectrum.Series.FindByName("seriesIrradiance");

            seriesSpectrum.Points.Clear();
            seriesIrradiance.Points.Clear();

            for (int i = 0; i < pixels; i++)
                seriesSpectrum.Points.AddXY(wavelengths[i], procSpectrum[i]);

            if (checkBoxEnableIrradiance.Checked)
            {
                double[] irradiance = computeAbsoluteIrradiance();
                if (irradiance != null)
                    for (int i = 0; i < pixels; i++)
                        seriesIrradiance.Points.AddXY(wavelengths[i], irradiance[i]);
            }

            Refresh();
        }

        ////////////////////////////////////////////////////////////////////////
        // GUI Events
        ////////////////////////////////////////////////////////////////////////

        private void btnScan_Click(object sender, EventArgs e)
        {
            if (!scanning)
            {
                logger.display("Start scanning");
                btnScan.Text = "Stop";
                scanning = true;
                backgroundWorkerAcquisition.RunWorkerAsync();
            }
            else
            {
                logger.display("Stop scanning");
                backgroundWorkerAcquisition.CancelAsync();
            }
        }

        private void numericUpDownIntegrationTimeMillisec_ValueChanged(object sender, EventArgs e)
        {
            updateSpectrometerFromGUI();
        }

        ////////////////////////////////////////////////////////////////////////
        // Background Worker
        ////////////////////////////////////////////////////////////////////////

        private void backgroundWorkerAcquisition_DoWork(object sender, DoWorkEventArgs e)
        {
            logger.log("[Acquisition] starting...");
            if (spectrometer == null || !spectrometer.isOk())
            {
                logger.display("Can't start acquisition (missing spectrometer or driver)");
                return;
            }

            BackgroundWorker worker = sender as BackgroundWorker;
            int scanCount = 0;

            while (true)
            {
                double[] newRaw = spectrometer.getSpectrum();
                if (newRaw == null || newRaw.Length != pixels)
                {
                    logger.display("Error taking acquisition");
                    break;
                }

                // perform multi-scan averaging, if requested
                if (scansToAverage > 1)
                {
                    for (int i = 1; i < scansToAverage; i++)
                    {
                        double[] tmpRaw = spectrometer.getSpectrum();
                        if (tmpRaw == null || tmpRaw.Length != pixels)
                        {
                            logger.display("Error taking acquisition");
                            break;
                        }
                        for (int j = 0; j < pixels; j++)
                            newRaw[j] += tmpRaw[j];
                    }

                    for (int i = 0; i < pixels; i++)
                    {
                        newRaw[i] = newRaw[i] / ((double)scansToAverage);
                    }
                }

                // copy to graphable buffer
                Array.Copy(newRaw, rawSpectrum, pixels);

                // trigger graph update in GUI thread
                worker.ReportProgress(scanCount++);

                // necessary to ensure the GUI stays responsive
                int delay = integrationTimeMillisec;
                if (delay < 200)
                    delay = 200;
                Thread.Sleep(delay);
    
                // end thread if we've been asked to cancel
                if (worker.CancellationPending)
                {
                    logger.log("[Acquisition] closing");
                    e.Cancel = true;
                    break;
                }
            }
            logger.log("[Acquisition] done");
        }

        private void backgroundWorkerAcquisition_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            processSpectrum();
        }

        private void backgroundWorkerAcquisition_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            logger.log("[AcquistionComplete] worker cleanup");
            btnScan.Text = "Start";
            scanning = false;
            if (closePending)
                cleanShutdown();
        }

        ////////////////////////////////////////////////////////////////////////
        // Safe Shutdown (ensure "[x] Close box" releases spectrometers)
        ////////////////////////////////////////////////////////////////////////

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if(backgroundWorkerAcquisition.IsBusy || backgroundWorkerAcquisition.CancellationPending)
            {
                // if acquisition worker is running, tell it to quit, then retrigger shutdown
                e.Cancel = true;
                closePending = true;
                logger.log("[OnFormClosing] signalling acquisition worker to quit");
                backgroundWorkerAcquisition.CancelAsync();
            }
            else if (!closePending)
            {
                // otherwise, since nothing is running, shutdown now
                logger.log("[OnFormClosing] shutting down directly");
                cleanShutdown();
            }
        }

        private void cleanShutdown()
        {
            logger.log("[CleanShutdown] shutdown initiated");

            // block further user input
            this.Enabled = false;

            // shutdown spectrometer
            spectrometer.close();

            // shutdown app
            if (closePending)
                this.Close();
        }

        private void checkBoxEnableIrradiance_Click(object sender, EventArgs e)
        {
            logger.display("Irradiance {0}", checkBoxEnableIrradiance.Checked ? "enabled" : "disabled");
            processSpectrum();
        }

        private void buttonSaveDark_Click(object sender, EventArgs e)
        {
            if (procSpectrum != null && procSpectrum.Length > 0)
            {
                dark = new double[pixels];
                Array.Copy(procSpectrum, dark, pixels);
                checkBoxSubtractDark.Enabled = true;
            }
            else
            {
                dark = null;
                checkBoxSubtractDark.Checked = false;
                checkBoxSubtractDark.Enabled = false;
            }
        }

        private void numericUpDownScansToAverage_ValueChanged(object sender, EventArgs e)
        {
            scansToAverage = (uint) numericUpDownScansToAverage.Value;
        }

        private void buttonReset_Click(object sender, EventArgs e)
        {
            initializeSpectrometer();
            updateSpectrometerFromGUI();
            updateGUIFromSpectrometer();
        }
    }
}
