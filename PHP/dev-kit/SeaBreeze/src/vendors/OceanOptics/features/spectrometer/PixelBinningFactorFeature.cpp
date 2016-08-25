/***************************************************//**
 * @file    PixelBinningFactorFeature.cpp
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
#include "common/globals.h"
#include "vendors/OceanOptics/features/spectrometer/PixelBinningFactorFeature.h"
#include "vendors/OceanOptics/protocols/interfaces/PixelBinningFactorProtocolInterface.h"
#include "vendors/OceanOptics/protocols/obp/impls/OBPPixelBinningFactorProtocol.h"
#include "vendors/OceanOptics/utils/Polynomial.h"
#include "common/exceptions/FeatureProtocolNotFoundException.h"
#include "common/exceptions/FeatureControlException.h"
#include "api/seabreezeapi/FeatureFamilies.h"

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace seabreeze::api;
using namespace std;

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif

PixelBinningFactorFeature::PixelBinningFactorFeature(vector<ProtocolHelper *> helpers) {

    vector<ProtocolHelper *>::iterator iter;
    for(iter = helpers.begin(); iter != helpers.end(); iter++) {
        this->protocols.push_back(*iter);
    }
}

PixelBinningFactorFeature::~PixelBinningFactorFeature() {

}

int PixelBinningFactorFeature::readPixelBinningFactor(const Protocol &protocol,
                const Bus &bus) throw (FeatureException) {

    PixelBinningFactorProtocolInterface *binning = NULL;
    int result = 0;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        binning = static_cast<PixelBinningFactorProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error(
                "Could not find matching protocol implementation to get pixel binning factor.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        result = binning->readPixelBinningFactor(bus);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }

    return result;
}

void PixelBinningFactorFeature::writePixelBinningFactor(const Protocol &protocol,
                const Bus &bus, const int p) throw (FeatureException) {

    PixelBinningFactorProtocolInterface *binning = NULL;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        binning = static_cast<PixelBinningFactorProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error(
                "Could not find matching protocol implementation to write pixel binning factor.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        binning->writePixelBinningFactor(bus, p);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }
}

int PixelBinningFactorFeature::readMaximumPixelBinningfactor(const Protocol &protocol,
        const Bus &bus) throw (FeatureException) {

    PixelBinningFactorProtocolInterface *binning = NULL;
    int result = 0;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        binning = static_cast<PixelBinningFactorProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error(
                "Could not find matching protocol implementation to get pixel maximum binning factor.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        result = binning->readMaximumPixelBinningfactor(bus);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }

    return result;
}

FeatureFamily PixelBinningFactorFeature::getFeatureFamily() {
    FeatureFamilies families;

    return families.SPECTROMETER;
}
