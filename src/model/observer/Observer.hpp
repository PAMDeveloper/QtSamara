/**
 * @file model/observer/Observer.hpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2013-2017 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODEL_OBSERVER_OBSERVER_HPP
#define MODEL_OBSERVER_OBSERVER_HPP

#include <model/kernel/Model.hpp>
#include <model/observer/LAIView.hpp>
#include <model/observer/NUMPHASEView.hpp>
#include <model/observer/DEGRESDUJOURView.hpp>
#include <model/observer/DEGRESDUJOURCORView.hpp>
#include <model/observer/FTSWView.hpp>
#include <model/observer/CSTRView.hpp>
#include <model/observer/ROOTFRONTView.hpp>
#include <model/observer/ETOView.hpp>
#include <model/observer/ETMView.hpp>
#include <model/observer/ETRView.hpp>
#include <model/observer/RUEView.hpp>
#include <model/observer/CONVERSIONEFFView.hpp>
#include <model/observer/ICView.hpp>
#include <model/observer/CULMSPERPLANTView.hpp>
#include <model/observer/CULMSPOPView.hpp>
#include <model/observer/CULMSPERHILLView.hpp>
#include <model/observer/GRAINYIELDPOPView.hpp>
#include <model/observer/DRYMATSTRUCTLEAFPOPView.hpp>
#include <model/observer/DRYMATSTRUCTSHEATHPOPView.hpp>
#include <model/observer/DRYMATSTRUCTROOTPOPView.hpp>
#include <model/observer/DRYMATSTRUCTINTERNODEPOPView.hpp>
#include <model/observer/DRYMATRESINTERNODEPOPView.hpp>
#include <model/observer/DRYMATSTRUCTPANICLEPOPView.hpp>
#include <model/observer/DRYMATSTRUCTTOTPOPView.hpp>
#include <model/observer/DRYMATVEGETOTPOPView.hpp>
#include <model/observer/DRYMATPANICLETOTPOPView.hpp>
#include <model/observer/DRYMATSTEMPOPView.hpp>
#include <model/observer/DRYMATABOVEGROUNDPOPView.hpp>
#include <model/observer/DRYMATTOTPOPView.hpp>
#include <model/observer/STERILITYCOLDView.hpp>
#include <model/observer/STERILITYHEATView.hpp>
#include <model/observer/STERILITYDROUGHTView.hpp>
#include <model/observer/STERILITYTOTView.hpp>
#include <model/observer/HARVESTINDEXView.hpp>
#include <model/observer/PANICLENUMPOPView.hpp>
#include <model/observer/PANICLENUMPLANTView.hpp>
#include <model/observer/GRAINYIELDPANICLEView.hpp>
#include <model/observer/SPIKENUMPOPView.hpp>
#include <model/observer/SPIKENUMPANICLEView.hpp>
#include <model/observer/FERTSPIKENUMPOPView.hpp>
#include <model/observer/GRAINFILLINGSTATUSView.hpp>
#include <model/observer/PHASESTEMELONGATIONView.hpp>
#include <model/observer/SLAView.hpp>
#include <model/observer/HAUNINDEXView.hpp>
#include <model/observer/APEXHEIGHTView.hpp>
#include <model/observer/PLANTHEIGHTView.hpp>
#include <model/observer/PLANTWIDTHView.hpp>
#include <model/observer/KROLLINGView.hpp>
#include <model/observer/LIRKDFCLView.hpp>
#include <model/observer/LTRKDFCLView.hpp>
#include <model/observer/ASSIMPOTView.hpp>
#include <model/observer/ASSIMView.hpp>
#include <model/observer/RESPMAINTTOTView.hpp>
#include <model/observer/SUPPLYTOTView.hpp>
#include <model/observer/ASSIMSURPLUSView.hpp>
#include <model/observer/ASSIMNOTUSEDView.hpp>
#include <model/observer/TILLERDEATHPOPView.hpp>
#include <model/observer/PLANTLEAFNUMTOTView.hpp>
#include <model/observer/DEADLEAFDRYWTPOPView.hpp>
#include <model/observer/LAIDEADView.hpp>
#include <model/observer/RESCAPACITYINTERNODEPOPView.hpp>
#include <model/observer/INTERNODERESSTATUSView.hpp>
#include <model/observer/DAYLENGTHView.hpp>
#include <model/observer/PARView.hpp>
#include <model/observer/RGCALCView.hpp>
#include <model/observer/VPDCALCView.hpp>
#include <model/observer/TMOYCALCView.hpp>
#include <model/observer/HMOYCALCView.hpp>
#include <model/observer/KCEREALView.hpp>
#include <model/observer/STOCKTOTALView.hpp>
#include <model/observer/EAUDISPOView.hpp>
#include <model/observer/STOCKSURFACEView.hpp>
#include <model/observer/STOCKRACView.hpp>
#include <model/observer/RURACView.hpp>
#include <model/observer/KCPView.hpp>
#include <model/observer/KCEView.hpp>
#include <model/observer/EVAPPOTView.hpp>
#include <model/observer/EVAPView.hpp>
#include <model/observer/TRPOTView.hpp>
#include <model/observer/TRView.hpp>
#include <model/observer/LRView.hpp>
#include <model/observer/DRView.hpp>
#include <model/observer/PARINTERCEPTEView.hpp>
#include <model/observer/SOMMEDEGRESJOURView.hpp>
#include <model/observer/VITESSERACINAIREView.hpp>
#include <model/observer/CSTRASSIMView.hpp>
#include <model/observer/RAYEXTRAView.hpp>
#include <model/observer/CUMPARView.hpp>
#include <model/observer/SOMMEDEGRESJOURCORView.hpp>
#include <model/observer/SUMPPView.hpp>
#include <model/observer/SOMMEDEGRESJOURPHASEPRECEDENTEView.hpp>
#include <model/observer/RESPMAINTDEBTView.hpp>
#include <model/observer/TMINMOYView.hpp>
#include <model/observer/TMAXMOYView.hpp>
#include <model/observer/FTSWMOYView.hpp>
#include <model/observer/ROOTSHOOTRATIOView.hpp>
#include <model/observer/TREFFINSTView.hpp>
#include <model/observer/TREFFView.hpp>
#include <model/observer/WUEETView.hpp>
#include <model/observer/WUETOTView.hpp>
#include <model/observer/FLOODWATERDEPTHView.hpp>
#include <model/observer/IRRIGAUTODAYView.hpp>
#include <model/observer/IRRIGTOTDAYView.hpp>
#include <model/observer/FRACTIONPLANTHEIGHTSUBMERView.hpp>
#include <model/observer/DENSITYView.hpp>
#include <model/observer/ROOTMASSPERVOLView.hpp>
#include <model/observer/VOLMACROPORESView.hpp>
#include <model/observer/STOCKMACROPORESView.hpp>
#include <model/observer/RELPOTLEAFLENGTHView.hpp>
#include <model/observer/APEXHEIGHTGAINView.hpp>
#include <model/observer/HAUNGAINView.hpp>
#include <model/observer/CONVERSIONView.hpp>
#include <model/observer/STRESSCOLDView.hpp>
#include <model/observer/FRACTIONROOTSLOGGEDView.hpp>
#include <model/observer/RESUTILView.hpp>
#include <model/observer/KCTOTView.hpp>
#include <model/observer/NBJASView.hpp>
#include <model/observer/ENDLView.hpp>
#include <model/observer/View.hpp>

#include <vector>

namespace model { namespace observer {

class Observer
{
    typedef std::vector < View* > Views;

public:
    Observer(const model::kernel::Model* model) :
        model(model)
    { }

    virtual ~Observer()
   {
        for (typename Views::iterator it = views.begin(); it != views.end();
             ++it) {
            delete (*it);
        }
    }

    void attachView(View* view)
    {
        views.push_back(view);
        view->attachModel(model);
    }

    void init()
    {
        attachView(new NUMPHASEView);
        /*attachView(new NBJASView);*/
        /*attachView(new DEGRESDUJOURView);*/
        /*attachView(new DEGRESDUJOURCORView);*/
        /*attachView(new LAIView);*/
        /*attachView(new FTSWView);*/
        //attachView(new CSTRView);
        //attachView(new ROOTFRONTView);
        /*attachView(new ETOView);*/
        /*attachView(new ETMView);*/
        /*attachView(new ETRView);
        attachView(new RUEView);*/
        //attachView(new CONVERSIONEFFView);
        /*attachView(new ICView);*/
        /*attachView(new CULMSPERPLANTView);
        attachView(new CULMSPOPView);
        attachView(new CULMSPERHILLView);*/
        //attachView(new GRAINYIELDPOPView);
        /*attachView(new DRYMATSTRUCTLEAFPOPView);*/
        /*attachView(new DRYMATSTRUCTSHEATHPOPView);
        attachView(new DRYMATSTRUCTROOTPOPView);
        attachView(new DRYMATSTRUCTINTERNODEPOPView);
        attachView(new DRYMATRESINTERNODEPOPView);
        attachView(new DRYMATSTRUCTPANICLEPOPView);
        attachView(new DRYMATSTRUCTTOTPOPView);
        attachView(new DRYMATVEGETOTPOPView);
        attachView(new DRYMATPANICLETOTPOPView);
        attachView(new DRYMATSTEMPOPView);
        attachView(new DRYMATABOVEGROUNDPOPView);
        attachView(new DRYMATTOTPOPView);
        attachView(new STERILITYCOLDView);
        attachView(new STERILITYHEATView);*/
        //attachView(new STERILITYDROUGHTView);
        //attachView(new STERILITYTOTView);
        /*attachView(new HARVESTINDEXView);
        attachView(new PANICLENUMPOPView);
        attachView(new PANICLENUMPLANTView);
        attachView(new GRAINYIELDPANICLEView);
        attachView(new SPIKENUMPOPView);
        attachView(new SPIKENUMPANICLEView);
        attachView(new FERTSPIKENUMPOPView);
        attachView(new GRAINFILLINGSTATUSView);*/
        /*attachView(new PHASESTEMELONGATIONView);*/
        /*attachView(new SLAView);*/
        /*attachView(new HAUNINDEXView);*/
        /*attachView(new APEXHEIGHTView);*/
        /*attachView(new PLANTHEIGHTView);*/
        /*attachView(new PLANTWIDTHView);*/
        /*attachView(new KROLLINGView);
        attachView(new LIRKDFCLView);*/
        /*attachView(new LTRKDFCLView);*/
        /*attachView(new ASSIMPOTView);
        attachView(new ASSIMView);
        attachView(new RESPMAINTTOTView);
        attachView(new SUPPLYTOTView);
        attachView(new ASSIMSURPLUSView);
        attachView(new ASSIMNOTUSEDView);
        attachView(new TILLERDEATHPOPView);
        attachView(new PLANTLEAFNUMTOTView);
        attachView(new DEADLEAFDRYWTPOPView);
        attachView(new LAIDEADView);
        attachView(new RESCAPACITYINTERNODEPOPView);
        attachView(new INTERNODERESSTATUSView);
        attachView(new DAYLENGTHView);
        attachView(new PARView);
        attachView(new RGCALCView);
        attachView(new VPDCALCView);
        attachView(new TMOYCALCView);
        attachView(new HMOYCALCView);*/
        /*attachView(new KCEREALView);*/
        /*attachView(new STOCKTOTALView);*/
        /*attachView(new EAUDISPOView);*/
        /*attachView(new STOCKSURFACEView);*/
        //attachView(new STOCKRACView);
        //attachView(new RURACView);
        /*attachView(new KCPView);*/
        /*attachView(new KCEView);*/
        /*attachView(new EVAPPOTView);*/
        /*attachView(new EVAPView);*/
        /*attachView(new TRPOTView);*/
        /*attachView(new TRView);*/
        /*attachView(new LRView);
        attachView(new DRView);
        attachView(new PARINTERCEPTEView);
        attachView(new SOMMEDEGRESJOURView);
        attachView(new VITESSERACINAIREView);
        attachView(new CSTRASSIMView);
        attachView(new RAYEXTRAView);
        attachView(new CUMPARView);
        attachView(new SOMMEDEGRESJOURCORView);
        attachView(new SUMPPView);
        attachView(new SOMMEDEGRESJOURPHASEPRECEDENTEView);
        attachView(new RESPMAINTDEBTView);
        attachView(new TMINMOYView);
        attachView(new TMAXMOYView);*/
        /*attachView(new FTSWMOYView);*/
        /*attachView(new ROOTSHOOTRATIOView);
        attachView(new TREFFINSTView);
        attachView(new TREFFView);
        attachView(new WUEETView);
        attachView(new WUETOTView);
        attachView(new FLOODWATERDEPTHView);
        attachView(new IRRIGAUTODAYView);
        attachView(new IRRIGTOTDAYView);
        attachView(new FRACTIONPLANTHEIGHTSUBMERView);
        attachView(new DENSITYView);
        attachView(new ROOTMASSPERVOLView);
        attachView(new VOLMACROPORESView);
        attachView(new STOCKMACROPORESView);
        attachView(new RELPOTLEAFLENGTHView);
        attachView(new APEXHEIGHTGAINView);
        attachView(new HAUNGAINView);*/
        //attachView(new CONVERSIONView);
        /*attachView(new STRESSCOLDView);
        attachView(new FRACTIONROOTSLOGGEDView);
        attachView(new RESUTILView);
        attachView(new KCTOTView);*/
        attachView(new ENDLView);
    }

    void observe(double t)
    {
        for (typename Views::iterator it = views.begin(); it != views.end();
             ++it) {
            (*it)->observe(t);
        }
    }

private:
    const model::kernel::Model* model;
    Views views;
};

} } // namespace tnt model

#endif
