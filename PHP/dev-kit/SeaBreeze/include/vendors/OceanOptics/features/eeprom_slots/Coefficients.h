/***************************************************//**
 * @file    Coefficients.h
 * @date    April 2014
 * @author  Ocean Optics, Inc.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/
#include <vector>

namespace seabreeze {
    /**
     * The <code>Coefficients</code> class is the primary datatype for coefficients
     * that are stored in each spectrometer. This class determines
     * which coefficients the spectrometer has stored and saves them
     * as attributes within the <code>Coefficients</code> class.
     *
     */
    class Coefficients {
    public: 
        /* Wavelength calibration coefficients */
    
        /**
         * The EEPROM slot holding the 0th order wavelength calibration coefficient.
         */
        static const int WL_INTERCEPT     = 1;
    
        /**
         * The EEPROM slot holding the 1st order wavelength calibration coefficient.
         */
        static const int WL_FIRST         = 2;
    
        /**
         * The EEPROM slot holding the 2nd order wavelength calibration coefficient.
         */
        static const int WL_SECOND        = 3;
    
        /**
         * The EEPROM slot holding the 3rd order wavelength calibration coefficient.
         */
        static const int WL_THIRD         = 4;
    
        /**
         * The EEPROM slot holding the Stray light constant.
         */
        static const int STRAY_LIGHT      = 5;
    
        /**
         * The EEPROM slot holding the 0th order non-linearity correction coefficient.
         */
        static const int NL_0             = 6;
    
        /**
         * The EEPROM slot holding the 1st order non-linearity correction coefficient.
         */
        static const int NL_1             = 7;
    
        /**
         * The EEPROM slot holding the 2nd order non-linearity correction coefficient.
         */
        static const int NL_2             = 8;
    
        /**
         * The EEPROM slot holding the 3rd order non-linearity correction coefficient.
         */
        static const int NL_3             = 9;
    
        /**
         * The EEPROM slot holding the 4th order non-linearity correction coefficient.
         */
        static const int NL_4             = 10;
    
        /**
         * The EEPROM slot holding the 5th order non-linearity correction coefficient.
         */
        static const int NL_5             = 11;
    
        /**
         * The EEPROM slot holding the 6th order non-linearity correction coefficient.
         */
        static const int NL_6             = 12;
    
        /**
         * The EEPROM slot holding the 7th order non-linearity correction coefficient.
         */
        static const int NL_7             = 13;
    
        /**
         * The EEPROM slot holding the Polynomial order of non-linearity calibration.
         */
        static const int NL_ORDER         = 14;

    protected:  
        /** The value for the Wavelength Calibration Intercept. */
        double _WlIntercept;
    
        /** The value for the Wavelength Calibration First Coefficient. */
        double _WlFirst;
    
        /** The value for the Wavelength Calibration Second Coefficient. */
        double _WlSecond;
    
        /** The value for the Wavelength Calibration Third Coefficient. */
        double _WlThird;
    
        /** The value for the Stray Light Correction Coefficient. */
        double _StrayLight;
    
        /** Stray light first-order term */
        double strayLightSlope;
    
        /** Non-linearity Correction 0th-order Coefficient. */
        double _NlCoef0;
    
        /** Non-linearity Correction 1st-order Coefficient. */
        double _NlCoef1;
    
        /** Non-linearity Correction 2nd-order Coefficient. */
        double _NlCoef2;
    
        /** Non-linearity Correction 3rd-order Coefficient. */
        double _NlCoef3;
    
        /** Non-linearity Correction 4th-order Coefficient. */
        double _NlCoef4;
    
        /** Non-linearity Correction 5th-order Coefficient. */
        double _NlCoef5;
    
        /** Non-linearity Correction 6th-order Coefficient. */
        double _NlCoef6;
    
        /** Non-linearity Correction 7th-order Coefficient. */
        double _NlCoef7;
    
        /** Non-linearity Correction Order. */
        int    _NlOrder;
    
        // Spectrometer's such as the ADC1000USB1000 and the SAS do not have all
        // coefficients. They are set to false until they can be retrieved from
        // the spectrometer, or a file.
    
        /** Whether or not the spectrometer has a Wavelength Calibration
         * Intercept stored in EEPROM. */
        bool has_WlIntercept;
    
        /** Whether or not the spectrometer has a  Wavelength Calibration First
         * Coefficient stored in EEPROM. */
        bool has_WlFirst;
    
        /** Whether or not the spectrometer has a Wavelength Calibration
         * Second Coefficient stored in EEPROM. */
        bool has_WlSecond;
    
        /** Whether or not the spectrometer has a Wavelength Calibration
         * Third Coefficient stored in EEPROM. */
        bool has_WlThird;
    
        /** Whether or not the spectrometer has a Stray Light Correction
         * Coefficient stored in EEPROM. */
        bool has_StrayLight;

        bool has_StrayLightSlope;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 0th-order Coefficient stored in EEPROM. */
        bool has_NlCoef0;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 1st-order Coefficient stored in EEPROM. */
        bool has_NlCoef1;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 2nd-order Coefficient stored in EEPROM. */
        bool has_NlCoef2;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 3rd-order Coefficient stored in EEPROM. */
        bool has_NlCoef3;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 4th-order Coefficient stored in EEPROM. */
        bool has_NlCoef4;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 5th-order Coefficient stored in EEPROM. */
        bool has_NlCoef5;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 6th-order Coefficient stored in EEPROM. */
        bool has_NlCoef6;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * 7th-order Coefficient stored in EEPROM. */
        bool has_NlCoef7;
    
        /** Whether or not the spectrometer has a Non-linearity Correction
         * Order stored in EEPROM. */
        bool has_NlOrder;
    
    public:
        /**
         * Creates a new instance of Coefficients.
         */
        Coefficients() :
            has_WlIntercept(false),
            has_WlFirst(false),
            has_WlSecond(false),
            has_WlThird(false),
            has_StrayLight(false),
            has_StrayLightSlope(false),
            has_NlCoef0(false),
            has_NlCoef1(false),
            has_NlCoef2(false),
            has_NlCoef3(false),
            has_NlCoef4(false),
            has_NlCoef5(false),
            has_NlCoef6(false),
            has_NlCoef7(false),
            has_NlOrder(false) {}
    
        /**
         * Creates a new instance of Coefficient.
         * @param c a Coefficient array to set this <CODE>Coefficient</CODE> to.
         */
        Coefficients(Coefficients &c) {
            setWlIntercept(c.getWlIntercept());
            setWlFirst(c.getWlFirst());
            setWlSecond(c.getWlSecond());
            setWlThird(c.getWlThird());
            //setStrayLight(c.getStrayLight(), c.getStrayLightSlope());
            setStrayLight(c.getStrayLight());
            setNlCoef0(c.getNlCoef0());
            setNlCoef1(c.getNlCoef1());
            setNlCoef2(c.getNlCoef2());
            setNlCoef3(c.getNlCoef3());
            setNlCoef4(c.getNlCoef4());
            setNlCoef5(c.getNlCoef5());
            setNlCoef6(c.getNlCoef6());
            setNlCoef7(c.getNlCoef7());
            setNlOrder(c.getNlOrder());
        }
    
        /**
         * Returns the 0th order wavelength calibration coefficient.
         * @return the 0th order wavelength calibration coefficient.
         */
        double getWlIntercept() {
            return _WlIntercept;
        }
    
        /**
         * Sets the 0th order wavelength calibration coefficient.
         * @param value the 0th order wavelength calibration coefficient.
         */
        void setWlIntercept(double value) {
            _WlIntercept = value;
            has_WlIntercept = true;
        }
    
        /**
         * Returns the 1st order wavelength calibration coefficient.
         * @return the 1st order wavelength calibration coefficient.
         */
        double getWlFirst() {
            return _WlFirst;
        }
    
        /**
         * Sets the 1st order wavelength calibration coefficient.
         * @param value the 1st order wavelength calibration coefficient.
         */
        void setWlFirst(double value) {
            _WlFirst = value;
            has_WlFirst = true;
        }
    
        /**
         * Returns the 2nd order wavelength calibration coefficient.
         * @return the 2nd order wavelength calibration coefficient.
         */
        double getWlSecond() {
            return _WlSecond;
        }
    
        /**
         * Sets the 2nd order wavelength calibration coefficient.
         * @param value the 2nd order wavelength calibration coefficient.
         */
        void setWlSecond(double value) {
            _WlSecond = value;
            has_WlSecond = true;
        }
    
        /**
         * Returns the 3rd order wavelength calibration coefficient.
         * @return the 3rd order wavelength calibration coefficient.
         */
        double getWlThird() {
            return _WlThird;
        }
    
        /**
         * Sets the 3rd order wavelength calibration coefficient.
         * @param value the 3rd order wavelength calibration coefficient.
         */
        void setWlThird(double value) {
            _WlThird = value;
            has_WlThird = true;
        }
    
        /**
         * Gets all of the wavelength calibration coefficients.
         * @return an array of the wavelength calibration coefficients.
         */
        std::vector<double> getWlCoefficients() {
            std::vector<double> wl(4);
            wl[0] = _WlIntercept;
            wl[1] = _WlFirst;
            wl[2] = _WlSecond;
            wl[3] = _WlThird;
            return wl;
        }
    
        /**
         * Sets all of the wavelength calibration coefficients.
         * @param wl an array of the wavelength calibration coefficients.
         */
        void setWlCoefficients(const std::vector<double> &wl) {
            _WlIntercept = wl[0];
            has_WlIntercept = true;
            _WlFirst = wl[1];
            has_WlFirst = true;
            _WlSecond = wl[2];
            has_WlSecond = true;
            _WlThird = wl[3];
            has_WlThird = true;
        }
    
        /**
         * Returns the 0th order non-linearity correction coefficient.
         * @return 0th order non-linearity correction coefficient
         */
        double getNlCoef0() {
            return _NlCoef0;
        }
    
        /**
         * Sets the 0th order non-linearity correction coefficient
         * @param value 0th order non-linearity correction coefficient
         */
        void setNlCoef0(double value) {
            _NlCoef0 = value;
            has_NlCoef0 = true;
        }
    
        /**
         * Returns the 1st order non-linearity correction coefficient.
         * @return 1st order non-linearity correction coefficient
         */
        double getNlCoef1() {
            return _NlCoef1;
        }
    
        /**
         * Sets the 1st order non-linearity correction coefficient
         * @param value 1st order non-linearity correction coefficient
         */
        void setNlCoef1(double value) {
            _NlCoef1 = value;
            has_NlCoef1 = true;
        }
    
        /**
         * Returns the 2nd order non-linearity correction coefficient.
         * @return 2nd order non-linearity correction coefficient
         */
        double getNlCoef2() {
            return _NlCoef2;
        }
    
        /**
         * Sets the 2nd order non-linearity correction coefficient
         * @param value 2nd order non-linearity correction coefficient
         */
        void setNlCoef2(double value) {
            _NlCoef2 = value;
            has_NlCoef2 = true;
        }
    
        /**
         * Returns the 3rd order non-linearity correction coefficient.
         * @return 3rd order non-linearity correction coefficient
         */
        double getNlCoef3() {
            return _NlCoef3;
        }
    
        /**
         * Sets the 3rd order non-linearity correction coefficient
         * @param value 3rd order non-linearity correction coefficient
         */
        void setNlCoef3(double value) {
            _NlCoef3 = value;
            has_NlCoef3 = true;
        }
    
        /**
         * Returns the 3rd order non-linearity correction coefficient.
         * @return 3rd order non-linearity correction coefficient
         */
        double getNlCoef4() {
            return _NlCoef4;
        }
    
        /**
         * Sets the 4th order non-linearity correction coefficient
         * @param value 4th order non-linearity correction coefficient
         */
        void setNlCoef4(double value) {
            _NlCoef4 = value;
            has_NlCoef4 = true;
        }
    
        /**
         * Returns the 4th order non-linearity correction coefficient.
         * @return 4th order non-linearity correction coefficient
         */
        double getNlCoef5() {
            return _NlCoef5;
        }
    
        /**
         * Sets the 5th order non-linearity correction coefficient
         * @param value 5th order non-linearity correction coefficient
         */
        void setNlCoef5(double value) {
            _NlCoef5 = value;
            has_NlCoef5 = true;
        }
    
        /**
         * Returns the 5th order non-linearity correction coefficient.
         * @return 5th order non-linearity correction coefficient
         */
        double getNlCoef6() {
            return _NlCoef6;
        }
    
        /**
         * Sets the 6th order non-linearity correction coefficient
         * @param value 6th order non-linearity correction coefficient
         */
        void setNlCoef6(double value) {
            _NlCoef6 = value;
            has_NlCoef6 = true;
        }
    
        /**
         * Returns the 7th order non-linearity correction coefficient.
         * @return 7th order non-linearity correction coefficient
         */
        double getNlCoef7() {
            return _NlCoef7;
        }
    
        /**
         * Sets the 7th order non-linearity correction coefficient
         * @param value 7th order non-linearity correction coefficient
         */
        void setNlCoef7(double value) {
            _NlCoef7 = value;
            has_NlCoef7 = true;
        }
    
        /**
         * Returns the Polynomial order of non-linearity calibration coefficient.
         * @return 1st order non-linearity correction coefficient
         */
        int getNlOrder() {
            return _NlOrder;
        }
    
        /**
         * Sets the Polynomial order of non-linearity calibration coefficient
         * @param value 1st order non-linearity correction coefficient
         */
        void setNlOrder(int value) {
            _NlOrder = value;
            has_NlOrder = true;
        }
    
        /**
         * Returns the all of the non-linearity correction coefficients.
         * @return an array of all non-linearity correction coefficients.
         */
        std::vector<double> getNlCoefficients() {
            std::vector<double> nl(8);
            nl[0] = _NlCoef0;
            nl[1] = _NlCoef1;
            nl[2] = _NlCoef2;
            nl[3] = _NlCoef3;
            nl[4] = _NlCoef4;
            nl[5] = _NlCoef5;
            nl[6] = _NlCoef6;
            nl[7] = _NlCoef7;
            return nl;
        }
    
        /**
         * Sets all of the non-linearity correction coefficients.
         * @param nl an array of all of the non-linearity correction coefficients.
         */
        void setNlCoefficients(const std::vector<double> &nl) {
            _NlCoef0 = nl[0];
            has_NlCoef0 = true;
            _NlCoef1 = nl[1];
            has_NlCoef1 = true;
            _NlCoef2 = nl[2];
            has_NlCoef2 = true;
            _NlCoef3 = nl[3];
            has_NlCoef3 = true;
            _NlCoef4 = nl[4];
            has_NlCoef4 = true;
            _NlCoef5 = nl[5];
            has_NlCoef5 = true;
            _NlCoef6 = nl[6];
            has_NlCoef6 = true;
            _NlCoef7 = nl[7];
            has_NlCoef7 = true;
        }
        /**
         * Returns the Stray light constant.
         * @return the Stray light constant.
         */
        std::vector<double> getStrayLight() {
            std::vector<double> result(2);
            if (has_StrayLightSlope) {
                result[0] = _StrayLight;
                result[1] = strayLightSlope;
            }
            else if (has_StrayLight) {
                result[0] = _StrayLight;
                result[1] = 0.0;
            }
            return result;
        }
    
        //double getStrayLightSlope() {
        //    return strayLightSlope;
        //}
    
        /**
         * Sets the Stray light constant.
         * @param value Stray light constant.
         */
        void setStrayLight(const std::vector<double> &sl) {
            switch (sl.size()) {
                case 1:
                    _StrayLight = sl[0];
                    has_StrayLight = true;
                    break;
                case 2:
                    _StrayLight = sl[0];
                    has_StrayLight = true;
                    strayLightSlope = sl[1];
                    has_StrayLightSlope = true;
                    break;
            };
        }
    
        //void setStrayLight(double intercept, double slope) {
        //    setStrayLight(intercept);
        //    strayLightSlope = slope;
        //    has_StrayLightSlope = true;
        //}
    };
}
